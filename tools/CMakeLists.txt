qt5_wrap_ui(GUI_HEADERS mainWindow.ui)

add_executable(viewer
viewer.cpp
mainWindow.cpp
mainWindow.h
${GUI_HEADERS}
)

qt5_use_modules(viewer Widgets)
target_link_libraries(viewer notifybackend)
