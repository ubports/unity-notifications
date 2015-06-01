
#include <notify-backend.h>
#include <NotificationsInterface.h>

#include <libqtdbustest/DBusTestRunner.h>
#include <libqtdbustest/QProcessDBusService.h>
#include <QtTest/QtTest>
#include <stdexcept>

using namespace QtDBusTest;

class TestNotificationServer: public QObject {
Q_OBJECT

    QSharedPointer<DBusTestRunner> dbus;

    QSharedPointer<OrgFreedesktopNotificationsInterface> notificationsInterface;

private Q_SLOTS:

void init() {
    DBusTypes::registerNotificationMetaTypes();

    dbus.reset(new DBusTestRunner);

    dbus->registerService(
            DBusServicePtr(
                    new QProcessDBusService(DBUS_SERVICE_NAME,
                                            QDBusConnection::SessionBus,
                                            DBUS_SERVER_BINARY,
                                            QStringList{"--no-gui"})));
    dbus->startServices();

    notificationsInterface.reset(
                new OrgFreedesktopNotificationsInterface(DBUS_SERVICE_NAME,
                                                         DBUS_PATH,
                                                         dbus->sessionConnection()));
}

void cleanup() {
    notificationsInterface.reset();

    dbus.reset();
}

int notify(const QString& name, int replacesId = 0) {
    auto reply = notificationsInterface->Notify("my app", replacesId, "icon " + name,
                                                   "summary " + name,
                                                   "body " + name, QStringList(),
                                                   QVariantMap(), 0);
    reply.waitForFinished();
    if (reply.isError()) {
        throw std::domain_error(reply.error().message().toStdString());
    }
    return reply;
}

void expectNotification(const NotificationDataList& notifications, int index, int id, const QString& name) {
    QCOMPARE(
        notifications.at(index),
        NotificationData()
            .setAppName("my app")
            .setId(id)
            .setAppIcon("image://theme/icon " + name)
            .setBody("body " + name)
            .setSummary("summary " + name)
            .setExpireTimeout(5000)
        );
}

void testNotify() {
    uint id1 = notify("1");
    QCOMPARE(id1, uint(1));

    uint id2 = notify("2");
    QCOMPARE(id2, uint(2));

    NotificationDataList notifications = notificationsInterface->GetNotifications("my app");
    QCOMPARE(notifications.size(), 2);

    expectNotification(notifications, 0, id1, "1");
    expectNotification(notifications, 1, id2, "2");
}

void testClose() {
    uint id1 = notify("1");
    QCOMPARE(id1, uint(1));

    uint id2 = notify("2");
    QCOMPARE(id2, uint(2));

    {
        NotificationDataList notifications = notificationsInterface->GetNotifications("my app");
        QCOMPARE(notifications.size(), 2);

        expectNotification(notifications, 0, id1, "1");
        expectNotification(notifications, 1, id2, "2");
    }

    notificationsInterface->CloseNotification(id1).waitForFinished();

    {
        NotificationDataList notifications = notificationsInterface->GetNotifications("my app");
        QCOMPARE(notifications.size(), 1);

        expectNotification(notifications, 0, id2, "2");
    }
}

void testNotifyCloseNotify() {
    uint id1 = notify("1");
    QCOMPARE(id1, uint(1));

    {
        NotificationDataList notifications = notificationsInterface->GetNotifications("my app");
        QCOMPARE(notifications.size(), 1);

        expectNotification(notifications, 0, id1, "1");
    }

    notificationsInterface->CloseNotification(id1).waitForFinished();

    {
        NotificationDataList notifications = notificationsInterface->GetNotifications("my app");
        QCOMPARE(notifications.size(), 0);
    }

    id1 = notify("1", id1);
    QCOMPARE(id1, uint(1));

    {
        NotificationDataList notifications = notificationsInterface->GetNotifications("my app");
        QCOMPARE(notifications.size(), 1);

        expectNotification(notifications, 0, id1, "1");
    }
}

void testNotifyCounterIncrement() {
    uint id1 = notify("1", 1);
    QCOMPARE(id1, uint(1));

    uint id2 = notify("2", 2);
    QCOMPARE(id2, uint(2));

    uint id4 = notify("4", 4);
    QCOMPARE(id4, uint(4));

    uint id5 = notify("5", 5);
    QCOMPARE(id5, uint(5));

    uint id3 = notify("3");
    QCOMPARE(id3, uint(3));

    uint id6 = notify("6");
    QCOMPARE(id6, uint(6));

    {
        NotificationDataList notifications = notificationsInterface->GetNotifications("my app");
        QCOMPARE(notifications.size(), 6);

        expectNotification(notifications, 0, id1, "1");
        expectNotification(notifications, 1, id2, "2");
        expectNotification(notifications, 2, id3, "3");
        expectNotification(notifications, 3, id4, "4");
        expectNotification(notifications, 4, id5, "5");
        expectNotification(notifications, 5, id6, "6");
    }
}

void testGetCapabilities() {
    QStringList capabilities = notificationsInterface->GetCapabilities();
    QStringList expected {"actions",
        "body",
        "body-markup",
        "icon-static",
        "image/svg+xml",
        VALUE_HINT,
        VALUE_TINT_HINT,
        URGENCY_HINT,
        SOUND_HINT,
        SUPPRESS_SOUND_HINT,
        SYNCH_HINT,
        ICON_ONLY_HINT,
        AFFIRMATIVE_TINT_HINT,
        REJECTION_TINT_HINT,
        TRUNCATION_HINT,
        SNAP_HINT,
        SECONDARY_ICON_HINT,
        NON_SHAPED_ICON_HINT,
        MENU_MODEL_HINT,
        INTERACTIVE_HINT,
        TIMEOUT_HINT,
        SWIPE_HINT
    };
    QCOMPARE(capabilities, expected);
}

void testGetServerInformation() {
    QString vendor, version, specVersion;
    QString name = notificationsInterface->GetServerInformation(vendor, version, specVersion);

    QCOMPARE(name, QString("Unity notification server"));
    QCOMPARE(vendor, QString("Canonical Ltd"));
    QCOMPARE(version, QString("1.2"));
    QCOMPARE(specVersion, QString("1.1"));
}

};

QTEST_MAIN(TestNotificationServer)
#include "notificationservertest.moc"
