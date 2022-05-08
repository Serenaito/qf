#include "mainwindow.h"
#include "event_handler.hpp"
#include "resource_loader.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    if(event_handler::get_instance().initialize() == false)
    {
        return 0;
    }

    if(resource_loader::get_instance().initialize() == false)
    {
        return 0;
    }
    QApplication a(argc, argv);
    MainWindow w(nullptr,&a);
    w.show();
    return a.exec();
}
