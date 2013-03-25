#include "notification.h"

#include <QGuiApplication>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include <QQuickView>

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    QQuickView view;

    Notification msg;
    view.rootContext()->setContextProperty("msg", &msg);
    /* Hardcoded URLs are bad but tolerable here in test code. */
    view.setSource(QUrl::fromLocalFile("../tools/datatest.qml"));
    view.show();
    return app.exec();
}
