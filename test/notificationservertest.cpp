
#include <notify-backend.h>
#include <NotificationsInterface.h>

#include <libqtdbustest/DBusTestRunner.h>
#include <libqtdbustest/QProcessDBusService.h>
#include <QtTest/QtTest>

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

void testNotify() {
    uint id = notificationsInterface->Notify("my app", 0, "icon", "summary", "body", QStringList(), QVariantMap(), 0);
    QCOMPARE(id, uint(1));

    uint id2 = notificationsInterface->Notify("my app", 0, "icon 2", "summary 2", "body 2", QStringList(), QVariantMap(), 0);
    QCOMPARE(id2, uint(2));

    NotificationDataList notifications = notificationsInterface->GetNotifications("my app");
    QCOMPARE(notifications.size(), 2);

    NotificationData notification = notifications[0];

    QCOMPARE(
        notifications[0],
        NotificationData()
            .setAppName("my app")
            .setId(id)
            .setAppIcon("image://theme/icon")
            .setBody("body")
            .setSummary("summary")
            .setExpireTimeout(5000)
        );

    QCOMPARE(
        notifications[1],
        NotificationData()
            .setAppName("my app")
            .setId(id2)
            .setAppIcon("image://theme/icon 2")
            .setBody("body 2")
            .setSummary("summary 2")
            .setExpireTimeout(5000)
        );
}

void testClose() {
    uint id = notificationsInterface->Notify("my app", 0, "icon", "summary", "body", QStringList(), QVariantMap(), 0);
    QCOMPARE(id, uint(1));

    uint id2 = notificationsInterface->Notify("my app", 0, "icon 2", "summary 2", "body 2", QStringList(), QVariantMap(), 0);
    QCOMPARE(id2, uint(2));

    {
        NotificationDataList notifications = notificationsInterface->GetNotifications("my app");
        QCOMPARE(notifications.size(), 2);

        QCOMPARE(
            notifications[0],
            NotificationData()
                .setAppName("my app")
                .setId(id)
                .setAppIcon("image://theme/icon")
                .setBody("body")
                .setSummary("summary")
                .setExpireTimeout(5000)
            );

        QCOMPARE(
            notifications[1],
            NotificationData()
                .setAppName("my app")
                .setId(id2)
                .setAppIcon("image://theme/icon 2")
                .setBody("body 2")
                .setSummary("summary 2")
                .setExpireTimeout(5000)
            );
    }

    notificationsInterface->CloseNotification(id).waitForFinished();

    {
        NotificationDataList notifications = notificationsInterface->GetNotifications("my app");
        QCOMPARE(notifications.size(), 1);

        QCOMPARE(
            notifications[0],
            NotificationData()
                .setAppName("my app")
                .setId(id2)
                .setAppIcon("image://theme/icon 2")
                .setBody("body 2")
                .setSummary("summary 2")
                .setExpireTimeout(5000)
        );
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
