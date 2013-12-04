#include "Notification.h"
#include "NotificationModel.h"

#include <QtTest/QtTest>

class TestNotifications: public QObject
{
    Q_OBJECT

    private Q_SLOTS:
        void testFullQueue();
        void testHas();
        void testOrder();
        void testTypeSimple();
        void testSimpleInsertion();
        void testVisualSDQueueWithoutCritical();
        void testVisualSDQueueWithCritical();
        void testVisualSDQueueMax();
};

void TestNotifications::testSimpleInsertion() {
    int timeout = 5000;
    QSharedPointer<Notification> n(new Notification(42, timeout, Notification::Low, "this is text"));
    NotificationModel m;

    QVERIFY(m.numNotifications() == 1);
    m.insertNotification(n);
    QVERIFY(m.numNotifications() == 2);
    m.removeNotification(n->getID());
    QVERIFY(m.numNotifications() == 1);
}

void TestNotifications::testTypeSimple() {
    int timeout = 5000;
    QSharedPointer<Notification> n1(new Notification(1, timeout, Notification::Low, "low", Notification::Ephemeral));
    QSharedPointer<Notification> n2(new Notification(2, timeout, Notification::Low, "low", Notification::SnapDecision));
    QSharedPointer<Notification> n3(new Notification(3, timeout, Notification::Low, "low", Notification::Confirmation));
    NotificationModel m;

    m.insertNotification(n1);
    QVERIFY(m.showingNotificationOfType(Notification::Ephemeral));
    m.insertNotification(n2);
    QVERIFY(m.showingNotificationOfType(Notification::SnapDecision));

    m.insertNotification(n3);
    QVERIFY(m.showingNotificationOfType(Notification::Confirmation));
}

void TestNotifications::testOrder() {
    const int timeout = 5000;
    QSharedPointer<Notification> n1(new Notification(1, timeout, Notification::Low, "low", Notification::Ephemeral));
    QSharedPointer<Notification> n2(new Notification(2, timeout, Notification::Normal, "high", Notification::Ephemeral));
    QSharedPointer<Notification> n3(new Notification(3, timeout, Notification::Critical, "critical", Notification::Ephemeral));
    NotificationModel m;

    m.insertNotification(n1);
    QVERIFY(m.showingNotification(n1->getID()));

    m.insertNotification(n2);
    QVERIFY(!m.showingNotification(n1->getID()));
    QVERIFY(m.showingNotification(n2->getID()));
    QVERIFY(m.queued() == 1);

    m.insertNotification(n3);
    QVERIFY(!m.showingNotification(n1->getID()));
    QVERIFY(!m.showingNotification(n2->getID()));
    QVERIFY(m.showingNotification(n3->getID()));
    QVERIFY(m.queued() == 2);

    m.removeNotification(n3->getID());
    QVERIFY(!m.showingNotification(n1->getID()));
    QVERIFY(m.showingNotification(n2->getID()));
    QVERIFY(!m.showingNotification(n3->getID()));
    QVERIFY(m.queued() == 1);

    m.removeNotification(n2->getID());
    QVERIFY(m.showingNotification(n1->getID()));
    QVERIFY(!m.showingNotification(n2->getID()));
    QVERIFY(!m.showingNotification(n3->getID()));
    QVERIFY(m.queued() == 0);

    m.removeNotification(n1->getID());
    QVERIFY(!m.showingNotification(n1->getID()));
    QVERIFY(!m.showingNotification(n2->getID()));
    QVERIFY(!m.showingNotification(n3->getID()));
    QVERIFY(m.queued() == 0);
}

void TestNotifications::testHas() {
    const int timeout = 5000;
    QSharedPointer<Notification> n1(new Notification(1, timeout, Notification::Low, "low", Notification::Ephemeral));
    QSharedPointer<Notification> n2(new Notification(2, timeout, Notification::Low, "low", Notification::Ephemeral));
    QSharedPointer<Notification> n3(new Notification(3, timeout, Notification::Low, "low", Notification::Ephemeral));
    NotificationModel m;

    QVERIFY(!m.hasNotification(1));
    QVERIFY(!m.hasNotification(2));
    QVERIFY(!m.hasNotification(3));

    m.insertNotification(n1);
    QVERIFY(m.hasNotification(1));
    QVERIFY(!m.hasNotification(2));
    QVERIFY(!m.hasNotification(3));

    m.insertNotification(n2);
    QVERIFY(m.hasNotification(1));
    QVERIFY(m.hasNotification(2));
    QVERIFY(!m.hasNotification(3));

    m.insertNotification(n3);
    QVERIFY(m.hasNotification(1));
    QVERIFY(m.hasNotification(2));
    QVERIFY(m.hasNotification(3));
}

