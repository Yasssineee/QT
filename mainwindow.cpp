#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "client.h" // Include the client header
#include "connection.h"
#include <QDebug>
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
#include <QDate>
#include <QPropertyAnimation>

using namespace QtCharts;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    QStackedWidget* stackedWidget = ui->stackedWidget;
    QWidget* page = stackedWidget->widget(2);
    QTableView* tableView = page->findChild<QTableView*>("tableView");

    if (tableView) {

        client client;
        Connection c;

        if (c.createconnect()) {
            QMessageBox::information(nullptr, QObject::tr("Database is open"),
                                     QObject::tr("Connection successful.\nClick Cancel to exit."),
                                     QMessageBox::Cancel);

            tableView->setModel(client.Afficherclients());
            tableView->resizeColumnsToContents();
            tableView->horizontalHeader()->setStretchLastSection(true);








        } else {
            QMessageBox::critical(nullptr, QObject::tr("Database is not open"),
                                  QObject::tr("Connection failed.\nClick Cancel to exit."),
                                  QMessageBox::Cancel);
        }
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Error"),
                              QObject::tr("Failed to find the table view for Client data."),
                              QMessageBox::Cancel);
    }

    connect(ui->gostat, &QPushButton::clicked, this, [this]() { navigateToPage(3); });
    connect(ui->back, &QPushButton::clicked, this, [this]() { navigateToPage(2); });

    //pdf connecter
      connect(ui->pdf, &QPushButton::clicked, this, &MainWindow::exportToPDF);
     //excel connecter
      connect(ui->excel, &QPushButton::clicked, this, &MainWindow::exportToExcel);
    //recherche connecter
      connect(ui->search_bar, &QLineEdit::textChanged, this, &MainWindow::Rechercherclient);
    //tri connecter
      connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_sortComboBox_currentIndexChanged(int)));

      QDate selectedDate = ui->calendrier_reserv->selectedDate();
      updateCalendarAndEventDetails(selectedDate);

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_ajouter_clicked()
{
    QString nom = ui->nom->text();
    QString prenom = ui->prnom->text();
    QString gender = ui->comboBox_7->currentText();
    QString telStr = ui->nbrphone->text();
    int tel = telStr.toInt();
    QString adresse = ui->adresse->text();
    QString email = ui->mail->text();
    QString nationality = ui->nationality->text();
    QDate datereservation = ui->Date_reser->date();

    // Create a QSqlQuery to fetch the DATE_RESERVATION values from the database
    QSqlQuery query("SELECT DATE_RESERVATION FROM CLIENT");

    // Flag to indicate if DATE_RESERVATION corresponds to the clicked date
    bool dateFound = false;

    // Iterate through the query results
    while (query.next()) {
        // Extract the DATE_RESERVATION value from the query
        QDate datereservation_check = query.value(0).toDate();

        // Check if DATE_RESERVATION corresponds to the clicked date
        if (datereservation_check == datereservation) {
            // DATE_RESERVATION corresponds to the clicked date, so set the flag and break the loop
            dateFound = true;
            break;
        }
    }

    // Input validation
    QRegExp nameRx("^[a-zA-Z]+$"); // Only letters
    QRegExp telRx("^\\d+$"); // Only digits
    QRegExp emailRx("^[\\w-\\.]+@([\\w-]+\\.)+[\\w-]{2,4}$"); // Email format

    if (!nameRx.exactMatch(nom) || !nameRx.exactMatch(prenom)) {
        QMessageBox::critical(this, tr("Erreur"), tr("Le nom et le prénom doivent contenir uniquement des lettres."), QMessageBox::Cancel);
    } else if (!telRx.exactMatch(telStr)) {
        QMessageBox::critical(this, tr("Erreur"), tr("Le téléphone doit être des nombres valides."), QMessageBox::Cancel);
    } else if (!emailRx.exactMatch(email)) {
        QMessageBox::critical(this, tr("Erreur"), tr("L'email doit être au format valide."), QMessageBox::Cancel);
    } else if (dateFound) {
        QMessageBox::critical(this, tr("Erreur"), tr("Veuillez choisir une autre date, cette date est occupée."), QMessageBox::Cancel);
    } else {
        // Create client object
        client client(0, nom, prenom, tel, adresse, gender, nationality, email, datereservation, 0);

        if (client.Ajouterclient()) {
            // Refresh the table view if addition was successful
            ui->tableView->setModel(client.Afficherclients());
            QMessageBox::information(this, tr("Succès"), tr("Ajout effectué."), QMessageBox::Cancel);

            // Check if the client is every 5th client
            QSqlQuery query("SELECT MAX(ID_CLIENT) FROM client");
            if (query.next()) {
                int id_client = query.value(0).toInt();
                qDebug() << "ID_CLIENT: " << id_client;
                if (id_client % 2 == 0) {
                    // Set POINTS_FID to 10
                    QSqlQuery updateQuery;
                    updateQuery.prepare("UPDATE client SET POINTS_FID = 10 WHERE ID_CLIENT = :id_client");
                    updateQuery.bindValue(":id_client", id_client);
                    if (updateQuery.exec()) {
                        qDebug() << "Points successfully added to client ID: " << id_client;
                    } else {
                        qDebug() << "Failed to update POINTS_FID:" << updateQuery.lastError();
                    }

                    QString message = tr("Congrats! You won 20% reduction");

                    QLabel *label = new QLabel(message, this);
                    label->setStyleSheet("QLabel { font-size: 24px; color: white; background-color: rgba(0, 0, 0, 200); padding: 20px; border-radius: 10px; }");
                    label->setAlignment(Qt::AlignCenter);
                    label->setGeometry((width() - 600) / 2, (height() - 200) / 2, 600, 200); // Increased size to 600x200
                    label->show();

                    QPropertyAnimation *animation = new QPropertyAnimation(label, "windowOpacity");
                    animation->setDuration(10000);
                    animation->setStartValue(1.0);
                    animation->setEndValue(0.0);
                    animation->start(QAbstractAnimation::DeleteWhenStopped);

                    // Auto delete the label after the animation finishes
                    connect(animation, &QPropertyAnimation::finished, label, &QLabel::deleteLater);
                }
            }

            // Clear input fields after addition
            ui->nom->clear();
            ui->prnom->clear();
            ui->comboBox_7->setCurrentIndex(-1);
            ui->nbrphone->clear();
            ui->adresse->clear();
            ui->mail->clear();
            ui->nationality->clear();
            ui->Date_reser->clear();
        } else {
            QMessageBox::critical(this, tr("Erreur"), tr("Erreur lors de l'ajout du client."), QMessageBox::Cancel);
        }
    }

    QDate selectedDate = ui->calendrier_reserv->selectedDate();
    updateCalendarAndEventDetails(selectedDate);
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

    qry.prepare("SELECT NOM_CL, PRENOM_CL, GENDER, TEL_CL, ADRESSE_CL, NATIONALITY, EMAIL, DATE_RESERVATION FROM client WHERE ID_CLIENT = :ID_CLIENT");
    qry.bindValue(":ID_CLIENT", value);

    if(qry.exec())
    {
        if(qry.next())
        {
            ui->id->setText(value);

            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Update Client", "Are you sure you want to Update this client?",
                                            QMessageBox::Yes|QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                    ui->nom->setText(qry.value(0).toString());
                    ui->prnom->setText(qry.value(1).toString());
                    ui->comboBox_2->setCurrentText(qry.value(2).toString());
                    ui->nbrphone->setText(qry.value(3).toString());
                    ui->adresse->setText(qry.value(4).toString());
                    ui->nationality->setText(qry.value(5).toString());
                    ui->mail->setText(qry.value(6).toString());
                    ui->Date_reser->setDate(qry.value(7).toDate());
                    ui->id->setText(value);
                    ui->stackedWidget->setCurrentIndex(2);
                }
            QDate selectedDate = ui->calendrier_reserv->selectedDate();
            updateCalendarAndEventDetails(selectedDate);

        }
    }

        QDate selectedDate = ui->calendrier_reserv->selectedDate();
        updateCalendarAndEventDetails(selectedDate);
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

    int Id_client = selectedIndexes.at(0).sibling(selectedIndexes.at(0).row(), 0).data().toInt();

    QString nom = ui->nom->text();
    QString prenom = ui->prnom->text();
    QString gender = ui->comboBox_2->currentText();
    int tel = ui->nbrphone->text().toInt();
    QString adresse = ui->adresse->text();
    QString nationality = ui->nationality->text();
    QString email = ui->mail->text();
    QDate datereservation= ui->Date_reser->date();

    // Create a QSqlQuery to fetch the DATE_RESERVATION values from the database, excluding the current client
      QSqlQuery query;
      query.prepare("SELECT DATE_RESERVATION FROM CLIENT WHERE ID_CLIENT != :id");
      query.bindValue(":id", Id_client);
      query.exec();

      // Flag to indicate if DATE_RESERVATION corresponds to the clicked date
      bool dateFound = false;

      // Iterate through the query results
      while (query.next()) {
          QDate datereservation_check = query.value(0).toDate();

          // Check if DATE_RESERVATION corresponds to the clicked date
          if (datereservation_check == datereservation) {
              dateFound = true;
              break;
          }
      }
    // Input validation
    QRegExp nameRx("^[a-zA-Z]+$"); // Only letters
    QRegExp telRx("^\\d+$"); // Only digits
    QRegExp emailRx("^[\\w-\\.]+@([\\w-]+\\.)+[\\w-]{2,4}$"); // Email format

    if (!nameRx.exactMatch(nom) || !nameRx.exactMatch(prenom)) {
        QMessageBox::critical(this, tr("Erreur"), tr("Le nom et le prénom doivent contenir uniquement des lettres."), QMessageBox::Cancel);
    }  else if (!emailRx.exactMatch(email)) {
        QMessageBox::critical(this, tr("Erreur"), tr("L'email doit être au format valide."), QMessageBox::Cancel);
    } else if (dateFound) {
        QMessageBox::critical(this, tr("Erreur"), tr("Veuillez choisir une autre date , cette date est occupée."), QMessageBox::Cancel);
    }else{

    client client(Id_client, nom, prenom, tel, adresse, gender, nationality, email,datereservation,0);

    if (client.Modifierclient()) {
        ui->tableView->setModel(client.Afficherclients());
        QMessageBox::information(this, tr("Succès"), tr("Modification effectuée."), QMessageBox::Cancel);
        ui->nom->clear();
        ui->prnom->clear();
        ui->comboBox_2->setCurrentIndex(-1);
        ui->nbrphone->clear();
        ui->adresse->clear();
        ui->mail->clear();
        ui->nationality->clear();
    } else {
        QMessageBox::critical(this, tr("Erreur"), tr("Erreur lors de la modification du client."), QMessageBox::Cancel);
    }
    }

    QDate selectedDate = ui->calendrier_reserv->selectedDate();
    updateCalendarAndEventDetails(selectedDate);
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

    QFont titleFont = painter.font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.setPen(Qt::red);
    QString titleText = "TABLEAU DE GESTION DES CLIENTS";
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
            continue;
        }
        int xPos = col * 100 + (col + 1) * columnPadding;
        painter.drawText(xPos, headerYPos, headerData);
    }

    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < model->columnCount(); ++col) {
            QModelIndex index = model->index(row, col);
            QString data = model->data(index).toString();
            if (model->headerData(col, Qt::Horizontal).toString() == "Action") {
                continue;
            }
            int xPos = col * 100 + (col + 1) * columnPadding;
            int yPos = (row + 1) * 20 + headerYPos;



            painter.drawText(xPos, yPos, data);
        }
    }

    painter.end();

    if (printer.newPage() || success) {
        QMessageBox::information(this, "Export Status", "Your data has been exported successfully.");
    } else {
        QMessageBox::critical(this, "Export Status", "Failed to export data to PDF.");
    }
}

