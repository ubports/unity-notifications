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

int main(int argc, char **argv) {
#ifdef NDEBUG
    fprintf(stderr, "NDEBUG is defined, tests will not work.");
    return 1;
#else
    testSimpleInsertion();
    return 0;
#endif
}
