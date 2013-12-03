#include "Notification.h"
#include "NotificationModel.h"

#include<QApplication>
#include <cassert>
#include <cstdio>

void testSimpleInsertion() {
    int timeout = 5000;
    QSharedPointer<Notification> n(new Notification(42, timeout, Notification::Low, "this is text"));
    NotificationModel m;

    assert(m.numNotifications() == 1);
    m.insertNotification(n);
    assert(m.numNotifications() == 2);
    m.removeNotification(n->getID());
    assert(m.numNotifications() == 1);
}

void testTypeSimple() {
    int timeout = 5000;
    QSharedPointer<Notification> n1(new Notification(1, timeout, Notification::Low, "low", Notification::Ephemeral));
    QSharedPointer<Notification> n2(new Notification(2, timeout, Notification::Low, "low", Notification::SnapDecision));
    QSharedPointer<Notification> n3(new Notification(3, timeout, Notification::Low, "low", Notification::Confirmation));
    NotificationModel m;

    m.insertNotification(n1);
    assert(m.showingNotificationOfType(Notification::Ephemeral));
    m.insertNotification(n2);
    assert(m.showingNotificationOfType(Notification::SnapDecision));

    m.insertNotification(n3);
    assert(m.showingNotificationOfType(Notification::Confirmation));
}

void testOrder() {
    const int timeout = 5000;
    QSharedPointer<Notification> n1(new Notification(1, timeout, Notification::Low, "low", Notification::Ephemeral));
    QSharedPointer<Notification> n2(new Notification(2, timeout, Notification::Normal, "high", Notification::Ephemeral));
    QSharedPointer<Notification> n3(new Notification(3, timeout, Notification::Critical, "critical", Notification::Ephemeral));
    NotificationModel m;

    m.insertNotification(n1);
    assert(m.showingNotification(n1->getID()));

    m.insertNotification(n2);
    assert(!m.showingNotification(n1->getID()));
    assert(m.showingNotification(n2->getID()));
    assert(m.queued() == 1);

    m.insertNotification(n3);
    assert(!m.showingNotification(n1->getID()));
    assert(!m.showingNotification(n2->getID()));
    assert(m.showingNotification(n3->getID()));
    assert(m.queued() == 2);

    m.removeNotification(n3->getID());
    assert(!m.showingNotification(n1->getID()));
    assert(m.showingNotification(n2->getID()));
    assert(!m.showingNotification(n3->getID()));
    assert(m.queued() == 1);

    m.removeNotification(n2->getID());
    assert(m.showingNotification(n1->getID()));
    assert(!m.showingNotification(n2->getID()));
    assert(!m.showingNotification(n3->getID()));
    assert(m.queued() == 0);

    m.removeNotification(n1->getID());
    assert(!m.showingNotification(n1->getID()));
    assert(!m.showingNotification(n2->getID()));
    assert(!m.showingNotification(n3->getID()));
    assert(m.queued() == 0);
}

void testHas() {
    const int timeout = 5000;
    QSharedPointer<Notification> n1(new Notification(1, timeout, Notification::Low, "low", Notification::Ephemeral));
    QSharedPointer<Notification> n2(new Notification(2, timeout, Notification::Low, "low", Notification::Ephemeral));
    QSharedPointer<Notification> n3(new Notification(3, timeout, Notification::Low, "low", Notification::Ephemeral));
    NotificationModel m;

    assert(!m.hasNotification(1));
    assert(!m.hasNotification(2));
    assert(!m.hasNotification(3));

    m.insertNotification(n1);
    assert(m.hasNotification(1));
    assert(!m.hasNotification(2));
    assert(!m.hasNotification(3));

    m.insertNotification(n2);
    assert(m.hasNotification(1));
    assert(m.hasNotification(2));
    assert(!m.hasNotification(3));

    m.insertNotification(n3);
    assert(m.hasNotification(1));
    assert(m.hasNotification(2));
    assert(m.hasNotification(3));
}


void testFullQueue() {
    int timeout = 5000;
    NotificationModel m;
    for(unsigned int i=0; i< MAX_NOTIFICATIONS; i++) {
        QSharedPointer<Notification> n(new Notification(i, timeout, Notification::Low, "text", Notification::Ephemeral));
        m.insertNotification(n);
    }
    assert((unsigned int)m.numNotifications() == MAX_NOTIFICATIONS);
    QSharedPointer<Notification> wontFit(new Notification(1111, timeout, Notification::Critical, "foo"));
    m.insertNotification(wontFit);
    assert((unsigned int)m.numNotifications() == MAX_NOTIFICATIONS);
}

void testVisualSDQueueMax() {
    const int timeout = 60000;
    NotificationModel m;

    for(unsigned int i = 0; i < NotificationModel::maxSnapsShown + 1; i++) {
        QSharedPointer<Notification> n(new Notification(i, timeout, Notification::Low, "snap-decision", Notification::SnapDecision));
        m.insertNotification(n);
    }

    for(unsigned int i = 0; i < NotificationModel::maxSnapsShown; i++) {
        assert(m.showingNotification(i));
    }

    assert(!m.showingNotification(NotificationModel::maxSnapsShown+1));
}

void testVisualSDQueueWithCritical() {
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

    assert(!strcmp(m.getDisplayedNotification(0)->getBody().toStdString().c_str(), "snap-decision-critical"));
}

void testVisualSDQueueWithoutCritical() {
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

    assert(!strcmp(m.getDisplayedNotification(3)->getBody().toStdString().c_str(), "snap-decision-1"));
    assert(!strcmp(m.getDisplayedNotification(2)->getBody().toStdString().c_str(), "snap-decision-2"));
    assert(!strcmp(m.getDisplayedNotification(1)->getBody().toStdString().c_str(), "snap-decision-3"));
    assert(!strcmp(m.getDisplayedNotification(0)->getBody().toStdString().c_str(), "snap-decision-4"));
}

int main(int argc, char **argv) {
    QApplication app(argc, argv);
#ifdef NDEBUG
    fprintf(stderr, "NDEBUG is defined, tests will not work.");
    return 1;
#else
    testSimpleInsertion();
    testTypeSimple();
    testFullQueue();
    testOrder();
    testHas();
    testVisualSDQueueMax();
    testVisualSDQueueWithCritical();
    testVisualSDQueueWithoutCritical();
    return 0;
#endif
}
