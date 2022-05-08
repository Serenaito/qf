#include "event_handler.hpp"
#include "message_queue.hpp"
#include <thread>
#include "resource_loader.hpp"
#include "qf_log.h"
#include "cJSON.h"
#include <QtEvents>
#include <QApplication>
namespace
{
    constexpr int max_msg_count = 8;

    struct msg_packet:msg_queue::message
    {
        event_handler::event_type type;
        void* data;
        void release() override
        {
            free(data);
            data = NULL;
        }
    };

    std::thread handle_thread;
    msg_queue mq;
    QMainWindow* qmw;

    void handle_task()
    {
        msg_packet packet;
        while(1)
        {
            if(mq.get(&packet)==msg_queue::status::success)
            {
                switch (packet.type)
                {
                case event_handler::event_type::app_exit:
                {
                     QF_LOG_INFO("thread exit");
                     return;
                }
                break;
                case event_handler::event_type::app_config_change:
                {
                    char* config = (char*)packet.data;
                    const char* config_path = resource_loader::get_instance().get_config_path();
                    if(config!=NULL)
                    {
                        FILE* fd;
                        fopen_s(&fd,config_path ,"w");
                        if(fd == NULL)
                        {
                            QF_LOG_ERROR("open file fail:%s",config_path);
                        }
                        else
                        {
                            QF_LOG_INFO("save config success");
                            fwrite(config,1,strlen(config),fd);
                        }
                        cJSON_free(config);
                        fclose(fd);
                    }
                }
                break;
                case event_handler::event_type::app_all_modle_load_fail:
                    if(qmw)
                    {
                        QF_LOG_INFO("no modle load");
                        QApplication::postEvent(qmw,new QfQevent("no model use",QfQevent::event_type::no_modle));
                    }
                    break;
                case event_handler::event_type::app_current_modle_fail_by_initialize:
                    if(qmw)
                    {
                        QF_LOG_INFO("position update");
                        QApplication::postEvent(qmw,new QfQevent("load current model fail,so load default model",QfQevent::event_type::load_default_model));
                    }
                default:
                    QF_LOG_INFO("error msg:%d",packet.data);
                break;

                }
            }
        }
    }
    const QEvent::Type mtype = (QEvent::Type)QEvent::registerEventType();
}

QfQevent::QfQevent(const char* why,event_type e):QEvent(mtype)
{
    this->why = why;
    this->e = e;
}

event_handler& event_handler::get_instance()
{
    static event_handler handler;
    return handler;
}

void event_handler::report(event_handler::event_type e,void* data)
{
    msg_packet pack;
    pack.data = data;
    pack.type = e;
    mq.post(&pack);
}

void event_handler::release()
{
    if(is_init)
    {
        msg_packet pack;
        pack.data = NULL;
        pack.type = event_handler::event_type::app_exit;
        mq.post(&pack);
        handle_thread.join();
        mq.release();
        is_init = false;
        QF_LOG_INFO("release");
    }
    else
    {
        QF_LOG_INFO("no release");
    }
}

bool event_handler::initialize()
{
    if(is_init)
    {
        return true;
    }

    if(mq.initialize(sizeof(msg_packet),max_msg_count)!=msg_queue::status::success)
    {
        QF_LOG_ERROR("initialize mq fail");
        return false;
    }
    handle_thread = std::thread(handle_task);
    is_init = true;
    return true;
}

event_handler::event_handler()
{
    is_init = false;
}

event_handler::~event_handler()
{
    this->release();
}

void event_handler::resgist_main_window(QMainWindow* mw)
{
    qmw = mw;
}
