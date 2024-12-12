#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "hygiene.h"
#include "connection.h"
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QInputDialog>
#include <QFileDialog>
#include <QPainter>

#include <QDateTime>
#include <QSqlQuery>
#include <QSqlError>
#include <QSerialPort>

#include <QImage>
#include <QPainter>
#include <QPen>


#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>

#include <QtCharts>
#include <QChartView>
#include <QBarSeries>
#include <QBarSet>
#include <QCategoryAxis>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "smtpp.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , serial(new QSerialPort(this))

{
    ui->setupUi(this);

    Connection conn;
    if (conn.createconnect()) {
        QMessageBox::information(this, "Connexion", "5edmet hmdlh ya rabii");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec impossible");
    }

    ui->tableView25->setModel(Etmp.afficher());
    this->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);



    connect(ui->pushButton_pdf, &QPushButton::clicked, this, &MainWindow::exportToPDF);
    connect(ui->pushButton_search, &QPushButton::clicked, this, &MainWindow::rechercherParQuantiteEnStock);
    connect(ui->pushButton_generateBarcode, &QPushButton::clicked, this, &MainWindow::on_pushButton_generateBarcode_clicked);


    serial->setPortName("COM6"); // Replace with your Arduino's COM port
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (serial->open(QIODevice::ReadWrite)) {
        ui->statusLabel->setText("Connected to Arduino");
    } else {
        QMessageBox::critical(this, "Error", "Failed to connect to Arduino.");
        return;
    }

    // Connect serial data read event to the readFromArduino function
    connect(serial, &QSerialPort::readyRead, this, &MainWindow::on_readFromArduino);
}

MainWindow::~MainWindow()
{
    if (serial->isOpen()) {
        serial->close();
    }
    delete ui;
    m_selectedId = "";


}





void MainWindow::on_pushButton_92_clicked() {
    QString typeproduith = ui->lineEdit_type_produit->text();
    QString quantiteenstockh = ui->lineEdit_qua_stock->text();
    QString quantiteminimalh = ui->lineEdit_qua_mini->text();
    QString id_produith = ui->lineEdit_id->text();
    QString quantiteapresajouth = ui->lineEdit_apres_ajou->text();

    // Validation for the form fields
    if (quantiteminimalh.isEmpty() || quantiteapresajouth.isEmpty() ||
        quantiteenstockh.isEmpty() || typeproduith.isEmpty() || typeproduith.length() <= 2) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs obligatoires, "
                                             "et assurez-vous que le type de produit contient plus de 2 caractères.");
        return;
    }

    hygiene h(typeproduith, quantiteenstockh, quantiteminimalh, id_produith, quantiteapresajouth);
    bool success = h.ajouter();

    if (success) {
        QMessageBox::information(this, "Succès", "Ajout effectué avec succès.");
        ui->tableView25->setModel(h.afficher());  // Refresh the table view with the updated data
    } else {
        QMessageBox::critical(this, "Erreur", "Ajout non effectué.");
    }
}




void MainWindow::on_tableView25_clicked(const QModelIndex &index)
{
    // Get the ID of the selected row (assuming the ID is in the first column)
    m_selectedId = index.sibling(index.row(), 3).data().toString();  // Adjust column index for ID_PRODUITH   //3

    // Get other fields from the selected row (adjust the column indexes based on your table structure)
    QString typeproduith = index.sibling(index.row(), 0).data().toString();      //4201
    QString quantiteenstockh = index.sibling(index.row(), 1).data().toString();
    QString quantiteminimalh = index.sibling(index.row(), 2).data().toString();
    QString quantiteapresajouth = index.sibling(index.row(), 4).data().toString();


    // Set the data to the respective QLineEdits
    ui->lineEdit_type_produit->setText(typeproduith);
    ui->lineEdit_qua_stock->setText(quantiteenstockh);
    ui->lineEdit_qua_mini->setText(quantiteminimalh);
    ui->lineEdit_id->setText(m_selectedId);  // Use m_selectedId here
    ui->lineEdit_apres_ajou->setText(quantiteapresajouth);

    // Enable the barcode generation button when a valid row is selected
    ui->pushButton_generateBarcode->setEnabled(true);
}


void MainWindow::on_pushButton_94_clicked() {
    if (m_selectedId.isEmpty()) {
        QMessageBox::warning(this, "Select hygiene", "Please select a hygiene to delete.");
        return;
    }

    // Confirm deletion
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm Deletion", "Are you sure you want to delete this hygiene?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        hygiene hygiene;
        if (hygiene.supprimer(m_selectedId)) {
            QMessageBox::information(this, "Success", "Hygiene deleted successfully!");
            ui->tableView25->setModel(Etmp.afficher());  // Refresh after deletion
        } else {
            QMessageBox::warning(this, "Failure", "Failed to delete hygiene.");
        }
    }
}

