#include "notification.hpp"
#include "notificationbackend.hpp"
#include <cassert>
#include <cstdio>

void testSimpleInsertion() {
    Notification *n = new Notification(42, URGENCY_LOW, "this is text");
    NotificationBackend be;

    assert(be.numNotifications() == 0);
    be.insertNotification(n);
    assert(be.numNotifications() == 1);
    be.deleteNotification(n);
    assert(be.numNotifications() == 0);
}

void testOrder() {
    Notification *n1 = new Notification(1, URGENCY_LOW, "low");
    Notification *n2 = new Notification(2, URGENCY_NORMAL, "high");
    Notification *n3 = new Notification(3, URGENCY_CRITICAL, "critical");
    NotificationBackend be;

    be.insertNotification(n1);
    be.insertNotification(n2);
    assert(be.getNotification(0).getID() == n2->getID());
    assert(be.getNotification(1).getID() == n1->getID());

    be.insertNotification(n3);
    assert(be.getNotification(0).getID() == n3->getID());
    assert(be.getNotification(1).getID() == n2->getID());
    assert(be.getNotification(2).getID() == n1->getID());
}

int main(int argc, char **argv) {
#ifdef NDEBUG
    fprintf(stderr, "NDEBUG is defined, tests will not work.");
    return 1;
#else
    testSimpleInsertion();
    testOrder();
    return 0;
#endif
}
