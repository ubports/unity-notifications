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
        void testTextFilter();
        void testTextFilter_data();
};

void TestNotifications::testSimpleInsertion() {
    int timeout = 5000;
    QSharedPointer<Notification> n(new Notification(42, timeout, Notification::Low, "this is text"));
    NotificationModel m;

    QCOMPARE(m.numNotifications(), 1);
    m.insertNotification(n);
    QCOMPARE(m.numNotifications(), 2);
    m.removeNotification(n->getID());
    QCOMPARE(m.numNotifications(), 1);
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
    QCOMPARE(m.queued(), 2);

    m.removeNotification(n3->getID());
    QVERIFY(!m.showingNotification(n1->getID()));
    QVERIFY(m.showingNotification(n2->getID()));
    QVERIFY(!m.showingNotification(n3->getID()));
    QCOMPARE(m.queued(), 1);

    m.removeNotification(n2->getID());
    QVERIFY(m.showingNotification(n1->getID()));
    QVERIFY(!m.showingNotification(n2->getID()));
    QVERIFY(!m.showingNotification(n3->getID()));
    QCOMPARE(m.queued(), 0);

    m.removeNotification(n1->getID());
    QVERIFY(!m.showingNotification(n1->getID()));
    QVERIFY(!m.showingNotification(n2->getID()));
    QVERIFY(!m.showingNotification(n3->getID()));
    QCOMPARE(m.queued(), 0);
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
    QCOMPARE((unsigned int)m.numNotifications(), MAX_NOTIFICATIONS);
    QSharedPointer<Notification> wontFit(new Notification(1111, timeout, Notification::Critical, "foo"));
    m.insertNotification(wontFit);
    QCOMPARE((unsigned int)m.numNotifications(), MAX_NOTIFICATIONS);
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

    QCOMPARE(m.getDisplayedNotification(1)->getBody(), QString("snap-decision-critical"));
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

    QCOMPARE(m.getDisplayedNotification(4)->getBody(), QString("snap-decision-1"));
    QCOMPARE(m.getDisplayedNotification(3)->getBody(), QString("snap-decision-2"));
    QCOMPARE(m.getDisplayedNotification(2)->getBody(), QString("snap-decision-3"));
    QCOMPARE(m.getDisplayedNotification(1)->getBody(), QString("snap-decision-4"));
}

void TestNotifications::testTextFilter_data() {
    QTest::addColumn<QString>("string");
    QTest::addColumn<QString>("result");

    QTest::newRow("") << "<a href=\"http://www.ubuntu.com/\">Ubuntu</a>" << "Ubuntu";
    QTest::newRow("") << "Don't rock the boat" <<                           "Don't rock the boat";
    QTest::newRow("") << "<i>Not italic</i>" <<                             "Not italic";
    QTest::newRow("") << "<u>Test</u>" <<                                   "Test";
    QTest::newRow("") << "<b>Bold</b>" <<                                   "Bold";
    QTest::newRow("") << "<span>Span</span>" <<                             "Span";
    QTest::newRow("") << "<s>E-flat</s>" <<                                 "E-flat";
    QTest::newRow("") << "<sub>Sandwich</sub>" <<                           "Sandwich";
    QTest::newRow("") << "<small>Fry</small>" <<                            "Fry";
    QTest::newRow("") << "<tt>Testing tag</tt>" <<                          "Testing tag";
    QTest::newRow("") << "<html>Surrounded by html</html>" <<               "Surrounded by html";
    QTest::newRow("") << "<qt>Surrounded by qt</qt>" <<                     "Surrounded by qt";
    QTest::newRow("") << "\"Film spectators are quiet vampires.\"" <<       "\"Film spectators are quiet vampires.\"";
    QTest::newRow("") << "7 > 3" <<                                         "7 > 3";
    QTest::newRow("") << "7 &gt; 3" <<                                      "7 > 3";
    QTest::newRow("") << "7 &#62; 3" <<                                     "7 > 3";
    QTest::newRow("") << "7 &#x3e; 3" <<                                    "7 > 3";
    QTest::newRow("") << "14 &lt; 42" <<                                    "14 < 42";
    QTest::newRow("") << "14 &#60; 42" <<                                   "14 < 42";
    QTest::newRow("") << "14 &#x3c; 42" <<                                  "14 < 42";
    QTest::newRow("") << "War & Peace" <<                                   "War & Peace";
    QTest::newRow("") << "Law &#38; Order" <<                               "Law & Order";
    QTest::newRow("") << "Love &#x26; War" <<                               "Love & War";
    QTest::newRow("") << "Kick him while he&apos;s down" <<                 "Kick him while he's down";
    QTest::newRow("") << "Peace &amp; Love" <<                              "Peace & Love";
    QTest::newRow("") << "<b>So broken</i>" <<                              "So broken";
}

void TestNotifications::testTextFilter() {
    const int timeout = 1000;

    Notification n(new Notification(1, timeout, Notification::Low, "notification", Notification::Ephemeral));

    QFETCH(QString, string);
    QFETCH(QString, result);
    n.setSummary(string);
    QCOMPARE(n.getSummary(), result);
    n.setBody(string);
    QCOMPARE(n.getBody(), result);
}

QTEST_MAIN(TestNotifications)
#include "notificationtest.moc"
