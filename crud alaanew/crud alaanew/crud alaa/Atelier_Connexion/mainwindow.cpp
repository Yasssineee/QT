#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chambre.h" // Include the client header
#include "connection.h"
#include <QDebug>
#include "BackgroundColorDelegate.h"
#include <QPrinter>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QFileDialog>
#include <QTextStream>
#include <QPdfWriter>
#include <QAxObject>
#include <QChart>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QChartView>
#include <QVBoxLayout>

using namespace QtCharts;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    QStackedWidget* stackedWidget = ui->stackedWidget;
    QWidget* page = stackedWidget->widget(1);
    QTableView* tableView = page->findChild<QTableView*>("tableView");

    if (tableView) {

        chambre chambre;
        Connection c;

        if (c.createconnect()) {
            QMessageBox::information(nullptr, QObject::tr("Database is open"),
                                     QObject::tr("Connection successful.\nClick Cancel to exit."),
                                     QMessageBox::Cancel);

            tableView->setModel(chambre.Afficherchambres());
            tableView->resizeColumnsToContents();
            tableView->horizontalHeader()->setStretchLastSection(true);




            QPalette palette;
            palette.setColor(QPalette::Background,QColor("#00FF9C"));


            tableView->setItemDelegateForColumn(7, new BackgroundColorDelegate(palette));


            QPalette palette2;
            palette2.setColor(QPalette::Background,QColor("#FF4545"));


            tableView->setItemDelegateForColumn(8, new BackgroundColorDelegate(palette2));






        } else {
            QMessageBox::critical(nullptr, QObject::tr("Database is not open"),
                                  QObject::tr("Connection failed.\nClick Cancel to exit."),
                                  QMessageBox::Cancel);
        }
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Error"),
                              QObject::tr("Failed to find the table view for chambre data."),
                              QMessageBox::Cancel);
    }


    connect(ui->search_bar, &QLineEdit::textChanged, this, &MainWindow::Rechercherchambre);

    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_sortComboBox_currentIndexChanged(int)));

    connect(ui->pdf, &QPushButton::clicked, this, &MainWindow::exportToPDF);

    connect(ui->gostat, &QPushButton::clicked, this, [this]() { navigateToPage(2); });
    connect(ui->backto, &QPushButton::clicked, this, [this]() { navigateToPage(1); });
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_ajouter_clicked()
{
    QString type_c = ui->comboBox_1->currentText(); // Using QComboBox for type_c
    QString prixStr = ui->prix->text(); // Assuming prix_c is a QLineEdit
    double prix_c = prixStr.toDouble();
    QString vue = ui->comboBox_2->currentText(); // Using QComboBox for vue

    // Checking etat_dis using QRadioButton
    QString etat_dis;
    if (ui->radioButton_1->isChecked()) {
        etat_dis = "Available";
    } else if (ui->radioButton_2->isChecked()) {
        etat_dis = "Unavailable";
    }

    QString capaciteStr = ui->capacite->text(); // Assuming capacite_m is a QLineEdit
    int capacite_m = capaciteStr.toInt();

    QString nbrLitsStr = ui->nbrlits->text(); // Assuming nbrlits is a QLineEdit
    int nbrlits = nbrLitsStr.toInt();

    // Input validation
    QRegExp nameRx("^[a-zA-Z]+$"); // Only letters for string fields
    QRegExp numberRx("^\\d+(\\.\\d+)?$"); // Only digits and optional decimal point for numbers

    if (!numberRx.exactMatch(prixStr) || !numberRx.exactMatch(capaciteStr) || !numberRx.exactMatch(nbrLitsStr)) {
        QMessageBox::critical(this, tr("Erreur"), tr("Prix, capacité, et nombre de lits doivent être des nombres valides."), QMessageBox::Cancel);
    } else if (etat_dis.isEmpty()) {
        QMessageBox::critical(this, tr("Erreur"), tr("Veuillez sélectionner l'état de disponibilité."), QMessageBox::Cancel);
    } else {
        // Create chambre object
        chambre chambre(0, type_c, prix_c, etat_dis, vue, capacite_m, nbrlits);

        // Attempt to add chambre
        if (chambre.Ajouterchambre()) {
            // Update the table view if addition was successful
            ui->tableView->setModel(chambre.Afficherchambres());
            QMessageBox::information(this, tr("Succès"), tr("Ajout effectué."), QMessageBox::Cancel);

            // Clear input fields after addition
            ui->comboBox_1->setCurrentIndex(-1); // Clear combobox selection
            ui->prix->clear();
            ui->radioButton_1->setAutoExclusive(false);
            ui->radioButton_2->setAutoExclusive(false);
            ui->radioButton_1->setChecked(false);
            ui->radioButton_2->setChecked(false);
            ui->radioButton_1->setAutoExclusive(true);
            ui->radioButton_2->setAutoExclusive(true);
            ui->comboBox_2->setCurrentIndex(-1); // Clear combobox selection
            ui->capacite->clear();
            ui->nbrlits->clear();
        } else {
            QMessageBox::critical(this, tr("Erreur"), tr("Erreur lors de l'ajout de la chambre."), QMessageBox::Cancel);
        }
    }
}


