#include "mainwindow.h"
#include "connection.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    connection conn;
    if (!conn.createconnect()) {
        return -1;
    }

    MainWindow w;
    w.show();

    return app.exec();
}
