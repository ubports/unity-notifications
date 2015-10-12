include(FindPkgConfig)
pkg_check_modules(NOTIFICATIONS_API REQUIRED unity-shell-notifications=3)

set(CORE_SRCS
ActionModel.cpp
../include/ActionModel.h
DBusTypes.cpp
../include/DBusTypes.h
Notification.cpp
../include/Notification.h
NotificationModel.cpp
../include/NotificationModel.h
NotificationServer.cpp
../include/NotificationServer.h
NotificationClient.cpp
../include/NotificationClient.h
${NOTIFICATIONS_API_INCLUDEDIR}/unity/shell/notifications/ModelInterface.h
)

set_source_files_properties(
    "org.freedesktop.Notifications.xml"
    PROPERTIES
    NO_NAMESPACE YES
    INCLUDE "DBusTypes.h"
)

qt5_add_dbus_adaptor(
    CORE_SRCS
    "org.freedesktop.Notifications.xml"
    NotificationServer.h
    NotificationServer
    NotificationsAdaptor
)

qt5_add_dbus_interface(
    CORE_SRCS
    "org.freedesktop.Notifications.xml"
    NotificationsInterface
)

add_library(notifybackend STATIC
${CORE_SRCS}
)

add_library(notifyplugin MODULE
NotificationPlugin.cpp
../include/NotificationPlugin.h
)

set_target_properties(notifyplugin PROPERTIES
  SOVERSION ${SONAME}
  VERSION ${SOVERSION}
)

target_link_libraries(
  notifyplugin
  notifybackend
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

target_link_libraries(
  notifyclientplugin
  notifybackend
)

qt5_use_modules(notifybackend Widgets DBus Qml)

execute_process(COMMAND ${PKG_CONFIG_EXECUTABLE} --variable=plugindir_suffix unity-shell-api OUTPUT_VARIABLE SHELL_INSTALL_QML OUTPUT_STRIP_TRAILING_WHITESPACE)
if(SHELL_INSTALL_QML STREQUAL "")
    message(FATAL_ERROR "Could not determine plugin installation dir.")
endif()
 
execute_process(COMMAND ${PKG_CONFIG_EXECUTABLE} --variable=plugindir unity-shell-api OUTPUT_VARIABLE SHELL_PLUGINDIR OUTPUT_STRIP_TRAILING_WHITESPACE)
if(SHELL_PLUGINDIR STREQUAL "")
    message(FATAL_ERROR "Could not determine plugin import dir.")
endif()

install(
  TARGETS notifyplugin notifyclientplugin
  ARCHIVE DESTINATION ${SHELL_PLUGINDIR}/Unity/Notifications
  LIBRARY DESTINATION ${SHELL_PLUGINDIR}/Unity/Notifications
)

install(FILES qmldir
  DESTINATION ${SHELL_INSTALL_QML}/Unity/Notifications
)
