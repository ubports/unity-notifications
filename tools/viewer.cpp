#include <QApplication>
#include <QListView>
#include "notificationmodel.hpp"

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    QListView *view = new QListView(0);
    NotificationModel *m = new NotificationModel();

    view->setModel(m);
    view->show();
    view->setWindowTitle("List viewer test app");
    return app.exec();
}
