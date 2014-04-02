include(FindPkgConfig)
pkg_check_modules(NOTIFICATIONS_API REQUIRED unity-shell-launcher=3)

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
${NOTIFICATIONS_API_INCLUDEDIR}/unity/shell/notifications/ModelInterface.h
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
