#include <QApplication>
#include <QListView>
#include <QTimer>
#include "notificationmodel.h"
#include "ui_mainWindow.h"

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    QMainWindow *win = new QMainWindow;
    Ui_MainWindow ui;
    ui.setupUi(win);

/*
    QListView *view = new QListView(0);

    NotificationModel *m = new NotificationModel();

    QTimer::singleShot(3000, m, SLOT(testInsert()));
    QTimer::singleShot(6000, m, SLOT(testDelete()));

    view->setModel(m);
    view->show();
    */
    win->setWindowTitle("List viewer test app");
    win->show();
    return app.exec();
}