void MainWindow::on_pushButton_93_clicked() {
    // Capture updated data from line edits
    QString typeproduith = ui->lineEdit_type_produit->text();
    QString quantiteenstockh = ui->lineEdit_qua_stock->text();
    QString quantiteminimalh = ui->lineEdit_qua_mini->text();
    QString id_produith = ui->lineEdit_id->text();
    QString quantiteapresajouth = ui->lineEdit_apres_ajou->text();

    hygiene h(typeproduith,quantiteenstockh,quantiteminimalh,id_produith,  quantiteapresajouth );

    bool success = h.modifier();

    if (success) {
        QMessageBox::information(this, "Succès", "Les données ont été mises à jour avec succès.");
        ui->tableView25->setModel(h.afficher());  // Refresh table view after modification
    } else {
        QMessageBox::critical(this, "Erreur", "La mise à jour a échoué.");
    }
}

void MainWindow::exportToPDF() {
    // Ask user to choose where to save the PDF
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save PDF"), "", tr("PDF Files (*.pdf)"));
    if (filePath.isEmpty()) {
        return; // Cancelled by user
    }

    // Initialize PDF writer
    QPdfWriter pdfWriter(filePath);
    pdfWriter.setPageSize(QPageSize::A4);
    pdfWriter.setResolution(300); // High resolution

    QPainter painter(&pdfWriter);

    // Set document title with larger font size
    painter.setFont(QFont("Arial", 20, QFont::Bold)); // Increased font size for title
    painter.drawText(QRect(0, 0, pdfWriter.width(), 50), Qt::AlignCenter, "Hygiene Product Report");

    int yPos = 90; // Start below the title with more space
    int rowHeight = 40; // Increased row height for more space between rows
    int colWidth = (pdfWriter.width() - 100) / 5; // Adjust column width considering margins
    int margin = 80; // Increased margin for more space around the content

    painter.setFont(QFont("Arial", 14)); // Larger font for better readability

    // Draw table headers with more padding and thicker font
    int xPos = margin;
    QStringList headers = {"Type", "Quantité en Stock", "Quantité Minimal", "ID", "Quantité Après Ajout"};
    for (const QString &header : headers) {
        painter.setFont(QFont("Arial", 14, QFont::Bold)); // Make header text bold
        painter.drawText(xPos, yPos, colWidth, rowHeight, Qt::AlignCenter, header);
        xPos += colWidth;
    }

    // Draw a line under the headers to separate it from the content
    painter.setPen(QPen(Qt::black, 2)); // Line thickness
    painter.drawLine(margin, yPos + rowHeight, pdfWriter.width() - margin, yPos + rowHeight); // Header separator
    yPos += rowHeight + 10; // Add space after header separator

    // Get model data and draw table rows
    QAbstractItemModel *model = ui->tableView25->model();
    for (int row = 0; row < model->rowCount(); ++row) {
        xPos = margin;

        // Draw each cell with padding inside the cells (left and right padding)
        for (int col = 0; col < model->columnCount(); ++col) {
            QString cellText = model->data(model->index(row, col)).toString();
            painter.drawText(xPos + 10, yPos, colWidth - 20, rowHeight, Qt::AlignLeft, cellText); // 10 px padding
            xPos += colWidth;
        }

        // Draw a horizontal line after each row to separate the lines more clearly
        painter.setPen(QPen(Qt::black, 1)); // Thinner line for row separation
        painter.drawLine(margin, yPos + rowHeight, pdfWriter.width() - margin, yPos + rowHeight); // Row separator

        yPos += rowHeight + 10; // Add extra space between rows for clearer separation

        // Check if we need a new page and add extra buffer
        if (yPos > pdfWriter.height() - margin - rowHeight) {
            pdfWriter.newPage();
            yPos = 90; // Reset y position for new page
            painter.setFont(QFont("Arial", 14)); // Reset font if needed
        }
    }

    painter.end();
    QMessageBox::information(this, "PDF Saved", "The PDF was successfully saved to " + filePath);
}



void MainWindow::rechercherParQuantiteEnStock() {
    // Get the quantity from the lineEdit_search
    QString quantiteEnStock = ui->lineEdit_search->text();

    if (quantiteEnStock.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer une quantité.");
        return;
    }

    // Call the search method from hygiene class with the entered quantity
    QSqlQueryModel *model = Etmp.rechercherParQuantite(quantiteEnStock);

    // Set the model to the table view
    ui->tableView25->setModel(model);
}
void MainWindow::on_pushButton_trier_clicked() {
    hygiene h;
    QSqlQueryModel *model = h.trierParQuantite();  // Sort records by QUANTITEENSTOCKH
    ui->tableView25->setModel(model);
}
void MainWindow::on_pushButton_trier_2_clicked()
{
    hygiene h;
    QSqlQueryModel *model = h.trierParTypeDeProduit();  // Sort records by TYPEDEPRODUITH alphabetically
    ui->tableView25->setModel(model);
}


