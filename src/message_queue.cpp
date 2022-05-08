#include "message_queue.hpp"
#include <assert.h>
#include <stdint.h>
#include "qf_log.h"
namespace 
{
    constexpr int message_max_size = 32;
    constexpr int message_max_count = 64;
}

msg_queue::msg_queue()
{
    algin_block_size = 0;
    block_size = 0;
    max_count = 0;
    available_block = 0;
    first_block = 0;
    buffer = 0;
    mut = 0;
    cond = 0;
    is_init = false;
}

msg_queue::status msg_queue::initialize(int _size, int _max_count) {
    if(is_init == true)
    {
        QF_LOG_INFO("have been initialized");
        return msg_queue::status::success;
    }

    assert(_size > 0);
    assert(_max_count > 0);
    assert(_size <= message_max_size);
    assert(_max_count < message_max_count);
    block_size = _size;
    max_count = _max_count;
    if ((block_size & 0x1) == 1)
    {
        algin_block_size = ((block_size >> 2) | 1) << 2;
    }
    else
    {
        algin_block_size = block_size;
    }
    buffer = 0;
    buffer = (ptr<sizeof(void*)>::type)new uint8_t[algin_block_size * max_count + 4];
    available_block = _max_count;
    do{
        if (buffer == 0)
        {
            break;
        }

        int move_position = buffer % 4;
        if (move_position == 0)
        {
            buffer = buffer + 4;
            *(uint8_t*)(buffer - 1) = 4;
        }
        else
        {
            buffer = buffer + 4 - move_position;
            *(uint8_t*)(buffer - 1) = 4 - move_position;
        }

        mut = new std::mutex();
        if(mut == nullptr)
        {
            buffer = buffer - (*(uint8_t*)(buffer - 1));
            delete[] (uint8_t*)buffer;
            break;
        }

        cond = new std::condition_variable();
        if(cond == nullptr)
        {
            buffer = buffer - (*(uint8_t*)(buffer - 1));
            delete[] (uint8_t*)buffer;
            delete mut;
            break;
        }
        is_init = true;
        return msg_queue::status::success;
    }while(0);

    return msg_queue::status::error;
}

msg_queue::status msg_queue::post(void* data)
{
    if(is_init == false)
    {
        QF_LOG_INFO("not initialized");
        return msg_queue::status::error;
    }

    std::unique_lock<std::mutex> lock(*mut);
    if (available_block > 0)
    {
        memcpy((uint8_t*)(buffer + first_block * algin_block_size), data, block_size);
        if (available_block == 1)
        {
            first_block = (first_block + 1) % max_count;
        }
        else
        {
            int next_block = (first_block + 1) % max_count;
            *(uint8_t*)(buffer + next_block * algin_block_size) = first_block;
            first_block = next_block;
        }
        available_block--;
        cond->notify_one();
        return msg_queue::status::success;
    }
    return msg_queue::status::error;
}

msg_queue::status msg_queue::get(void* data)
{
    if(is_init == false)
    {
        QF_LOG_INFO("not initialized");
        return msg_queue::status::error;
    }

    std::unique_lock<std::mutex> lock(*mut);
    while (available_block == max_count)
    {
        cond->wait(lock);
    }

    if (available_block == 0)
    {
        memcpy(data, (uint8_t*)(buffer + first_block * algin_block_size), block_size);
        *(uint8_t*)(buffer + first_block * algin_block_size) = (first_block + 1) % max_count;
    }
    else
    {
        ptr<sizeof(void*)>::type ptr = buffer + first_block * algin_block_size;
        int head = *(uint8_t*)ptr;
        memcpy(data, (uint8_t*)(buffer + head * algin_block_size), block_size);
        *(uint8_t*)ptr = (head + 1) % max_count;
    }

    available_block++;
    return msg_queue::status::success;
}

void msg_queue::release()
{
    if(is_init == false)
    {
        return;
    }

    {
        std::unique_lock<std::mutex> lock(*mut);
        is_init = false;
        msg_queue* data;
        while (available_block < max_count)
        {
            QF_LOG_INFO("message release");
            if (available_block == 0)
            {
                data = (msg_queue*)(buffer + first_block * algin_block_size);
                *(uint8_t*)(buffer + first_block * algin_block_size) = (first_block + 1) % max_count;
            }
            else
            {
                ptr<sizeof(void*)>::type ptr = buffer + first_block * algin_block_size;
                int head = *(uint8_t*)ptr;
                data = (msg_queue*)(buffer + head * algin_block_size);
                *(uint8_t*)ptr = (head + 1) % max_count;
            }
            data->release();
            available_block++;
        }
    }
    buffer = buffer - (*(uint8_t*)(buffer - 1));
    delete[] (uint8_t*)buffer;
    delete mut;
    delete cond;
    QF_LOG_INFO("release success");
}
msg_queue::~msg_queue()
{
    release();
}
