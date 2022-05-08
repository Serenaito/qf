#ifndef QF_LOG_H
#define QF_LOG_H
#include <qdebug.h>
#ifdef QF_DEBUG
#define QF_LOG_DEBUG(fmt, ...) qDebug("[%s,%d] debug:"fmt"\n",__FUNCTION__,__LINE__,##__VA_ARGS__)
#else
#define QF_LOG_DEBUG(fmt, ...)
#endif

#define QF_LOG_INFO(fmt, ...) qDebug("[%s,%d] info:"fmt"\n",__FUNCTION__,__LINE__,##__VA_ARGS__)
#define QF_LOG_ERROR(fmt, ...) qDebug("[%s,%d] error:"fmt"\n",__FUNCTION__,__LINE__,##__VA_ARGS__)

#endif // QF_LOG_H
