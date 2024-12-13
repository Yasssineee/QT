#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QTableView>
#include <QSqlQueryModel>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QSettings>
#include <QTranslator>
#include <QApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include "alimentaire.h" // Include Alimentaire class
#include <QtCharts>
#include <QChartView>
#include <QBarSet>
#include <QBarSeries>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_279_clicked(); // Add button
    void on_pushButton_281_clicked(); // Delete button
    void on_pushButton_280_clicked(); // Update button
    void on_pushButton_278_clicked(); // Reset button
    void on_pushButton_282_clicked(); // Generate PDF
    void on_pushButton_287_clicked(); // Generate QR Code
    void on_pushButton_286_clicked();
    void on_pushButton_305_clicked(); // Sort by quantity difference
    void on_pushButton_284_clicked(); // Recycle Bin
    void onTableViewSelectionChanged();
    void showQRCode(const QString& data);
    QString generateProductQRData(int code, const QString& prd, int qtt, int qttmin);
    void on_pushButton_56_clicked();
    bool moveToRecycleBin(const Alimentaire& item);
    void displayRecycleBin();
    bool restoreFromRecycleBin(int code);
    void emptyRecycleBin();
    void on_pushButton_clicked();  // Add this line for the restock list button
    void on_pushButton_2_clicked();  // Add help assistant button handler
    void on_pushButton_3_clicked();

private:
    void updateStockChart();
    Ui::MainWindow *ui;
    Alimentaire currentAlimentaire; // Declare currentAlimentaire
    void displayTable(); // Declare displayTable function
    bool showingRecycleBin;
    void createRecycleBinTable(); // Create recycle bin table if not exists
    void sortTable(); // Add sorting function declaration
    void checkLowStock();  // Add this function to check for low stock
    void loadLanguage(const QString& language);
    QTranslator* translator;
};

#endif // MAINWINDOW_H
