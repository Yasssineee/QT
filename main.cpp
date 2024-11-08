#include <QApplication>
#include <QMessageBox>
#include "mainwindow.h"
#include "connection.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);  // Create the application object

    Connection c;  // Create an instance of the Connection class
    if (!c.createconnect()) {  // Attempt to create a database connection
        // If the connection failed, show an error message
        QMessageBox::critical(nullptr, "Database Connection Error",
                              "Failed to connect to the database:\n" + QSqlDatabase::drivers().join(", "));
        return -1;  // Exit the application
    }

    MainWindow w;  // Create an instance of your MainWindow
    w.show();      // Show the main window

    return a.exec();  // Enter the main event loop
}
