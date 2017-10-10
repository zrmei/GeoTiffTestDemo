#ifndef LOGGING_H
#define LOGGING_H

#include <QtGlobal>
#include <QLoggingCategory>

// Defines for configuring the used loglevels
// just uncomment if you want to hide one level
#ifdef SFT_NO_DEBUG
#define QLOG_NO_TRACE
#define QLOG_NO_DEBUG
#define QLOG_NO_INFO
#define QLOG_NO_WARN
#define QLOG_NO_ERROR
#endif

// logging macros
#ifdef QLOG_NO_TRACE
    #define QLOG_TRACE(...) if(1){} else qCCritical(__VA_ARGS__)
    #define LOG_TRACE() if(1){} else  qCCritical(____Category) << __func__
#else
    #define QLOG_TRACE(...) qCCritical(__VA_ARGS__) << __func__
    #define LOG_TRACE() qCCritical(____Category) << __func__
#endif

#ifdef QLOG_NO_DEBUG
    #define QLOG_DEBUG(...) if(1){} else qCDebug(__VA_ARGS__)
    #define LOG_DEBUG() if(1){} else qCDebug(____Category)
#else
    #define QLOG_DEBUG(...) qCDebug(__VA_ARGS__) << __func__
    #define LOG_DEBUG() qCDebug(____Category) << __func__
#endif

#ifdef QLOG_NO_INFO
    #define QLOG_INFO(...)  if(1){} else qCInfo(__VA_ARGS__)
    #define LOG_INFO() if(1){} qCInfo(____Category) << __func__
#else
    #if QT_VERSION < QT_VERSION_CHECK(5, 5, 0)
        #define QLOG_INFO() qDebug()
    #else
        #define QLOG_INFO(...) qCInfo(__VA_ARGS__) << __func__
        #define LOG_INFO() qCInfo(____Category) << __func__
    #endif
#endif

#ifdef QLOG_NO_WARN
    #define QLOG_WARN(...)  if(1){} else qCWarning(__VA_ARGS__)
    #define LOG_WARN() if(1){} else qCWarning(____Category) << __func__
#else
    #define QLOG_WARN(...) qCWarning(__VA_ARGS__) << __func__
    #define LOG_WARN() qCWarning(____Category) << __func__
#endif

#ifdef QLOG_NO_ERROR
    #define QLOG_ERROR(...) if(1){} else qCCritical(__VA_ARGS__)
    #define LOG_ERROR() if(1){} else qCCritical(____Category)
#else
    #define QLOG_ERROR(...) qCCritical(__VA_ARGS__) << __func__
    #define LOG_ERROR() qCCritical(____Category) << __func__
#endif

#ifdef QLOG_NO_ERROR
    #define QLOG_FATAL(...) if(1){} else qCCritical(__VA_ARGS__)
    #define LOG_FATAL() if(1){} else qCCritical(____Category)
#else
    #define QLOG_FATAL(...) qCCritical(__VA_ARGS__) << __func__
    #define LOG_FATAL() qCCritical(____Category) << __func__
#endif


#define Q_LOGGING_CATEGORY_EX(name, ...) \
    static Q_LOGGING_CATEGORY(name ## Category, __VA_ARGS__)

#define Q_LOGGING_CATEGORY_BY(name) \
    static Q_LOGGING_CATEGORY(name ## Category, APPLICATION_NAME  "."  #name ".log")

#define Q_LOGGING_CATEGORY_DEFAULT(name) \
    Q_LOGGING_CATEGORY_BY(name) \
    static Q_LOGGING_CATEGORY(____Category, APPLICATION_NAME  "."  #name ".log")

#ifdef SFT_NO_DEBUG
#undef qCDebug
#define qCDebug(...) if(1){} else qDebug()
#endif

#endif // LOGGING_H