void MainWindow::on_pushButton_statistique_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
    // Create the chart
    QChart *chart = new QChart();

    QSqlQuery query("SELECT TYPEDEPRODUITH, QUANTITEENSTOCKH FROM GS_HYGIENE");
    QBarSeries *series = new QBarSeries();
    QBarSet *set = new QBarSet("Quantité en stock");
    QStringList productTypes;

    while (query.next()) {
        QString typeDeProduit = query.value(0).toString();
        int quantiteEnStock = query.value(1).toInt();

        *set << quantiteEnStock;
        productTypes << typeDeProduit;
    }

    series->append(set);
    chart->addSeries(series);
    chart->setTitle("Quantité en stock par produit");
    chart->createDefaultAxes();

    QCategoryAxis *axisX = new QCategoryAxis();
    for (int i = 0; i < productTypes.size(); ++i) {
        axisX->append(productTypes[i], i);
    }
    chart->setAxisX(axisX, series);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 6000);  // Adjust based on your data
    chart->setAxisY(axisY, series);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Remove the existing tableView and replace it with the chart
    ui->tableView->setParent(nullptr);  // Detach from layout
    ui->tableView->hide();              // Optionally hide it

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(chartView);       // Add chart to the layout
    ui->centralWidget->setLayout(layout);

}

 void MainWindow::on_page1Button_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}
void MainWindow::on_page2Button_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}
void MainWindow::on_page3Button_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}
void MainWindow::on_page4Button_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}
void MainWindow::on_page5Button_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}
void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_pushButton_95_clicked()
{
    ui->stackedWidget->setCurrentIndex(11);
}


void MainWindow::on_pushButton_2_clicked()
{
    QString mail = ui->lineEdit->text();
    QString titre = ui->lineEdit_2->text();
    QString typeproduit = ui->comboBox->currentText();
    QSqlQuery query;
    QString body;
    query.prepare("SELECT TYPEDEPRODUITH, QUANTITEENSTOCKH, QUANTITEMINIMALH, ID_PRODUITH, QUANTITEAPRESAJOUTH "
                  "FROM GS_HYGIENE WHERE TYPEDEPRODUITH = :typeProduit");
    query.bindValue(":typeProduit", typeproduit);
    if (!query.exec()) {
        qDebug() << "SQL Error: " << query.lastError().text();
        return;
    }
    if (query.next()) {
        // Retrieve data into variables
        QString typedeproduith = query.value("TYPEDEPRODUITH").toString();
        QString quantiteEnStockH = query.value("QUANTITEENSTOCKH").toString();
        QString quantiteMinimalH = query.value("QUANTITEMINIMALH").toString();
        QString idProduitH = query.value("ID_PRODUITH").toString();
        QString quantiteApresAjoutH = query.value("QUANTITEAPRESAJOUTH").toString();
        body = "type : "+typedeproduith+" quantieenstock : "+quantiteEnStockH+" quantiteminimal : "+quantiteMinimalH+ " id produit : "+idProduitH+" quantite apres ajout : "+quantiteApresAjoutH;
        qDebug() << body;
    }

    smtpp* ssmtp = new smtpp("rayen.sboui@esprit.tn", "pyco xtxb zqcw xqfp", "smtp.gmail.com", 465);

    connect(ssmtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString)));


    ssmtp->sendMail("rayen.sboui00@esprit.tn", mail , titre,body);
    QMessageBox::information(nullptr, QObject::tr("EMAIL"),
                             QObject::tr("Email Envoyé avec succees.\n"
                                         "click Cancel to exit"),QMessageBox::Cancel);
}

void MainWindow::mailSent(QString status)
{

    if(status == "Message sent")
        QMessageBox::warning( 0, tr( "Qt Simple SMTP client" ), tr( "Message sent!\n\n" ) );
}
void MainWindow::box()
{
    QSqlQuery query;
    query.prepare("SELECT DISTINCT typedeproduith FROM gs_hygiene");

    if (!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
        return;
    }

    // Populate the combo box with the results
    while (query.next()) {
        QString typeDeProduit = query.value(0).toString();
        ui->comboBox->addItem(typeDeProduit);
    }

}
    void MainWindow::on_pushButton_retour_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}
void MainWindow::on_pushButton_barcode_clicked()
{
    ui->stackedWidget->setCurrentIndex(10);
}



