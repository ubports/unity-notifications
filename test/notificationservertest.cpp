
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

int notify(const QString& name) {
        return int(
                notificationsInterface->Notify("my app", 0, "icon " + name,
                                               "summary " + name,
                                               "body " + name, QStringList(),
                                               QVariantMap(), 0));
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
