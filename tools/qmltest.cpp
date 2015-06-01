#include "Notification.h"
#include "NotificationModel.h"
#include "NotificationServer.h"

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

    new NotificationServer(QDBusConnection::sessionBus(), *m, &app);

    // Shared pointer problem: http://qt-project.org/wiki/SharedPointersAndQmlOwnership
    view.rootContext()->setContextProperty("notificationmodel", m);
    qmlRegisterType<Notification>("Notification", 1, 0, "Notification");
    /* Hardcoded URLs are bad but tolerable here in test code. */
    view.setSource(QUrl::fromLocalFile("../tools/datatest.qml"));
    view.show();
    return app.exec();
}