void MainWindow::exportToExcel()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Export Excel", "", "*.xlsx");
    if (!fileName.isEmpty()) {
        QAxObject* excel = new QAxObject("Excel.Application");
        QAxObject* workbooks = excel->querySubObject("Workbooks");
        QAxObject* workbook = workbooks->querySubObject("Add");
        QAxObject* sheets = workbook->querySubObject("Worksheets");
        QAxObject* sheet = sheets->querySubObject("Item(int)", 1);

        QAbstractItemModel *model = ui->tableView->model();


        QAxObject* rangeTitle = sheet->querySubObject("Range(const QString&)", "A1:C1");
        rangeTitle->dynamicCall("Merge()"); // Merge cells for the title
        rangeTitle->setProperty("Value", "TABLEAU DE GESTION DES CLIENTS");
        QAxObject* titleFont = rangeTitle->querySubObject("Font");
        titleFont->setProperty("Bold", true);
        titleFont->setProperty("Color", QColor(Qt::red));


        for (int col = 0; col < model->columnCount(); ++col) {
            QString headerData = model->headerData(col, Qt::Horizontal).toString();

            if (headerData == "Action") {
                continue;
            }

            QAxObject* cell = sheet->querySubObject("Cells(int,int)", 2, col + 1);
            cell->dynamicCall("SetValue(const QVariant&)", headerData);
            // Apply bold font to header cell
            QAxObject* headerFont = cell->querySubObject("Font");
            headerFont->setProperty("Bold", true);
        }

        for (int i = 0; i < model->rowCount(); ++i) {
            int excelCol = 1;
            for (int j = 0; j < model->columnCount(); ++j) {
                if (model->headerData(j, Qt::Horizontal).toString() == "Action") {
                    continue;
                }

                QModelIndex index = model->index(i, j);
                QString data = model->data(index).toString();
                QAxObject* cell = sheet->querySubObject("Cells(int,int)", i + 3, excelCol);
                cell->dynamicCall("SetValue(const QVariant&)", data);
                excelCol++;
            }
        }

        QAxObject* usedRange = sheet->querySubObject("UsedRange");
        if (usedRange) {
            QAxObject* rows = usedRange->querySubObject("Rows");
            QAxObject* columns = usedRange->querySubObject("Columns");
            if (rows && columns) {
                rows->setProperty("RowHeight", 20);
                columns->setProperty("ColumnWidth", 20);
            }
            delete usedRange;
        }

        workbook->dynamicCall("SaveAs(const QString&)", QDir::toNativeSeparators(fileName));
        workbook->dynamicCall("Close()");
        excel->dynamicCall("Quit()");
        delete excel;


        QMessageBox::information(this, "Export Status", "Your data has been exported successfully.");
    } else {
        QMessageBox::critical(this, "Export Status", "Failed to export data to Excel.");
    }
}


