qt5_wrap_ui(GUI_HEADERS mainWindow.ui)

add_executable(viewer
viewer.cpp
mainWindow.cpp
mainWindow.h
${GUI_HEADERS}
)

qt5_use_modules(viewer Widgets)
target_link_libraries(viewer notifybackend)

add_executable(qmltest qmltest.cpp)

qt5_use_modules(qmltest Qml Quick)
target_link_libraries(qmltest notifybackend)

add_executable(dbustool dbustool.cpp)
qt5_use_modules(dbustool Widgets DBus)
target_link_libraries(dbustool notifybackend)