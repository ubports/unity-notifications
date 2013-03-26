#include "notification.h"
#include "notificationmodel.h"

#include <QGuiApplication>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include <QQuickView>

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    QQuickView view;


    QSharedPointer<Notification> msg(new Notification(55, URGENCY_LOW, "default notification", ASYNCHRONOUS));
    NotificationModel *m = new NotificationModel();
    m->insertNotification(msg);
    // Shared pointer problem: http://qt-project.org/wiki/SharedPointersAndQmlOwnership
    view.rootContext()->setContextProperty("notificationmodel", m);
    /* Hardcoded URLs are bad but tolerable here in test code. */
    view.setSource(QUrl::fromLocalFile("../tools/datatest.qml"));
    view.show();
    return app.exec();
}
