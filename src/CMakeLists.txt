add_library(notifybackend SHARED
notification.cpp
../include/notification.h
notificationbackend.cpp
renderer.cpp
notificationmodel.cpp
../include/notificationmodel.h
notificationserver.cpp
../include/notificationserver.h
notificationclient.cpp
../include/notificationclient.h
)

qt5_use_modules(notifybackend Widgets DBus)