void MainWindow::Rechercherclient(const QString &text)
{
    client c;
    QSqlQueryModel *model = c.Rechercherclient(text);
    ui->tableView->setModel(model);
}




QSqlQueryModel *clientModel;

void MainWindow::on_sortComboBox_currentIndexChanged(int index)
{
    clientModel = new QSqlQueryModel(); // Create a new model each time

    QString queryStr;

    switch (index) {
        case 1: // Sort by Prénom (A-Z)
            queryStr = "SELECT ID_CLIENT, NOM_CL, PRENOM_CL, TEL_CL, ADRESSE_CL, GENDER, nationality, EMAIL, 'Update' AS Action, 'Delete' AS Action FROM client ORDER BY PRENOM_CL ASC";
            break;
        case 2: // Sort by ID_CLIENT (Ascending)
            queryStr = "SELECT ID_CLIENT, NOM_CL, PRENOM_CL, TEL_CL, ADRESSE_CL, GENDER, nationality, EMAIL, 'Update' AS Action, 'Delete' AS Action FROM client ORDER BY ID_CLIENT ASC";
            break;
        default:
            queryStr = "SELECT ID_CLIENT, NOM_CL, PRENOM_CL, TEL_CL, ADRESSE_CL, GENDER, nationality, EMAIL, 'Update' AS Action, 'Delete' AS Action FROM client";
            break;
    }

    clientModel->setQuery(queryStr);

    clientModel->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_CLIENT"));
    clientModel->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM_CL"));
    clientModel->setHeaderData(2, Qt::Horizontal, QObject::tr("PRENOM_CL"));
    clientModel->setHeaderData(3, Qt::Horizontal, QObject::tr("TEL_CL"));
    clientModel->setHeaderData(4, Qt::Horizontal, QObject::tr("ADRESSE_CL"));
    clientModel->setHeaderData(5, Qt::Horizontal, QObject::tr("GENDER"));
    clientModel->setHeaderData(6, Qt::Horizontal, QObject::tr("NATIONALITY"));
    clientModel->setHeaderData(7, Qt::Horizontal, QObject::tr("EMAIL"));
    clientModel->setHeaderData(8, Qt::Horizontal, QObject::tr("Action"));
    clientModel->setHeaderData(9, Qt::Horizontal, QObject::tr("Action"));

    ui->tableView->setModel(clientModel);
}

