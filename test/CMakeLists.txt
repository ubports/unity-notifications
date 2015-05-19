add_executable(notificationtest notificationtest.cpp)
target_link_libraries(notificationtest notifybackend)
qt5_use_modules(notificationtest Gui Test Core DBus Widgets)

add_test(notification notificationtest)
set_tests_properties(notification PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=minimal")

# Disabled until frontend and backend are fully
# merged to respective trunks.
#add_qml_test(NotificationInterface)