void MainWindow::on_pushButton_generateBarcode_clicked()
{
    QString barcodeData = m_selectedId;

    // Validate the barcode data
    if (barcodeData.isEmpty()) {
        QMessageBox::warning(this, "Error", "Barcode data is empty!");
        return;
    }

    // Get QLabel dimensions
    int labelWidth = ui->label_barcode->width();
    int labelHeight = ui->label_barcode->height();

    // Dynamically calculate a very thick bar width
    const double barWidthRatio = 0.8; // Use 80% of QLabel width for bars
    int barWidth = static_cast<int>(labelWidth * barWidthRatio / barcodeData.length());
    if (barWidth < 50) barWidth = 50; // Minimum width for visibility

    // Allocate 20% of QLabel height for numbers
    int textHeight = labelHeight / 5;
    int barcodeHeight = labelHeight - textHeight;

    // Calculate total barcode width based on the calculated bar width
    int barcodeWidth = barWidth * barcodeData.length();

    // Create a QImage that matches QLabel size
    QImage barcodeImage(barcodeWidth, labelHeight, QImage::Format_RGB888);
    barcodeImage.fill(Qt::white);

    QPainter painter(&barcodeImage);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    QPen pen(Qt::black);
    painter.setPen(pen);

    QFont font("Arial", textHeight / 2, QFont::Bold); // Bold font for large numbers
    painter.setFont(font);

    // Start drawing bars from the leftmost position
    int xPos = 0;
    for (int i = 0; i < barcodeData.length(); ++i) {
        QChar c = barcodeData[i];
        if (c.isDigit()) {
            // Draw an ultra-thick bar
            painter.drawRect(xPos, 0, barWidth, barcodeHeight);

            // Draw the corresponding number below the bar
            QString text = QString(c);
            int textWidth = painter.fontMetrics().horizontalAdvance(text);
            painter.drawText(xPos + (barWidth - textWidth) / 2, barcodeHeight + textHeight - 10, text);
        }
        xPos += barWidth; // Move to the next bar position
    }

    painter.end();

    // Set the barcode image to the QLabel
    ui->label_barcode->setPixmap(QPixmap::fromImage(barcodeImage).scaled(ui->label_barcode->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->label_barcode->setAlignment(Qt::AlignCenter);
}




void MainWindow::on_pushButton_retour2_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}
void MainWindow::on_pushButton_retour3_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}
void MainWindow::on_pushButton_arduino_clicked()
{
    ui->stackedWidget->setCurrentIndex(8);
}
void MainWindow::on_pushButton_retour5_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}



void MainWindow::on_readFromArduino()
{
    QByteArray data = serial->readAll();
    QString response = QString::fromUtf8(data);

    // Print received data for debugging
    qDebug() << "Received from Arduino: " << response;

    // Trim the response to remove unwanted characters (\r\n)
    response = response.trimmed();

    // Check if the data starts with "ID:" and parse the ID correctly
    if (response.startsWith("ID:")) {
        QString clientId = response.mid(3); // Extract ID after "ID:"

        // Debug print to see if the clientId is parsed correctly
        qDebug() << "Client ID: " << clientId;

        // Call the database check function
        on_checkClient(clientId);
    } else {
        // Handle unexpected data format or errors
        qDebug() << "Invalid data received: " << response;
    }
}


void MainWindow::on_checkClient(const QString &clientId)
{
    QSqlQuery query;
    query.prepare("SELECT ID_PRODUITH FROM GS_HYGIENE WHERE ID_PRODUITH=:clientId");
    query.bindValue(":clientId", clientId);
    query.exec();

    if (query.next()) { // Client found
        grantAccess(clientId);
    } else { // Client not found
        denyAccess(clientId);
    }
}


void MainWindow::grantAccess(const QString &clientId)
{
    QString entryTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    // Insert entry log in database
    QSqlQuery query;
    query.prepare("INSERT INTO LOGS (CLIENT_ID, ENTRY_TIME) VALUES (:clientId, TO_TIMESTAMP(:entryTime, 'YYYY-MM-DD HH24:MI:SS'))");
    query.bindValue(":clientId", clientId);
    query.bindValue(":entryTime", entryTime);

    if (query.exec()) {
        ui->logTextEdit->append("Access granted for produit: " + clientId + " at " + entryTime);
        ui->statusLabel->setText("Access Granted");

        // Send command to Arduino to open door (Grant access)
        qDebug() << "Sending GRANT_ACCESS to Arduino";

        serial->write("GRANT_ACCESS");

    } else {
        QString errorMsg = query.lastError().text();
        ui->logTextEdit->append("Database Error: Unable to log entry for client: " + clientId + " - " + errorMsg);
    }
}


void MainWindow::denyAccess(const QString &clientId)
{
    ui->logTextEdit->append("Access Denied for produit: " + clientId);
    ui->statusLabel->setText("Access Denied");
    qDebug() << "Sending DENY_ACCESS to Arduino";

    // Send command to Arduino to deny access
    serial->write("DENY_ACCESS");

}
