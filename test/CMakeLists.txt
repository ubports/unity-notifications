add_executable(notificationtest notificationtest.cpp)
target_link_libraries(notificationtest notifybackend)
qt5_use_modules(notificationtest Gui Core Widgets)

add_test(notification notificationtest)


# Disabled until frontend and backend are fully
# merged to respective trunks.
#add_qml_test(NotificationInterface)