void MainWindow::displayCharts_clientByGender()
{
    QtCharts::QChart *chart = new QtCharts::QChart();

    chart->setBackgroundBrush(QBrush(Qt::white));

    QtCharts::QPieSeries *series = new QtCharts::QPieSeries();

    QSqlQuery query;
    query.exec("SELECT GENDER, COUNT(*) FROM client GROUP BY GENDER");

    int totalClients = 0;

    while (query.next()) {
        QString gender = query.value(0).toString();
        int count = query.value(1).toInt();
        series->append(gender + ": " + QString::number(count), count);
        totalClients += count;
    }

    chart->addSeries(series);

    chart->setTitle(QString("<div style='text-align: center;'><font size='4'>Total number of clients = %1</font></div><div style='text-align: center;'><font size='4'>Clients by Gender</font></div>").arg(totalClients));

    QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    chartView->setMinimumSize(700, 550);


    QPixmap pixmap = chartView->grab();



    ui->stat_gender->setPixmap(pixmap);
}

void MainWindow::on_gostat_clicked()
{
    displayCharts_clientByGender();
    ui->stackedWidget->setCurrentIndex(3);
}




void MainWindow::navigateToPage(int pageIndex)
{
    ui->stackedWidget->setCurrentIndex(pageIndex);
}

