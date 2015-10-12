qt5_wrap_ui(GUI_HEADERS mainWindow.ui serverMainWindow.ui clientMainWindow.ui)

# Viewer app
############

add_executable(viewer
  viewer.cpp
  mainWindow.cpp
  mainWindow.h
  ${GUI_HEADERS}
)

qt5_use_modules(viewer Widgets)

target_link_libraries(viewer notifybackend)

# QMLtest app
#############

add_executable(qmltest
  qmltest.cpp
)

qt5_use_modules(qmltest
  Qml
  Quick
  DBus
  Widgets
)

target_link_libraries(qmltest notifybackend)

# DBus server app
#################

add_executable(dbusserver
  dbusserver.cpp
  serverMainWindow.cpp
  ${GUI_HEADERS}
)

qt5_use_modules(dbusserver
  Widgets
  DBus
)

target_link_libraries(dbusserver notifybackend)

# DBus client app
#################

add_executable(dbusclient
  dbusclient.cpp
  clientMainWindow.cpp
  ${GUI_HEADERS}
)

qt5_use_modules(dbusclient
  Widgets
  DBus
)

target_link_libraries(dbusclient notifybackend)

# Move files around
###################

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Notifications
    DESTINATION ${CMAKE_BINARY_DIR}/tools
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/datatest.qml
    DESTINATION ${CMAKE_BINARY_DIR}/tools
)
