Unity notifications

This project provides an implementation of the
Free Desktop Notification server for Unity. It also
provides some additional features as needed
on alternative platforms, such as mobile.

Since the Dash is implemented in QML, this functionality
is implemented as a QML plugin. Notification status is provided
as a QAbstractListModel, making integration simple. The exact
form of the API is defined by the Dash, Unity-notifications
only implements it.

Unity-notifications does not and will not provide a C/C++
API for sending or receiving notifications. It is QML only.

In the roadmap there is also plans to provide a QML plugin
for sending notifications from client applications.