void MainWindow::on_calendrier_reserv_clicked(const QDate &date)
{
    // Create a QSqlQuery to fetch the idClient for the clicked date from the database
    QSqlQuery idQuery;
    idQuery.prepare("SELECT ID_CLIENT FROM CLIENT WHERE DATE_RESERVATION >= ? AND DATE_RESERVATION < ?");
    QDateTime startOfDay = QDateTime(date);
    QDateTime endOfDay = startOfDay.addDays(1);
    idQuery.addBindValue(startOfDay);
    idQuery.addBindValue(endOfDay);

    if (!idQuery.exec()) {
        qDebug() << "Failed to execute query:" << idQuery.lastError().text();
        return;
    }

    if (idQuery.next()) {
        // Extract idClient from the query result
        QString idClient = idQuery.value(0).toString();

        // Format the string with HTML to make "ID:" red
        QString formattedIDText = "<span style='color: red;'>ID:</span> " + idClient;

        // Create a QSqlQuery to fetch all information related to the retrieved idClient
        QSqlQuery query;
        query.prepare("SELECT NOM_CL, PRENOM_CL, TEL_CL, ADRESSE_CL, GENDER, NATIONALITY, EMAIL, DATE_RESERVATION, POINTS_FID FROM CLIENT WHERE ID_CLIENT = ?");
        query.addBindValue(idClient);

        if (!query.exec()) {
            qDebug() << "Failed to execute query:" << query.lastError().text();
            return;
        }

        if (query.next()) {
            // Extract fields from the query result with HTML formatting
            QString nomClient = "<span style='color: red;'>Nom:</span> " + query.value(0).toString();
            QString prenom = "<span style='color: red;'>Prenom:</span> " + query.value(1).toString();
            QString telephone = "<span style='color: red;'>Telephone:</span> " + query.value(2).toString();
            QString adress = "<span style='color: red;'>Adress:</span> " + query.value(3).toString();
            QString Gender = "<span style='color: red;'>Gender:</span> " + query.value(4).toString();
            QString Nationalite = "<span style='color: red;'>Nationalite:</span> " + query.value(5).toString();
            QString Email = "<span style='color: red;'>Email:</span> " + query.value(6).toString();
            QDate datereception = query.value(7).toDate();
            QString formattedDateRservation = "<span style='color: red;'>Date Reservation:</span> " + datereception.toString("dd/MM/yyyy");
            QString pts_fid = "<span style='color: red;'>Points Fidelité:</span> " + query.value(5).toString();



            // Set the retrieved values to corresponding labels
            ui->label_id->setText(formattedIDText);
            ui->label_nom->setText(nomClient);
            ui->label_prenom->setText(prenom);
            ui->label_tel->setText(telephone);
            ui->label_adress->setText(adress);
            ui->label_gender->setText(Gender);
            ui->label_nationalite->setText(Nationalite);
            ui->label_email->setText(Email); // Format with date
            ui->label_datereserv->setText(formattedDateRservation);// Format with
            ui->label_ptsfid->setText(pts_fid);// Format with

            // Make labels visible
            ui->label_id->setVisible(true);
            ui->label_nom->setVisible(true);
            ui->label_prenom->setVisible(true);
            ui->label_tel->setVisible(true);
            ui->label_adress->setVisible(true);
            ui->label_gender->setVisible(true);
            ui->label_nationalite->setVisible(true);
            ui->label_email->setVisible(true);
            ui->label_datereserv->setVisible(true);
            ui->label_ptsfid->setVisible(true);


            //ui->no_events_today_image->setVisible(false);
        }
    } else {
        // No data found for the clicked date, hide labels
        ui->label_id->setVisible(false);
        ui->label_nom->setVisible(false);
        ui->label_prenom->setVisible(false);
        ui->label_tel->setVisible(false);
        ui->label_adress->setVisible(false);
        ui->label_gender->setVisible(false);
        ui->label_nationalite->setVisible(false);
        ui->label_email->setVisible(false);
        ui->label_datereserv->setVisible(false);
        ui->label_ptsfid->setVisible(false);

        //ui->no_events_today_image->setVisible(true);
    }
}





