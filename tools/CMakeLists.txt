add_executable(viewer viewer.cpp)
qt5_use_modules(viewer Widgets)
target_link_libraries(viewer notifybackend)