void MainWindow::on_tableView_activated(const QModelIndex &index)
{
    if (!index.isValid()) {
        qDebug() << "Invalid index";
        return;
    }
    QString value = ui->tableView->model()->data(ui->tableView->model()->index(index.row(), 0)).toString();

    Connection c;
    c.closeconnection();
    QSqlQuery qry;

    qry.prepare("SELECT TYPE_C, PRIX_C, ETAT_DIS, VUE, CAPACITE_M, NBRLITS FROM chambre WHERE ID_C = :ID_C");
    qry.bindValue(":ID_C", value);

    if(qry.exec())
    {
        if(qry.next())
        {
            ui->id->setText(value);
            if (index.column() == 7) {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, "Update Chambre", "Are you sure you want to update this chambre?",
                                              QMessageBox::Yes|QMessageBox::No);
                if (reply == QMessageBox::Yes) {
                    ui->comboBox_1->setCurrentText(qry.value(0).toString());
                    ui->prix->setText(qry.value(1).toString());
                    QString etat = qry.value(2).toString();
                    if (etat == "Available") {
                        ui->radioButton_1->setChecked(true);
                    } else if (etat == "Unavailable") {
                        ui->radioButton_2->setChecked(true);
                    }
                    ui->comboBox_2->setCurrentText(qry.value(3).toString());
                    ui->capacite->setText(qry.value(4).toString());
                    ui->nbrlits->setText(qry.value(5).toString());
                    ui->stackedWidget->setCurrentIndex(2);
                }
            }
        }
    }

    if (index.column() == 8) {
        QString chambreId = ui->tableView->model()->data(ui->tableView->model()->index(index.row(), 0)).toString();

        // Confirm the deletion
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Delete Chambre", "Are you sure you want to delete this chambre?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            chambre chambreToDelete; // Create an instance of chambre
            if (chambreToDelete.Supprimerchambre(chambreId)) {
                QMessageBox::information(this, "Deleted", "Chambre successfully deleted.");
                ui->tableView->setModel(chambreToDelete.Afficherchambres()); // Refresh the table view
            } else {
                QMessageBox::critical(this, "Error", "Failed to delete chambre.");
            }
        }
    }
    // Close the database connection
    c.closeconnection();
}


void MainWindow::on_modifier_clicked()
{
    QModelIndexList selectedIndexes = ui->tableView->selectionModel()->selectedIndexes();

    if(selectedIndexes.size() == 0) {
        QMessageBox::warning(this, tr("Aucune sélection"), tr("Veuillez sélectionner une ligne à modifier."), QMessageBox::Ok);
        return;
    } else if(selectedIndexes.size() > 1) {
        QMessageBox::warning(this, tr("Sélection multiple"), tr("Veuillez sélectionner une seule ligne à modifier."), QMessageBox::Ok);
        return;
    }

    int ID_C = selectedIndexes.at(0).sibling(selectedIndexes.at(0).row(), 0).data().toInt();

    QString type_c = ui->comboBox_1->currentText();
    int prix = ui->prix->text().toInt();
    QString dispo;
    if (ui->radioButton_1->isChecked()) {
        dispo = "Available";
    } else if (ui->radioButton_2->isChecked()) {
        dispo = "Unavailable";
    }
    QString vue = ui->comboBox_2->currentText();
    int capacite = ui->capacite->text().toInt();
    int nbrlits = ui->nbrlits->text().toInt();

    if (type_c.isEmpty() || ui->prix->text().isEmpty() || dispo.isEmpty() || vue.isEmpty() || ui->capacite->text().isEmpty() || ui->nbrlits->text().isEmpty()) {
        QMessageBox::critical(this, tr("Erreur"), tr("Veuillez remplir tous les champs."), QMessageBox::Cancel);
        return;
    }

    chambre chambre(ID_C, type_c, prix, dispo, vue, capacite, nbrlits);

    if (chambre.Modifierchambre()) {
        ui->tableView->setModel(chambre.Afficherchambres());
        QMessageBox::information(this, tr("Succès"), tr("Modification effectuée."), QMessageBox::Cancel);
        ui->comboBox_1->setCurrentIndex(-1);
        ui->prix->clear();
        ui->radioButton_1->setChecked(false);
        ui->radioButton_2->setChecked(false);
        ui->comboBox_2->setCurrentIndex(-1);
        ui->capacite->clear();
        ui->nbrlits->clear();
    } else {
        QMessageBox::critical(this, tr("Erreur"), tr("Erreur lors de la modification du client."), QMessageBox::Cancel);
    }
}