void MainWindow::updateCalendarAndEventDetails(const QDate &date) {
    // Create a QSqlQuery to fetch the idClient for the clicked date from the database
    QSqlQuery idQuery;
    idQuery.prepare("SELECT ID_CLIENT FROM CLIENT WHERE DATE_RESERVATION >= ? AND DATE_RESERVATION < ?");
    QDateTime startOfDay = QDateTime(date);
    QDateTime endOfDay = startOfDay.addDays(1);
    idQuery.addBindValue(startOfDay);
    idQuery.addBindValue(endOfDay);

    if (!idQuery.exec()) {
        qDebug() << "Failed to execute query:" << idQuery.lastError().text();
        return;
    }

    if (idQuery.next()) {
        // Extract idClient from the query result
        QString idClient = idQuery.value(0).toString();

        // Format the string with HTML to make "ID:" red
        QString formattedIDText = "<span style='color: red;'>ID:</span> " + idClient;

        // Create a QSqlQuery to fetch all information related to the retrieved idClient
        QSqlQuery query;
        query.prepare("SELECT NOM_CL, PRENOM_CL, TEL_CL, ADRESSE_CL, GENDER, NATIONALITY, EMAIL, DATE_RESERVATION, POINTS_FID FROM CLIENT WHERE ID_CLIENT = ?");
        query.addBindValue(idClient);

        if (!query.exec()) {
            qDebug() << "Failed to execute query:" << query.lastError().text();
            return;
        }

        if (query.next()) {
            // Extract fields from the query result with HTML formatting
            QString nomClient = "<span style='color: red;'>Nom:</span> " + query.value(0).toString();
            QString prenom = "<span style='color: red;'>Prenom:</span> " + query.value(1).toString();
            QString telephone = "<span style='color: red;'>Telephone:</span> " + query.value(2).toString();
            QString adress = "<span style='color: red;'>Adress:</span> " + query.value(3).toString();
            QString Gender = "<span style='color: red;'>Gender:</span> " + query.value(4).toString();
            QString Nationalite = "<span style='color: red;'>Nationalite:</span> " + query.value(5).toString();
            QString Email = "<span style='color: red;'>Email:</span> " + query.value(6).toString();
            QDate datereception = query.value(7).toDate();
            QString formattedDateRservation = "<span style='color: red;'>Date Reservation:</span> " + datereception.toString("dd/MM/yyyy");
            QString pts_fid = "<span style='color: red;'>Points Fidelité:</span> " + query.value(5).toString();



            // Set the retrieved values to corresponding labels
            ui->label_id->setText(formattedIDText);
            ui->label_nom->setText(nomClient);
            ui->label_prenom->setText(prenom);
            ui->label_tel->setText(telephone);
            ui->label_adress->setText(adress);
            ui->label_gender->setText(Gender);
            ui->label_nationalite->setText(Nationalite);
            ui->label_email->setText(Email); // Format with date
            ui->label_datereserv->setText(formattedDateRservation);// Format with
            ui->label_ptsfid->setText(pts_fid);// Format with

            // Make labels visible
            ui->label_id->setVisible(true);
            ui->label_nom->setVisible(true);
            ui->label_prenom->setVisible(true);
            ui->label_tel->setVisible(true);
            ui->label_adress->setVisible(true);
            ui->label_gender->setVisible(true);
            ui->label_nationalite->setVisible(true);
            ui->label_email->setVisible(true);
            ui->label_datereserv->setVisible(true);
            ui->label_ptsfid->setVisible(true);


            //ui->no_events_today_image->setVisible(false);
        }
    } else {
        // No data found for the clicked date, hide labels
        ui->label_id->setVisible(false);
        ui->label_nom->setVisible(false);
        ui->label_prenom->setVisible(false);
        ui->label_tel->setVisible(false);
        ui->label_adress->setVisible(false);
        ui->label_gender->setVisible(false);
        ui->label_nationalite->setVisible(false);
        ui->label_email->setVisible(false);
        ui->label_datereserv->setVisible(false);
        ui->label_ptsfid->setVisible(false);

        //ui->no_events_today_image->setVisible(true);
    }

    // Clear existing date text formats in the calendar
    ui->calendrier_reserv->setDateTextFormat(QDate(), QTextCharFormat());

    // Create a QSqlQuery to fetch the DATE_DEBUT values from the database
    QSqlQuery calendarQuery("SELECT DATE_RESERVATION FROM client");

    QList<QDate> dateDebuts;
    // Iterate through the query results
    while (calendarQuery.next()) {
        // Extract the DATE_RECEPTION value from the query
        QDate dateDebut = calendarQuery.value(0).toDate();
        dateDebuts.append(dateDebut);

        // Set the color for the date in the calendar
        QTextCharFormat format;
        format.setBackground(Qt::green); // Set background color to green
        ui->calendrier_reserv->setDateTextFormat(dateDebut, format);
    }

    // Get today's date
    QDate currentDate = QDate::currentDate();

    // Check if today's date corresponds to any DATE_RECEPTION
    if (dateDebuts.contains(currentDate)) { // Today's date corresponds to a DATE_DEBUT
        // Make labels visible
        ui->label_id->setVisible(true);
        ui->label_nom->setVisible(true);
        ui->label_prenom->setVisible(true);
        ui->label_tel->setVisible(true);
        ui->label_adress->setVisible(true);
        ui->label_gender->setVisible(true);
        ui->label_nationalite->setVisible(true);
        ui->label_email->setVisible(true);
        ui->label_datereserv->setVisible(true);
        ui->label_ptsfid->setVisible(true);



        //ui->no_events_today_image->setVisible(false);
    } else {
        // No data found for the clicked date, hide labels
        ui->label_id->setVisible(false);
        ui->label_nom->setVisible(false);
        ui->label_prenom->setVisible(false);
        ui->label_tel->setVisible(false);
        ui->label_adress->setVisible(false);
        ui->label_gender->setVisible(false);
        ui->label_nationalite->setVisible(false);
        ui->label_email->setVisible(false);
        ui->label_datereserv->setVisible(false);
        ui->label_ptsfid->setVisible(false);

        //ui->no_events_today_image->setVisible(true);
    }
}





