set(CORE_SRCS
actionmodel.cpp
../include/actionmodel.h
notification.cpp
../include/notification.h
notificationmodel.cpp
../include/notificationmodel.h
notificationserver.cpp
../include/notificationserver.h
notificationclient.cpp
../include/notificationclient.h
)

# Build everything twice. Since we have only a few
# source files, this is not an issue. If it does
# become one, change tools so they use the plugin
# instead of calling the classes directly.

add_library(notifybackend SHARED
${CORE_SRCS}
)

add_library(notifyplugin MODULE
${CORE_SRCS}
notificationplugin.cpp
../include/notificationplugin.h
)

qt5_use_modules(notifybackend Widgets DBus Qml)
qt5_use_modules(notifyplugin Widgets DBus Qml)

install(
  TARGETS notifyplugin
  ARCHIVE DESTINATION ${LIBDIR}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION ${LIBDIR}
)
