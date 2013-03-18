add_library(notifybackend SHARED notification.cpp notificationbackend.cpp renderer.cpp notificationmodel.cpp)

qt5_use_modules(notifybackend Core)