if(${private_dbus})
  set(PRIVATE_DBUS 1)
else()
  set(PRIVATE_DBUS 0)
endif()

configure_file(notify-backend.h.in notify-backend.h @ONLY)
