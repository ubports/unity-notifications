#include "notification.hpp"

void testNotification() {
    Notification *n = new Notification();
    delete n;
}

int main(int argc, char **argv) {
    testNotification();
}
