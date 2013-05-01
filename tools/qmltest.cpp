#include "notification.h"
#include "notificationmodel.h"

#include <QGuiApplication>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include <QQuickView>
#include <QTimer>

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    QQuickView view;

    QSharedPointer<Notification> msg1(new Notification(55, 5000, Notification::Urgency::Low, "default notification", Notification::Type::Ephemeral));
    QSharedPointer<Notification> msg2(new Notification(56, 5000, Notification::Urgency::Normal, "default notification", Notification::Type::Ephemeral));
    QSharedPointer<Notification> msg3(new Notification(57, 5000, Notification::Urgency::Critical, "default notification", Notification::Type::Ephemeral));
    QSharedPointer<Notification> msg4(new Notification(58, 10000, Notification::Urgency::Normal, "default notification", Notification::Type::SnapDecision));
    NotificationModel *m = new NotificationModel();

    msg1->setSummary("Critical urgency");
    msg1->setBody("Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.");
    msg1->setIcon("graphics/avatar1.jpg");
    msg1->setSecondaryIcon("graphics/icon_phone.png");
    msg1->setUrgency(Notification::Urgency::Critical);

    msg2->setSummary("Normal urgency");
    msg2->setBody("At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.");
    msg2->setIcon("graphics/avatar3.jpg");
    msg2->setSecondaryIcon("graphics/icon_phone.png");
    msg2->setUrgency(Notification::Urgency::Normal);

    msg3->setSummary("Low urgency");
    msg3->setBody("Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.");
    msg3->setIcon("graphics/avatar2.jpg");
    msg3->setSecondaryIcon("graphics/facebook.png");
    msg3->setUrgency(Notification::Urgency::Low);

    msg4->setSummary("Snap-decision example");
    msg4->setBody("This is a Snap-decision with buttons (hopefully) and a longer timeout.");
    msg4->setIcon("graphics/avatar1.jpg");
    msg4->setSecondaryIcon("graphics/icon_phone.png");
    QStringList actions;
    actions << "Accept" << "accept_id" << "Cancel" << "cancel_id";
    msg4->setActions(actions);

    m->insertNotification(msg3);
    m->insertNotification(msg1);
    m->insertNotification(msg4);
    m->insertNotification(msg2);
    printf("We have %d notifications in our model at the moment.\n", m->numNotifications());

    // Shared pointer problem: http://qt-project.org/wiki/SharedPointersAndQmlOwnership
    view.rootContext()->setContextProperty("notificationmodel", m);
    /* Hardcoded URLs are bad but tolerable here in test code. */
    view.setSource(QUrl::fromLocalFile("../tools/datatest.qml"));
    view.show();
    return app.exec();
}
