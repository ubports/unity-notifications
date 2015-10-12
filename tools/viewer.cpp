#include <QApplication>
#include <QListView>
#include <QTimer>
#include "NotificationModel.h"
#include "mainWindow.h"

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    MainWindow *win = new MainWindow();
    win->setWindowTitle("List viewer test app");
    win->show();
    return app.exec();
}
