
function(add_notification_test NAME)
    add_executable(${NAME} "${NAME}.cpp")

    target_link_libraries(${NAME}
        notifybackend
        ${QTDBUSTEST_LDFLAGS}
    )

    qt5_use_modules(${NAME}
        Core
        DBus
        Gui
        Qml
        Test
        Widgets
    )

    add_test(${NAME} ${NAME})

    set_tests_properties(${NAME} PROPERTIES
        ENVIRONMENT "QT_QPA_PLATFORM=minimal"
    )
endfunction()

pkg_check_modules(QTDBUSTEST REQUIRED libqtdbustest-1 REQUIRED)
include_directories(${QTDBUSTEST_INCLUDE_DIRS})

set(DBUS_SERVER_BINARY "${CMAKE_BINARY_DIR}/tools/dbusserver")
add_definitions(-DDBUS_SERVER_BINARY="${DBUS_SERVER_BINARY}")

add_notification_test(notificationtest)
add_notification_test(notificationservertest)

# Disabled until frontend and backend are fully
# merged to respective trunks.
#add_qml_test(NotificationInterface)
