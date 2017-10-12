#ifndef GLOBAL_H
#define GLOBAL_H

#include <QtGlobal>
#include <QVariant>
#include <atomic>
#include <mutex>

#define Q_DECLARE_STATIC_INSTANCE(classname)\
    static classname *Instance(); \
    Q_DISABLE_COPY(classname)\
    friend struct QScopedPointerDeleter<classname>

#define Q_STATIC_INSTANCE(classname, ...) \
classname * classname::Instance() \
{\
    static QScopedPointer<classname> instance{ nullptr };\
    static std::once_flag oc;\
    std::call_once(oc, [] { \
        if(instance.isNull()) {\
            instance.reset(new classname(__VA_ARGS__));\
        }\
    });\
    return instance.data(); \
}

namespace Global {
QVariant loadSettings(const QString &key, const QVariant &val = QVariant(), const QString &group = QString());
void     saveSettings(const QString &key, const QVariant &val, const QString &group = QString());
}

#endif // GLOBAL_H
