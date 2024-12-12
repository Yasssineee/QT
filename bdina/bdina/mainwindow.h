#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QSerialPort>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDateTime>
#include <QMainWindow>
#include <QSqlQueryModel>
#include "hygiene.h"


namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void mailSent(QString status);
    void box();
private slots:
    void on_pushButton_92_clicked();  // Ensure this is declared here
    void on_pushButton_94_clicked();
    void exportToPDF();
    void rechercherParQuantiteEnStock();
    void on_pushButton_trier_clicked();  // Sort by quantity in stock
    void on_pushButton_trier_2_clicked();  // Sort by quantity in stock

    void on_pushButton_retour_clicked();
    void on_pushButton_retour2_clicked();
    void on_pushButton_retour3_clicked();
    void on_pushButton_retour5_clicked();

    void on_page1Button_2_clicked();
    void on_page2Button_2_clicked();
    void on_page3Button_2_clicked();

    void on_page4Button_2_clicked();
    void on_page5Button_2_clicked();


    void on_readFromArduino();
    void on_checkClient(const QString &clientId);
    void on_pushButton_arduino_clicked();



    void on_tableView25_clicked(const QModelIndex &index);

    void on_pushButton_93_clicked();

    void on_pushButton_statistique_clicked();

    void on_pushButton_clicked();

    void on_pushButton_95_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_barcode_clicked();

    void on_pushButton_generateBarcode_clicked();
private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    QSqlQueryModel *model;
    hygiene Etmp;
    QString m_selectedId;
    void grantAccess(const QString &clientId);
    void denyAccess(const QString &clientId);
    QSqlDatabase db;

};

#endif // MAINWINDOW_H
