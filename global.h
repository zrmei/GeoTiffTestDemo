#ifndef GLOBAL_H
#define GLOBAL_H

#define MoveToOtherThread(obj) do { \
    QThread *__t = new QThread(); \
    __t->setProperty("self", true);\
    obj->moveToThread(__t); \
    QObject::connect(obj, &QObject::destroyed, __t, &QThread::terminate);\
    __t->start(); \
} while(false)

#define TerminateThisThread(obj) do { if(obj->thread()->property("self").toBool()) obj->thread()->terminate();  } while(false)

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

#endif // GLOBAL_H
