#include <QApplication>
#include <QListView>
#include <QTimer>
#include "notificationmodel.h"

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    QListView *view = new QListView(0);
    NotificationModel *m = new NotificationModel();

    QTimer::singleShot(3000, m, SLOT(testInsert()));
    QTimer::singleShot(6000, m, SLOT(testDelete()));

    view->setModel(m);
    view->show();
    view->setWindowTitle("List viewer test app");
    return app.exec();
}