void MainWindow::on_calendrier_clicked()
{
    QDate selectedDate = ui->calendrier_reserv->selectedDate();
    updateCalendarAndEventDetails(selectedDate);
    ui->stackedWidget->setCurrentIndex(6);
}

void MainWindow::on_Back_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_supprimerbtn_clicked()
{
    QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirmation", "voulez vous supprimer ce evenement ?", QMessageBox::Yes | QMessageBox::No);
    int ID_CLIENT = ui->id->text().toInt();
    client ev;
    if (reply == QMessageBox::Yes){
    ev.setID_CLIENT(ID_CLIENT);
    bool test =ev.Supprimerclients();


    if(test){
        ui->tableView->setModel(ev.Afficherclients());
        QMessageBox::information(nullptr,QObject::tr("OK"),
                                 QObject::tr("Suppression effectuée\n"
                                             "Click Cancel to exit."),QMessageBox::Cancel);
        //ui->type_gender->setCurrentIndex(0);
        //ui->lineEdit_nom_event->clear();
        //ui->lineEdit_nom_galerie->clear();
        //ui->lineEdit_nbre_invites->clear();
        //ui->lineEdit_adresse_galerie->clear();
        //ui->lineEdit_nbre_oeuvres->clear();
        //ui->textEdit_description->clear();
    }
    else {
        QMessageBox::critical(nullptr,QObject::tr("NOT OK"),
                                 QObject::tr("Suppression non effectuée\n"
                                             "Click Cancel to exit."),QMessageBox::Cancel);
    }
    }


    QDate selectedDate = ui->calendrier_reserv->selectedDate();
    updateCalendarAndEventDetails(selectedDate);
}
