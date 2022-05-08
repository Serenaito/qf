#include <GL/glew.h> // glew must put first,and can not include QtOpenGL
#include <dwmapi.h>
#include <QtGui>
#include <math.h>
//#include <QtDebug>
//#include <QtOpenGL>
#include <Windows.h>
#include <winuser.h>
#include "LAppDelegate.hpp"
#include "glwidget.h"
#include "resource_loader.hpp"
#include <QApplication>
namespace {
    constexpr int frame = 40;
    constexpr int fps = 1000/frame;
}

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent)
{
    this->startTimer(fps);
}

GLWidget::~GLWidget()
{

}

void GLWidget::initializeGL()
{
    LAppDelegate::GetInstance()->Initialize(this);
    LAppDelegate::GetInstance()->resize(this->width(),this->height());
}

void GLWidget::paintGL()
{
   // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //qDebug("GLWidget::paintGL()");
    LAppDelegate::GetInstance()->update();
}

void GLWidget::resizeGL(int width, int height)
{
#if 0
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    double halfW = width / 2.0;
    double halfH = height / 2.0;
    glOrtho(-halfW, halfW, -halfH, +halfH, 4.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
#endif
    //qDebug("width:%d,height:%d",width,height);
    LAppDelegate::GetInstance()->resize(width,height);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    if(resource_loader::get_instance().moveable())
    {
        QApplication::sendEvent(this->parent(), event);
    }
    else
    {
        int x = event->x();
        int y = event->y();
        //qDebug("x:%d y:%d",x,y);
        LAppDelegate::GetInstance()->mousePressEvent(x,y);
    }
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(resource_loader::get_instance().moveable())
    {
        QApplication::sendEvent(this->parent(), event);
    }
    else
    {
        int x = event->x();
        int y = event->y();
        //qDebug("x:%d y:%d",x,y);
        LAppDelegate::GetInstance()->mouseReleaseEvent(x,y);
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(resource_loader::get_instance().moveable())
    {
        QApplication::sendEvent(this->parent(), event);
    }
    else
    {
        int x = event->x();
        int y = event->y();
        //qDebug("x:%d y:%d",x,y);
        LAppDelegate::GetInstance()->mouseMoveEvent(x,y);
    }
}

void GLWidget::timerEvent(QTimerEvent*)
{
    this->updateGL();
}

void GLWidget::closeEvent(QCloseEvent * e)
{
    QApplication::sendEvent(this->parent(), e);
}
