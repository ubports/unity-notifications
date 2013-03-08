#include "notification.hpp"
#include "notificationbackend.hpp"
#include "renderer.hpp"

#include <cassert>
#include <cstdio>

void testSimpleInsertion() {
    Renderer r;
    Notification *n = new Notification(42, URGENCY_LOW, "this is text");
    NotificationBackend be(r);

    assert(be.numNotifications() == 0);
    be.insertNotification(n);
    assert(be.numNotifications() == 1);
    be.deleteNotification(n);
    assert(be.numNotifications() == 0);
}

void testOrder() {
    Renderer r;
    Notification *n1 = new Notification(1, URGENCY_LOW, "low");
    Notification *n2 = new Notification(2, URGENCY_NORMAL, "high");
    Notification *n3 = new Notification(3, URGENCY_CRITICAL, "critical");
    NotificationBackend be(r);

    be.insertNotification(n1);
    be.insertNotification(n2);
    assert(be.getNotification(0).getID() == n2->getID());
    assert(be.getNotification(1).getID() == n1->getID());

    be.insertNotification(n3);
    assert(be.getNotification(0).getID() == n3->getID());
    assert(be.getNotification(1).getID() == n2->getID());
    assert(be.getNotification(2).getID() == n1->getID());
}

void testRenderCalls() {
    Renderer r;
    NotificationBackend be(r);
    Notification *n1 = new Notification(1, URGENCY_LOW, "text");
    Notification *n2 = new Notification(2, URGENCY_LOW, "here too");
    Notification *n3 = new Notification(3, URGENCY_LOW, "third");

    assert(r.numChanges() == 0);
    be.insertNotification(n1);
    assert(r.numChanges() == 1);
    be.insertNotification(n2);
    assert(r.numChanges() == 2);
    be.insertNotification(n3);
    assert(r.numChanges() == 3);

    be.deleteNotification(n2);
    assert(r.numChanges() == 4);
    be.clear();
    assert(r.numChanges() == 5);
    assert(be.numNotifications() == 0);
}

void testFullQueue() {
    Renderer r;
    NotificationBackend be(r);
    for(unsigned int i=0; i< MAX_NOTIFICATIONS; i++) {
        Notification *n = new Notification(i, URGENCY_LOW, "text");
        bool result = be.insertNotification(n);
        assert(result);
    }
    Notification *wontFit = new Notification(1111, URGENCY_CRITICAL, "foo");
    bool result = be.insertNotification(wontFit);
    assert(!result);
    assert(be.numNotifications() == MAX_NOTIFICATIONS);
    delete wontFit;

}

int main(int argc, char **argv) {
#ifdef NDEBUG
    fprintf(stderr, "NDEBUG is defined, tests will not work.");
    return 1;
#else
    testSimpleInsertion();
    testOrder();
    testRenderCalls();
    testFullQueue();
    return 0;
#endif
}
