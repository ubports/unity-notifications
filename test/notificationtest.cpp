#include "notification.hpp"

void testNotification() {
    Notification *n = new Notification(URGENCY_LOW, "this is text");
    delete n;
}

int main(int argc, char **argv) {
    testNotification();
}