void TestNotifications::testFullQueue() {
    int timeout = 5000;
    NotificationModel m;
    for(unsigned int i=0; i< MAX_NOTIFICATIONS; i++) {
        QSharedPointer<Notification> n(new Notification(i, timeout, Notification::Low, "text", Notification::Ephemeral));
        m.insertNotification(n);
    }
    QVERIFY((unsigned int)m.numNotifications() == MAX_NOTIFICATIONS);
    QSharedPointer<Notification> wontFit(new Notification(1111, timeout, Notification::Critical, "foo"));
    m.insertNotification(wontFit);
    QVERIFY((unsigned int)m.numNotifications() == MAX_NOTIFICATIONS);
}

void TestNotifications::testVisualSDQueueMax() {
    const int timeout = 60000;
    NotificationModel m;

    for(unsigned int i = 0; i < NotificationModel::maxSnapsShown + 1; i++) {
        QSharedPointer<Notification> n(new Notification(i, timeout, Notification::Low, "snap-decision", Notification::SnapDecision));
        m.insertNotification(n);
    }

    for(unsigned int i = 0; i < NotificationModel::maxSnapsShown; i++) {
        QVERIFY(m.showingNotification(i));
    }

    QVERIFY(!m.showingNotification(NotificationModel::maxSnapsShown+1));
}

void TestNotifications::testVisualSDQueueWithCritical() {
    const int timeout = 60000;
    NotificationModel m;

    QSharedPointer<Notification> n1(new Notification(1, timeout, Notification::Low, "snap-decision", Notification::SnapDecision));
    QSharedPointer<Notification> n2(new Notification(2, timeout, Notification::Low, "snap-decision", Notification::SnapDecision));
    QSharedPointer<Notification> n3(new Notification(3, timeout, Notification::Critical, "snap-decision-critical", Notification::SnapDecision));
    QSharedPointer<Notification> n4(new Notification(4, timeout, Notification::Low, "snap-decision", Notification::SnapDecision));

    m.insertNotification(n1);
    m.insertNotification(n2);
    m.insertNotification(n3);
    m.insertNotification(n4);

    QVERIFY(!strcmp(m.getDisplayedNotification(0)->getBody().toStdString().c_str(), "snap-decision-critical"));
}

void TestNotifications::testVisualSDQueueWithoutCritical() {
    const int timeout = 60000;
    NotificationModel m;

    QSharedPointer<Notification> n1(new Notification(1, timeout, Notification::Low, "snap-decision-1", Notification::SnapDecision));
    QSharedPointer<Notification> n2(new Notification(2, timeout, Notification::Low, "snap-decision-2", Notification::SnapDecision));
    QSharedPointer<Notification> n3(new Notification(3, timeout, Notification::Low, "snap-decision-3", Notification::SnapDecision));
    QSharedPointer<Notification> n4(new Notification(4, timeout, Notification::Low, "snap-decision-4", Notification::SnapDecision));

    m.insertNotification(n1);
    m.insertNotification(n2);
    m.insertNotification(n3);
    m.insertNotification(n4);

    QVERIFY(!strcmp(m.getDisplayedNotification(3)->getBody().toStdString().c_str(), "snap-decision-1"));
    QVERIFY(!strcmp(m.getDisplayedNotification(2)->getBody().toStdString().c_str(), "snap-decision-2"));
    QVERIFY(!strcmp(m.getDisplayedNotification(1)->getBody().toStdString().c_str(), "snap-decision-3"));
    QVERIFY(!strcmp(m.getDisplayedNotification(0)->getBody().toStdString().c_str(), "snap-decision-4"));
}

QTEST_MAIN(TestNotifications)
#include "notificationtest.moc"
