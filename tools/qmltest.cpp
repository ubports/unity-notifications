#include "notification.h"
#include "notificationmodel.h"
#include "notificationserver.h"

#include <QDBusAbstractAdaptor>
#include <QDBusConnection>
#include <QApplication>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include <QQuickView>
#include <QTimer>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QQuickView view;
    NotificationModel *m = new NotificationModel();

    new NotificationServer(*m, &app);

    if(!QDBusConnection::sessionBus().registerService(DBUS_SERVICE_NAME)) {
        printf("Service name already taken.\n");
        return 1;
    }
    if(!QDBusConnection::sessionBus().registerObject(DBUS_PATH, &app)) {
        printf("Could not register to DBus session.\n");
        return 1;
    }

    // Shared pointer problem: http://qt-project.org/wiki/SharedPointersAndQmlOwnership
    view.rootContext()->setContextProperty("notificationmodel", m);
    qmlRegisterType<Notification>("Notification", 0, 1, "Notification");
    /* Hardcoded URLs are bad but tolerable here in test code. */
    view.setSource(QUrl::fromLocalFile("../tools/datatest.qml"));
    view.show();
    return app.exec();
}
