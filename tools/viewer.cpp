#include <QApplication>
#include <QListView>
#include <QStringListModel>

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    QListView *view = new QListView(0);
    QStringList list;
    list << "one" << "two" << "three";
    QStringListModel *m = new QStringListModel();
    m->setStringList(list);
    view->setModel(m);
    view->show();
    view->setWindowTitle("List viewer test app");
    return app.exec();
}