QSqlQueryModel *chambreModel;

void MainWindow::on_sortComboBox_currentIndexChanged(int index)
{
    chambreModel = new QSqlQueryModel(); // Create a new model each time

    QString queryStr;

    switch (index) {
        case 1: // Sort by PRIX_C (Ascending)
            queryStr = "SELECT ID_C, CAPACITE_M, PRIX_C, NBRLITS, VUE, TYPE_C, ETAT_DIS, 'Update' AS Action, 'Delete' AS Action FROM chambre ORDER BY PRIX_C ASC";
            break;
        case 2: // Sort by ID_C (Ascending)
            queryStr = "SELECT ID_C, CAPACITE_M, PRIX_C, NBRLITS, VUE, TYPE_C, ETAT_DIS, 'Update' AS Action, 'Delete' AS Action FROM chambre ORDER BY ID_C ASC";
            break;
        default:
            queryStr = "SELECT ID_C, CAPACITE_M, PRIX_C, NBRLITS, VUE, TYPE_C, ETAT_DIS, 'Update' AS Action, 'Delete' AS Action FROM chambre";
            break;
    }

    // Set the query for the model
    chambreModel->setQuery(queryStr);

    // Set headers for the columns
    chambreModel->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_C"));
    chambreModel->setHeaderData(1, Qt::Horizontal, QObject::tr("CAPACITE_M"));
    chambreModel->setHeaderData(2, Qt::Horizontal, QObject::tr("PRIX_C"));
    chambreModel->setHeaderData(3, Qt::Horizontal, QObject::tr("NBRLITS"));
    chambreModel->setHeaderData(4, Qt::Horizontal, QObject::tr("VUE"));
    chambreModel->setHeaderData(5, Qt::Horizontal, QObject::tr("TYPE_C"));
    chambreModel->setHeaderData(6, Qt::Horizontal, QObject::tr("ETAT_DIS"));
    chambreModel->setHeaderData(7, Qt::Horizontal, QObject::tr("Action"));
    chambreModel->setHeaderData(8, Qt::Horizontal, QObject::tr("Action"));

    // Set the model for the table view
    ui->tableView->setModel(chambreModel);
}
void MainWindow::exportToPDF()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save PDF"), QDir::currentPath(), tr("PDF files (*.pdf)"));
    if (filePath.isEmpty()) {
        return;
    }

    QAbstractItemModel *model = ui->tableView->model();
    if (!model) {
        qDebug() << "Model not set for the table view!";
        return;
    }

    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);
    printer.setOrientation(QPrinter::Landscape);

    QSizeF pageSize = printer.pageRect().size();
    pageSize.rwidth() *= 1.3; // Increase width by 30%
    pageSize.rheight() *= 1.4; // Increase height by 40%
    printer.setPageSize(QPageSize(pageSize, QPageSize::Point));

    QPainter painter;
    painter.begin(&printer);
    bool success = true;

    int columnPadding = 80;

    // Title setup
    QFont titleFont = painter.font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.setPen(Qt::red);
    QString titleText = "TABLEAU DE GESTION DES CHAMBRES";
    int titleWidth = painter.fontMetrics().width(titleText);
    int titleXPos = (printer.pageRect().width() - titleWidth) / 2;
    int titleYPos = 50;
    painter.drawText(titleXPos, titleYPos, titleText);

    // Draw header row
    int headerYPos = 100;
    QFont headerFont = painter.font();
    headerFont.setPointSize(8);
    headerFont.setBold(true);
    painter.setFont(headerFont);
    painter.setPen(Qt::black);
    for (int col = 0; col < model->columnCount(); ++col) {
        QString headerData = model->headerData(col, Qt::Horizontal).toString();
        if (headerData == "Action") {
            continue; // Skip the Action column
        }

        int xPos = col * 100 + (col + 1) * columnPadding;
        painter.drawText(xPos, headerYPos, headerData);
    }

    // Draw data rows
    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < model->columnCount(); ++col) {
            QModelIndex index = model->index(row, col);
            QString data = model->data(index).toString();
            if (model->headerData(col, Qt::Horizontal).toString() == "Action") {
                continue; // Skip the Action column
            }
            int xPos = col * 100 + (col + 1) * columnPadding;
            int yPos = (row + 1) * 20 + headerYPos;

            painter.drawText(xPos, yPos, data);
        }
    }

    painter.end();

    if (success) {
        QMessageBox::information(this, "Export Status", "Your data has been exported successfully.");
    } else {
        QMessageBox::critical(this, "Export Status", "Failed to export data to PDF.");
    }
}

