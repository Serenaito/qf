#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <mutex>

template <int _size>
struct ptr
{
	typedef int type;
};

template<>
struct ptr<sizeof(int)>
{
	typedef int type;
};

template<>
struct ptr<sizeof(long long)>
{
	typedef long long type;
};

class msg_queue
{
public:
	struct message
	{
        virtual void release()=0;
	};

	enum class status
	{
		error,
		success,
		fail
	};

	msg_queue();
	~msg_queue();
	msg_queue::status initialize(int _size, int _max_count);
    void release();
	msg_queue::status post(void* data);
	msg_queue::status get(void* data);
private:
    bool is_init;
	ptr<sizeof(void*)>::type buffer;
	int block_size;
	int algin_block_size;
	int max_count;
	int available_block;
	int first_block;
	std::mutex* mut;
	std::condition_variable* cond;
};




#endif // !MESSAGE_QUEUE_H

