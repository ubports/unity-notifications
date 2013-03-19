add_executable(viewer
viewer.cpp
testWindow.ui
)

qt5_use_modules(viewer Widgets)
target_link_libraries(viewer notifybackend)