void MainWindow::displayCharts_roomsByType()
{
    // Create the chart and set background color to white
    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->setBackgroundBrush(QBrush(Qt::white));

    // Create a pie chart series
    QtCharts::QPieSeries *series = new QtCharts::QPieSeries();

    // Query to fetch the number of rooms by type from the 'chambre' table
    QSqlQuery query;
    query.exec("SELECT TYPE_C, COUNT(*) FROM chambre GROUP BY TYPE_C");

    int totalRooms = 0;  // Variable to store the total number of rooms

    // Loop through the query results and populate the pie series
    while (query.next()) {
        QString roomType = query.value(0).toString();  // Room type (e.g., "Single", "Double", etc.)
        int count = query.value(1).toInt();  // Count of rooms of this type
        series->append(roomType + ": " + QString::number(count), count);  // Add data to the pie chart
        totalRooms += count;  // Accumulate the total number of rooms
    }

    // Add the pie series to the chart
    chart->addSeries(series);

    // Set the chart title, showing the total number of rooms and the chart description
    chart->setTitle(QString("<div style='text-align: center;'><font size='4'>Total number of rooms = %1</font></div><div style='text-align: center;'><font size='4'>Rooms by Type</font></div>").arg(totalRooms));

    // Create a chart view to display the chart
    QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);  // Enable smooth rendering

    // Set the minimum size of the chart view
    chartView->setMinimumSize(700, 550);

    // Capture the chart view as a QPixmap and set it in the QLabel
    QPixmap pixmap = chartView->grab();

    // Set the pixmap in the UI element (e.g., QLabel or another widget) to display the chart
    ui->stat_type->setPixmap(pixmap);

    // Optionally, adjust label size to fit the pixmap
    ui->stat_type->setScaledContents(true);
}


void MainWindow::on_gostat_clicked()
{
    displayCharts_roomsByType();
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::Rechercherchambre(const QString &text)
{
    chambre c;
    QSqlQueryModel *model = c.Rechercherchambre(text);
    ui->tableView->setModel(model);
}


void MainWindow::navigateToPage(int pageIndex)
{
    ui->stackedWidget->setCurrentIndex(pageIndex);
}
void MainWindow::on_pushButton_login_clicked() {
    QString userID = ui->lineEdit_User_Name->text(); // Username entered by the user
    QString password = ui->lineEdit_Password->text(); // Password entered by the user

    // Check if the password contains only numbers using regular expression
    QRegularExpression re("^[0-9]+$"); // Regex for numeric passwords only
    QRegularExpressionMatch match = re.match(password);

    if (!match.hasMatch()) {
        // If password is not numeric
        QMessageBox::warning(this, "Login Failed", "Invalid password! Password must be numeric.");
        return;
    }

    // Query the database to check if the ID exists in TABLE4
    QSqlQuery query;
    query.prepare("SELECT ID FROM TABLE4 WHERE ID = :id");
    query.bindValue(":id", userID);

    if (!query.exec()) {
        // Handle database query error
        QMessageBox::critical(this, "Database Error", query.lastError().text());
        return;
    }

    if (query.next()) {
        // If the ID exists in the database
        QMessageBox::information(this, "Login Successful", "Welcome!");
        ui->stackedWidget->setCurrentIndex(1); // Navigate to the next page
    } else {
        // If the ID does not exist
        QMessageBox::warning(this, "Login Failed", "Invalid username! No such ID exists in the database.");
    }
}

void MainWindow::on_pushButton_cancel_clicked() {
    qDebug() << "Cancel button clicked";
    QApplication::quit();
}

void MainWindow::on_stackedWidget_currentChanged(int index) {
    qDebug() << "Current page changed to index:" << index;
    // Add actions for when the page changes
}
