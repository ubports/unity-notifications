set(CORE_SRCS
ActionModel.cpp
../include/ActionModel.h
Notification.cpp
../include/Notification.h
NotificationModel.cpp
../include/NotificationModel.h
NotificationServer.cpp
../include/NotificationServer.h
NotificationClient.cpp
../include/NotificationClient.h
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
NotificationPlugin.cpp
../include/NotificationPlugin.h
)

set_target_properties(notifyplugin PROPERTIES
  SOVERSION ${SONAME}
  VERSION ${SOVERSION}
  )

add_library(notifyclientplugin MODULE
NotificationClientPlugin.cpp
../include/NotificationClientPlugin.h
NotificationClient.cpp
../include/NotificationClient.h
)

set_target_properties(notifyclientplugin PROPERTIES
  SOVERSION ${SONAME}
  VERSION ${SOVERSION}
  )

qt5_use_modules(notifybackend Widgets DBus Qml)
qt5_use_modules(notifyplugin Widgets DBus Qml)
qt5_use_modules(notifyclientplugin Widgets DBus Qml)

install(
  TARGETS notifyplugin notifyclientplugin
  ARCHIVE DESTINATION ${QT_IMPORTS_DIR}/Unity/Notifications.${SONAME}
  LIBRARY DESTINATION ${QT_IMPORTS_DIR}/Unity/Notifications.${SONAME}
)

install(FILES qmldir
  DESTINATION ${QT_IMPORTS_DIR}/Unity/Notifications.${SONAME}
)
