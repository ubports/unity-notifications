add_library(notifybackend SHARED
notification.cpp
../include/notification.h
notificationbackend.cpp
renderer.cpp
notificationmodel.cpp
../include/notificationmodel.h
)

qt5_use_modules(notifybackend Core)
