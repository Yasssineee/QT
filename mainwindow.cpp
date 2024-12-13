#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>
#include <QCryptographicHash>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

class QRCodeGenerator {
public:
    static QImage generateQR(const QString& data, int size) {
        // Create a white background image
        QImage image(size, size, QImage::Format_RGB32);
        image.fill(Qt::white);
        
        QPainter painter(&image);
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::black);
        
        // Generate a simple pattern based on data
        int cellSize = size / 25; // 25x25 grid
        QByteArray hash = QCryptographicHash::hash(data.toUtf8(), QCryptographicHash::Md5);
        
        for(int i = 0; i < 25; i++) {
            for(int j = 0; j < 25; j++) {
                // Use hash to determine if cell should be black
                if((hash[i % 16] >> (j % 8)) & 1) {
                    painter.drawRect(i * cellSize, j * cellSize, cellSize, cellSize);
                }
            }
        }
        
        // Add position detection patterns
        painter.setBrush(Qt::black);
        // Top-left
        painter.drawRect(0, 0, cellSize * 7, cellSize * 7);
        painter.setBrush(Qt::white);
        painter.drawRect(cellSize, cellSize, cellSize * 5, cellSize * 5);
        painter.setBrush(Qt::black);
        painter.drawRect(cellSize * 2, cellSize * 2, cellSize * 3, cellSize * 3);
        
        // Top-right
        painter.setBrush(Qt::black);
        painter.drawRect(size - cellSize * 7, 0, cellSize * 7, cellSize * 7);
        painter.setBrush(Qt::white);
        painter.drawRect(size - cellSize * 6, cellSize, cellSize * 5, cellSize * 5);
        painter.setBrush(Qt::black);
        painter.drawRect(size - cellSize * 5, cellSize * 2, cellSize * 3, cellSize * 3);
        
        // Bottom-left
        painter.setBrush(Qt::black);
        painter.drawRect(0, size - cellSize * 7, cellSize * 7, cellSize * 7);
        painter.setBrush(Qt::white);
        painter.drawRect(cellSize, size - cellSize * 6, cellSize * 5, cellSize * 5);
        painter.setBrush(Qt::black);
        painter.drawRect(cellSize * 2, size - cellSize * 5, cellSize * 3, cellSize * 3);
        
        return image;
    }
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), translator(nullptr) {
    ui->setupUi(this);
    
    // Initialize database tables
    createRecycleBinTable();
    
    // Load data
    displayTable();
    
    // Connect signals
    connect(ui->tableView_6->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &MainWindow::onTableViewSelectionChanged);
}

MainWindow::~MainWindow() {
    delete ui;
    if (translator != nullptr) {
        delete translator;
    }
}

void MainWindow::on_pushButton_279_clicked() {
    int code = ui->lineEdit_4->text().toInt();
    QString prd = ui->lineEdit_3->text();
    int qttmin = ui->lineEdit->text().toInt();
    int qtt = ui->lineEdit_2->text().toInt();

    // Capitalize first letter of product name
    if (!prd.isEmpty()) {
        prd = prd.at(0).toUpper() + prd.mid(1).toLower();
    }

    currentAlimentaire.setCode(code);
    currentAlimentaire.setPrd(prd);
    currentAlimentaire.setQttmin(qttmin);
    currentAlimentaire.setQtt(qtt);

    if (currentAlimentaire.add()) {
        QMessageBox::information(this, "Success", "Item added successfully.");
        displayTable();
    } else {
        QMessageBox::critical(this, "Error", "Failed to add item.");
    }
}

void MainWindow::on_pushButton_281_clicked() {
    QTableView *table = ui->tableView_6;
    if (!table->selectionModel()->hasSelection()) {
        QMessageBox::warning(this, "Warning", "Please select an item to delete!");
        return;
    }

    int row = table->currentIndex().row();
    
    // Get all the necessary data from the table
    QModelIndex codeIndex = table->model()->index(row, 0);
    QModelIndex qttIndex = table->model()->index(row, 1);
    QModelIndex qttminIndex = table->model()->index(row, 2);
    QModelIndex prdIndex = table->model()->index(row, 3);
    
    if (!codeIndex.isValid() || !qttIndex.isValid() || !qttminIndex.isValid() || !prdIndex.isValid()) {
        QMessageBox::warning(this, "Error", "Failed to get item data from table!");
        return;
    }
    
    int code = table->model()->data(codeIndex).toInt();
    int qtt = table->model()->data(qttIndex).toInt();
    int qttmin = table->model()->data(qttminIndex).toInt();
    QString prd = table->model()->data(prdIndex).toString();
    
    if (code <= 0) {
        QMessageBox::warning(this, "Error", "Invalid item code!");
        return;
    }
    
    if (QMessageBox::question(this, "Confirm Delete",
                            "Are you sure you want to move " + prd + " to the recycle bin?",
                            QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        
        // Create Alimentaire object with current values
        Alimentaire item(code, qtt, qttmin, prd);
        
        // Move to recycle bin instead of permanent deletion
        if (moveToRecycleBin(item)) {
            QMessageBox::information(this, "Success", "Item moved to recycle bin successfully!");
            displayTable();
        } else {
            QMessageBox::critical(this, "Error", "Failed to move item to recycle bin! Check the application output for details.");
        }
    }
}

void MainWindow::on_pushButton_280_clicked() {
    int code = ui->lineEdit_4->text().toInt();
    QString prd = ui->lineEdit_3->text();
    int qttmin = ui->lineEdit->text().toInt();
    int qtt = ui->lineEdit_2->text().toInt();

    // Capitalize first letter of product name
    if (!prd.isEmpty()) {
        prd = prd.at(0).toUpper() + prd.mid(1).toLower();
    }

    currentAlimentaire.setCode(code);
    currentAlimentaire.setPrd(prd);
    currentAlimentaire.setQttmin(qttmin);
    currentAlimentaire.setQtt(qtt);

    if (currentAlimentaire.update()) {
        QMessageBox::information(this, "Success", "Item updated successfully.");
        displayTable();
    } else {
        QMessageBox::critical(this, "Error", "Failed to update item.");
    }
}

void MainWindow::on_pushButton_278_clicked() {
    ui->lineEdit_4->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
}

void MainWindow::on_pushButton_282_clicked()
{
    // Create a printer object
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPageSize::A4);

    // Open file dialog to choose where to save the PDF
    QString fileName = QFileDialog::getSaveFileName(this, "Export PDF", QString(), "*.pdf");
    if (fileName.isEmpty())
        return;

    if (!fileName.endsWith(".pdf"))
        fileName += ".pdf";

    printer.setOutputFileName(fileName);

    // Create painter to paint on printer device
    QPainter painter;
    if (!painter.begin(&printer)) {
        QMessageBox::critical(this, "Error", "Cannot open PDF file for writing!");
        return;
    }

    // Get the model from tableView
    QAbstractItemModel* model = ui->tableView_6->model();
    if (!model) {
        painter.end();
        return;
    }

    // Set up page parameters
    int rows = model->rowCount();
    int cols = model->columnCount();
    QRectF pageRect = printer.pageRect(QPrinter::Point); // Specify the unit
    double cellWidth = pageRect.width() / cols;
    double cellHeight = 40;
    double yPosition = 0;

    // Draw header
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 12, QFont::Bold));
    for (int column = 0; column < cols; column++) {
        QRectF cell(column * cellWidth, yPosition, cellWidth, cellHeight);
        painter.drawText(cell, Qt::AlignCenter, model->headerData(column, Qt::Horizontal).toString());
        painter.drawRect(cell);
    }
    yPosition += cellHeight;

    // Draw data
    painter.setFont(QFont("Arial", 10));
    for (int row = 0; row < rows; row++) {
        // Check if we need a new page
        if (yPosition + cellHeight > pageRect.height()) { // Use stored pageRect
            printer.newPage();
            yPosition = 0;
        }

        for (int column = 0; column < cols; column++) {
            QRectF cell(column * cellWidth, yPosition, cellWidth, cellHeight);
            painter.drawText(cell, Qt::AlignCenter, model->index(row, column).data().toString());
            painter.drawRect(cell);
        }
        yPosition += cellHeight;
    }

    painter.end();

    // Open the generated PDF
    QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
    QMessageBox::information(this, "Success", "PDF has been generated and opened!");
}

void MainWindow::displayTable() {
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT * FROM TABLEALIMENTATION"); // Ensure this query is correct for your table
    if (query.exec()) {
        model->setQuery(std::move(query));
        ui->tableView_6->setModel(model);
    } else {
        QMessageBox::critical(this, "Error", "Failed to load data into table.");
    }

    // Optional: Clear selection after refresh
    ui->tableView_6->clearSelection();
    
    updateStockChart();
    
    checkLowStock();
}

QString MainWindow::generateProductQRData(int code, const QString& prd, int qtt, int qttmin) {
    return QString("Code: %1\nProduct: %2\nQuantity: %3\nMin Quantity: %4")
        .arg(code)
        .arg(prd)
        .arg(qtt)
        .arg(qttmin);
}

void MainWindow::showQRCode(const QString& data) {
    // Create network manager for API request
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    
    // Create URL with parameters
    QUrl url("https://api.qrserver.com/v1/create-qr-code/");
    QUrlQuery query;
    query.addQueryItem("size", "300x300");
    query.addQueryItem("data", data);
    url.setQuery(query);
    
    // Create request
    QNetworkRequest request(url);
    
    // Send GET request
    QNetworkReply* reply = manager->get(request);
    
    // Connect to handle response
    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            // Read the image data
            QByteArray imageData = reply->readAll();
            QImage qrImage;
            qrImage.loadFromData(imageData);
            
            // Create dialog to display QR code
            QDialog* dialog = new QDialog(this);
            dialog->setWindowTitle("QR Code");
            
            // Create layout
            QVBoxLayout* layout = new QVBoxLayout(dialog);
            
            // Create and set up image label
            QLabel* imageLabel = new QLabel(dialog);
            imageLabel->setPixmap(QPixmap::fromImage(qrImage));
            layout->addWidget(imageLabel);
            
            // Add text label with encoded data
            QLabel* dataLabel = new QLabel(data, dialog);
            dataLabel->setWordWrap(true);
            layout->addWidget(dataLabel);
            
            // Show dialog
            dialog->setLayout(layout);
            dialog->exec();
            
            delete dialog;
        } else {
            QMessageBox::critical(this, "Error", "Failed to generate QR code: " + reply->errorString());
        }
        
        reply->deleteLater();
        manager->deleteLater();
    });
}

void MainWindow::on_pushButton_287_clicked() {
    // Get the currently selected row
    QModelIndex currentIndex = ui->tableView_6->selectionModel()->currentIndex();
    
    if (currentIndex.isValid()) {
        // Get data from the selected row
        int code = currentIndex.sibling(currentIndex.row(), 0).data().toInt();
        QString prd = currentIndex.sibling(currentIndex.row(), 3).data().toString();
        int qtt = currentIndex.sibling(currentIndex.row(), 1).data().toInt();
        int qttmin = currentIndex.sibling(currentIndex.row(), 2).data().toInt();
        
        // Generate QR code data
        QString qrData = generateProductQRData(code, prd, qtt, qttmin);
        
        // Show QR code
        showQRCode(qrData);
    } else {
        // Get the model from tableView_6
        QAbstractItemModel* model = ui->tableView_6->model();
        QString tableData = "INVENTORY LIST\n==================\n\n";
        
        if (model) {
            // Add column headers
            tableData += "Code | Product | Quantity | Min Quantity\n";
            tableData += "----------------------------------------\n";
            
            // Add all rows from the table
            for (int row = 0; row < model->rowCount(); ++row) {
                QString code = model->index(row, 0).data().toString();
                QString quantity = model->index(row, 1).data().toString();
                QString minQuantity = model->index(row, 2).data().toString();
                QString product = model->index(row, 3).data().toString();
                
                tableData += QString("%1 | %2 | %3 | %4\n")
                    .arg(code, -4)
                    .arg(product, -15)
                    .arg(quantity, -8)
                    .arg(minQuantity, -8);
            }
        }
        
        // Show QR code with all table data
        showQRCode(tableData);
    }
}

void MainWindow::onTableViewSelectionChanged() {
    QModelIndex currentIndex = ui->tableView_6->selectionModel()->currentIndex();
    if (!currentIndex.isValid()) return;

    // Get data from the selected row
    int selectedCode = currentIndex.sibling(currentIndex.row(), 0).data().toInt();
    ui->lineEdit_4->setText(QString::number(selectedCode));
    
    // Offer to generate QR code if clicked on product name (column 3)
    if (currentIndex.column() == 3) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this, 
            "Generate QR Code",
            "Would you like to generate a QR code for this product?",
            QMessageBox::Yes | QMessageBox::No
        );
        
        if (reply == QMessageBox::Yes) {
            on_pushButton_287_clicked();
        }
    }
}

void MainWindow::on_pushButton_286_clicked()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    
    // Use PRD instead of PRODUCT for the column name
    query.prepare("SELECT * FROM \"TABLEALIMENTATION\" ORDER BY \"PRD\" ASC");
    
    if (query.exec()) {
        model->setQuery(std::move(query));
        ui->tableView_6->setModel(model);
        ui->tableView_6->resizeColumnsToContents();
        
        // Show success message
        int rowCount = model->rowCount();
        QString message = QString("Products sorted alphabetically.\nTotal products: %1").arg(rowCount);
        QMessageBox::information(this, "Sort Complete", message);
    } else {
        QMessageBox::critical(this, "Error", 
            "Failed to sort data: " + query.lastError().text());
    }
}

void MainWindow::on_pushButton_305_clicked()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    
    // Query that calculates qtt-qttmin and sorts by it
    QString query = "SELECT CODE, PRD, QTT, QTTMIN, (QTT - QTTMIN) as DIFFERENCE "
                   "FROM WIEM.TABLEALIMENTATION "
                   "ORDER BY (QTT - QTTMIN) ASC";
    
    model->setQuery(query);
    
    if (model->lastError().isValid()) {
        QMessageBox::warning(this, "Error", "Failed to sort items: " + model->lastError().text());
        return;
    }
    
    // Set headers
    model->setHeaderData(0, Qt::Horizontal, tr("Code"));
    model->setHeaderData(1, Qt::Horizontal, tr("Product"));
    model->setHeaderData(2, Qt::Horizontal, tr("Quantity"));
    model->setHeaderData(3, Qt::Horizontal, tr("Min Quantity"));
    model->setHeaderData(4, Qt::Horizontal, tr("Difference"));
    
    // Apply to table view
    ui->tableView_6->setModel(model);
    ui->tableView_6->resizeColumnsToContents();
    
    // Highlight rows where quantity is below minimum
    for(int row = 0; row < model->rowCount(); row++) {
        int difference = model->data(model->index(row, 4)).toInt();
        if (difference < 0) {
            // If you want to highlight entire rows that need attention
            for(int col = 0; col < model->columnCount(); col++) {
                QModelIndex index = model->index(row, col);
                model->setData(index, QColor(255, 200, 200), Qt::BackgroundRole);
            }
        }
    }
    
    QMessageBox::information(this, "Success", "Products sorted by stock level difference (Quantity - Minimum Quantity)");
}

bool MainWindow::moveToRecycleBin(const Alimentaire& item)
{
    QSqlDatabase::database().transaction();
    
    try {
        // Insert into recycle bin
        QSqlQuery insertQuery;
        insertQuery.prepare("INSERT INTO WIEM.RB (CODE_RB, PRD_RB, QTT_RB, QTTMIN_RB, DURRÉE) "
                          "VALUES (:code, :prd, :qtt, :qttmin, SYSDATE)");
        
        insertQuery.bindValue(":code", item.getCode());
        insertQuery.bindValue(":prd", item.getPrd());
        insertQuery.bindValue(":qtt", item.getQtt());
        insertQuery.bindValue(":qttmin", item.getQttmin());
        
        if (!insertQuery.exec()) {
            QMessageBox::warning(this, "Error", "Failed to move item to recycle bin");
            QSqlDatabase::database().rollback();
            return false;
        }
        
        // Delete from main table
        QSqlQuery deleteQuery;
        deleteQuery.prepare("DELETE FROM WIEM.TABLEALIMENTATION WHERE CODE = :code");
        deleteQuery.bindValue(":code", item.getCode());
        
        if (!deleteQuery.exec()) {
            QMessageBox::warning(this, "Error", "Failed to delete item from main table");
            QSqlDatabase::database().rollback();
            return false;
        }
        
        QSqlDatabase::database().commit();
        QMessageBox::information(this, "Success", "Item moved to recycle bin");
        displayTable();
        updateStockChart();
        return true;
        
    } catch (...) {
        QSqlDatabase::database().rollback();
        QMessageBox::critical(this, "Error", "An unexpected error occurred");
        return false;
    }
}

void MainWindow::displayRecycleBin()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT CODE_RB \"Code\", PRD_RB \"Product\", QTT_RB \"Quantity\", "
                   "QTTMIN_RB \"Min Quantity\", DURRÉE \"Deleted Date\" FROM WIEM.RB");
    
    if (model->lastError().isValid()) {
        QMessageBox::warning(this, "Error", "Failed to display recycle bin: " + model->lastError().text());
        return;
    }
    
    // Set up a new table view for recycle bin
    QTableView *recycleBinView = new QTableView(this);
    recycleBinView->setModel(model);
    recycleBinView->setSelectionBehavior(QAbstractItemView::SelectRows);
    recycleBinView->setSelectionMode(QAbstractItemView::SingleSelection);
    recycleBinView->resizeColumnsToContents();
    
    // Create restore and empty buttons
    QPushButton *restoreButton = new QPushButton("Restore Selected", this);
    QPushButton *emptyButton = new QPushButton("Empty Recycle Bin", this);
    
    connect(restoreButton, &QPushButton::clicked, [this, recycleBinView, model]() {
        QModelIndex index = recycleBinView->currentIndex();
        if (index.isValid()) {
            int code = model->data(model->index(index.row(), 0)).toInt();
            if (restoreFromRecycleBin(code)) {
                displayRecycleBin();  // Refresh the view
            }
        }
    });
    
    connect(emptyButton, &QPushButton::clicked, this, &MainWindow::emptyRecycleBin);
    
    // Create layout for the dialog
    QDialog *dialog = new QDialog(this);
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->addWidget(recycleBinView);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(restoreButton);
    buttonLayout->addWidget(emptyButton);
    layout->addLayout(buttonLayout);
    
    dialog->setWindowTitle("Recycle Bin");
    dialog->resize(600, 400);
    dialog->exec();
}

bool MainWindow::restoreFromRecycleBin(int code)
{
    QSqlDatabase::database().transaction();
    
    try {
        // First retrieve the item data from recycle bin
        QSqlQuery selectQuery;
        selectQuery.prepare("SELECT CODE_RB, PRD_RB, QTT_RB, QTTMIN_RB FROM WIEM.RB WHERE CODE_RB = :code");
        selectQuery.bindValue(":code", code);
        
        if (!selectQuery.exec() || !selectQuery.next()) {
            QMessageBox::warning(this, "Error", "Failed to retrieve item from recycle bin");
            QSqlDatabase::database().rollback();
            return false;
        }
        
        // Insert back into main table
        QSqlQuery insertQuery;
        insertQuery.prepare("INSERT INTO WIEM.TABLEALIMENTATION (CODE, PRD, QTT, QTTMIN) "
                          "VALUES (:code, :prd, :qtt, :qttmin)");
        
        insertQuery.bindValue(":code", selectQuery.value(0).toInt());
        insertQuery.bindValue(":prd", selectQuery.value(1).toString());
        insertQuery.bindValue(":qtt", selectQuery.value(2).toInt());
        insertQuery.bindValue(":qttmin", selectQuery.value(3).toInt());
        
        if (!insertQuery.exec()) {
            QMessageBox::warning(this, "Error", "Failed to restore item");
            QSqlDatabase::database().rollback();
            return false;
        }
        
        // Delete from recycle bin
        QSqlQuery deleteQuery;
        deleteQuery.prepare("DELETE FROM WIEM.RB WHERE CODE_RB = :code");
        deleteQuery.bindValue(":code", code);
        
        if (!deleteQuery.exec()) {
            QMessageBox::warning(this, "Error", "Failed to remove item from recycle bin");
            QSqlDatabase::database().rollback();
            return false;
        }
        
        QSqlDatabase::database().commit();
        QMessageBox::information(this, "Success", "Item restored successfully");
        displayTable();
        updateStockChart();
        
        return true;
    } catch (...) {
        QSqlDatabase::database().rollback();
        QMessageBox::critical(this, "Error", "An unexpected error occurred");
        return false;
    }
}

void MainWindow::emptyRecycleBin()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm", "Are you sure you want to permanently delete all items in the recycle bin?",
                                QMessageBox::Yes|QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        QSqlQuery query;
        if (!query.exec("DELETE FROM WIEM.RB")) {
            QMessageBox::warning(this, "Error", "Failed to empty recycle bin");
            return;
        }
        
        QMessageBox::information(this, "Success", "Recycle bin emptied successfully");
        displayRecycleBin();  // Refresh the recycle bin view
    }
}

void MainWindow::on_pushButton_284_clicked()
{
    displayRecycleBin();
}

void MainWindow::createRecycleBinTable() {
    QSqlQuery query;
    
    // First check if the table exists
    query.exec("SELECT table_name FROM user_tables WHERE table_name = 'RB'");
    if (query.next()) {
        // Table exists, drop it
        if (!query.exec("DROP TABLE RB")) {
            qDebug() << "Failed to drop existing table:" << query.lastError().text();
            return;
        }
    }
    
    // Create the table with Oracle compatible syntax
    QString createTableQuery = 
        "CREATE TABLE RB ("
        "    CODE_RB NUMBER(10) PRIMARY KEY,"
        "    PRD_RB VARCHAR2(50),"
        "    QTT_RB NUMBER(10),"
        "    QTTMIN_RB NUMBER(10),"
        "    DURRÉE DATE"
        ")";
        
    if (!query.exec(createTableQuery)) {
        QString errorMsg = "Failed to create recycle bin table: " + query.lastError().text();
        qDebug() << errorMsg;
        QMessageBox::critical(this, "Database Error", errorMsg);
        return;
    }
    
    qDebug() << "Recycle bin table created successfully";
}

void MainWindow::on_pushButton_clicked()
{
    // Create a new dialog for the restock list
    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("Products Needing Restock");
    dialog->setMinimumWidth(600);
    dialog->setMinimumHeight(400);

    // Create a table widget to display the data
    QTableWidget* table = new QTableWidget(dialog);
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels({"Code", "Product", "Current Qty", "Min Qty", "Qty to Add"});

    // Query for products below minimum quantity
    QSqlQuery query;
    query.prepare("SELECT CODE, PRD, QTT, QTTMIN, (QTTMIN - QTT) as NEEDED "
                 "FROM WIEM.TABLEALIMENTATION "
                 "WHERE QTT < QTTMIN "
                 "ORDER BY (QTTMIN - QTT) DESC");

    if (!query.exec()) {
        QMessageBox::warning(this, "Error", "Failed to retrieve restock information");
        return;
    }

    // Count total products and total quantity needed
    int totalProducts = 0;
    int totalQuantityNeeded = 0;

    // Populate the table
    while (query.next()) {
        int currentRow = table->rowCount();
        table->insertRow(currentRow);

        // Get values
        int code = query.value(0).toInt();
        QString product = query.value(1).toString();
        int currentQty = query.value(2).toInt();
        int minQty = query.value(3).toInt();
        int neededQty = query.value(4).toInt();

        // Add values to table
        table->setItem(currentRow, 0, new QTableWidgetItem(QString::number(code)));
        table->setItem(currentRow, 1, new QTableWidgetItem(product));
        table->setItem(currentRow, 2, new QTableWidgetItem(QString::number(currentQty)));
        table->setItem(currentRow, 3, new QTableWidgetItem(QString::number(minQty)));
        table->setItem(currentRow, 4, new QTableWidgetItem(QString::number(neededQty)));

        // Update totals
        totalProducts++;
        totalQuantityNeeded += neededQty;

        // Highlight rows based on urgency
        QColor rowColor;
        if (currentQty == 0) {
            rowColor = QColor(255, 200, 200); // Red for out of stock
        } else if (currentQty < (minQty / 2)) {
            rowColor = QColor(255, 229, 200); // Orange for critically low
        } else {
            rowColor = QColor(255, 255, 200); // Yellow for low
        }

        // Apply color to all cells in the row
        for (int col = 0; col < table->columnCount(); col++) {
            if (table->item(currentRow, col)) {
                table->item(currentRow, col)->setBackground(rowColor);
            }
        }
    }

    // Create summary label
    QLabel* summaryLabel = new QLabel(dialog);
    summaryLabel->setText(QString("Total Products Needing Restock: %1\nTotal Quantity to Order: %2")
                         .arg(totalProducts)
                         .arg(totalQuantityNeeded));
    summaryLabel->setStyleSheet("QLabel { font-weight: bold; padding: 10px; }");

    // Create export button
    QPushButton* exportButton = new QPushButton("Export to CSV", dialog);
    connect(exportButton, &QPushButton::clicked, [table]() {
        QString fileName = QFileDialog::getSaveFileName(nullptr, "Save Restock List",
                                                      QDir::homePath() + "/restock_list.csv",
                                                      "CSV Files (*.csv)");
        if (!fileName.isEmpty()) {
            QFile file(fileName);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream stream(&file);
                
                // Write headers
                QStringList headers;
                for (int col = 0; col < table->columnCount(); col++) {
                    headers << table->horizontalHeaderItem(col)->text();
                }
                stream << headers.join(",") << "\n";
                
                // Write data
                for (int row = 0; row < table->rowCount(); row++) {
                    QStringList rowData;
                    for (int col = 0; col < table->columnCount(); col++) {
                        rowData << table->item(row, col)->text();
                    }
                    stream << rowData.join(",") << "\n";
                }
                
                file.close();
                QMessageBox::information(nullptr, "Success", "Restock list exported successfully!");
            }
        }
    });

    // Create layout
    QVBoxLayout* layout = new QVBoxLayout(dialog);
    layout->addWidget(summaryLabel);
    layout->addWidget(table);
    layout->addWidget(exportButton);

    // Set table properties
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->resizeColumnsToContents();
    table->setAlternatingRowColors(true);

    // Show the dialog
    dialog->exec();
}

void MainWindow::on_pushButton_56_clicked()
{
    QString searchText = ui->lineEdit_127->text().trimmed();
    QSqlQueryModel *model = new QSqlQueryModel();
    
    // Prepare the search query
    QString query;
    if (searchText.isEmpty()) {
        // If search is empty, show all records
        query = "SELECT * FROM TABLEALIMENTATION";
    } else {
        // Search in all columns using Oracle compatible syntax
        query = QString("SELECT * FROM TABLEALIMENTATION WHERE "
                      "TO_CHAR(code) LIKE '%%%1%%' OR "
                      "LOWER(prd) LIKE '%%%2%%' OR "
                      "TO_CHAR(qtt) LIKE '%%%1%%' OR "
                      "TO_CHAR(qttmin) LIKE '%%%1%%'")
                      .arg(searchText)
                      .arg(searchText.toLower());
    }
    
    model->setQuery(query);
    
    if (model->lastError().isValid()) {
        QMessageBox::warning(this, "Search Error", 
            "Failed to perform search: " + model->lastError().text());
        return;
    }
    
    // Set column headers
    model->setHeaderData(0, Qt::Horizontal, tr("Code"));
    model->setHeaderData(1, Qt::Horizontal, tr("Product"));
    model->setHeaderData(2, Qt::Horizontal, tr("Quantity"));
    model->setHeaderData(3, Qt::Horizontal, tr("Min Quantity"));
    
    // Set model to table view
    ui->tableView_6->setModel(model);
    ui->tableView_6->resizeColumnsToContents();
    
    // Show number of results found
    int resultCount = model->rowCount();
    QString message = QString("%1 result%2 found")
                     .arg(resultCount)
                     .arg(resultCount != 1 ? "s" : "");
    QMessageBox::information(this, "Search Results", message);
}

void MainWindow::updateStockChart()
{
    // Create query to get counts
    QSqlQuery query;
    int overStock = 0;
    int underStock = 0;
    
    // Count products over minimum stock
    query.exec("SELECT COUNT(*) FROM TABLEALIMENTATION WHERE (qtt - qttmin) > 0");
    if (query.next()) {
        overStock = query.value(0).toInt();
    }
    
    // Count products under minimum stock
    query.exec("SELECT COUNT(*) FROM TABLEALIMENTATION WHERE (qtt - qttmin) < 0");
    if (query.next()) {
        underStock = query.value(0).toInt();
    }
    
    // Create pie series
    QPieSeries *series = new QPieSeries();
    
    // Add slices
    QPieSlice *overStockSlice = series->append("Above", overStock);
    QPieSlice *underStockSlice = series->append("Below", underStock);
    
    // Set colors
    overStockSlice->setColor(QColor(46, 204, 113));  // Green
    underStockSlice->setColor(QColor(231, 76, 60));  // Red
    
    // Set labels with percentages
    double totalProducts = overStock + underStock;
    double overPercent = (overStock / totalProducts) * 100;
    double underPercent = (underStock / totalProducts) * 100;
    
    overStockSlice->setLabel(QString("Above: %1 (%2%)").arg(overStock).arg(QString::number(overPercent, 'f', 1)));
    underStockSlice->setLabel(QString("Below: %1 (%2%)").arg(underStock).arg(QString::number(underPercent, 'f', 1)));
    
    // Make slices pop out slightly
    overStockSlice->setExploded();
    underStockSlice->setExploded();
    overStockSlice->setExplodeDistanceFactor(0.1);
    underStockSlice->setExplodeDistanceFactor(0.1);
    
    // Create chart
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Stock Level Statistics");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    
    // Style the chart
    chart->setBackgroundBrush(QBrush(Qt::white));
    chart->setTitleBrush(QBrush(Qt::black));
    chart->setTitleFont(QFont("Arial", 12, QFont::Bold));
    
    // Show legend
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);
    
    // Create chart view
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    
    // Clear existing layout and add new chart
    if (ui->graphicsView->layout()) {
        delete ui->graphicsView->layout();
    }
    QVBoxLayout *layout = new QVBoxLayout(ui->graphicsView);
    layout->addWidget(chartView);
    ui->graphicsView->setLayout(layout);
    
    // Update title with total
    chart->setTitle(QString("Stock Level Statistics\nTotal Products: %1").arg(overStock + underStock));
}

void MainWindow::checkLowStock()
{
    QSqlQuery query;
    query.prepare("SELECT CODE, PRD, QTT, QTTMIN FROM WIEM.TABLEALIMENTATION WHERE QTT < QTTMIN");
    
    if (!query.exec()) {
        QMessageBox::warning(this, "Error", "Failed to check low stock items");
        return;
    }

    QStringList lowStockProducts;
    while (query.next()) {
        QString productName = query.value(1).toString();
        int currentQty = query.value(2).toInt();
        int minQty = query.value(3).toInt();
        lowStockProducts.append(QString("%1 (Current: %2, Minimum: %3)")
                              .arg(productName)
                              .arg(currentQty)
                              .arg(minQty));
    }

    if (!lowStockProducts.isEmpty()) {
        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("Low Stock Alert!");
        msgBox.setText("The following products are below minimum quantity:");
        msgBox.setInformativeText(lowStockProducts.join("\n"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        
        // Style the message box
        msgBox.setStyleSheet("QMessageBox { background-color: #ffebee; }"
                           "QMessageBox QLabel { color: #c62828; }");
        
        // Show as modal dialog
        msgBox.exec();
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    // Create main help dialog
    QDialog* helpDialog = new QDialog(this);
    helpDialog->setWindowTitle("Alimentaire Management System - Help Assistant");
    helpDialog->setMinimumSize(800, 600);
    
    // Create tab widget for different help sections
    QTabWidget* tabWidget = new QTabWidget(helpDialog);
    
    // Overview Tab
    QWidget* overviewTab = new QWidget();
    QVBoxLayout* overviewLayout = new QVBoxLayout(overviewTab);
    QLabel* overviewLabel = new QLabel(
        "<h2>Welcome to Alimentaire Management System</h2>"
        "<p>This system helps you manage your inventory efficiently with features for tracking, "
        "monitoring, and maintaining your product stock levels.</p>"
        "<h3>Main Features:</h3>"
        "<ul>"
        "<li>Product Management (Add, Edit, Delete)</li>"
        "<li>Stock Level Monitoring</li>"
        "<li>Low Stock Alerts</li>"
        "<li>Recycle Bin</li>"
        "<li>Search and Sort Functions</li>"
        "<li>Stock Statistics</li>"
        "</ul>"
    );
    overviewLabel->setWordWrap(true);
    overviewLayout->addWidget(overviewLabel);
    tabWidget->addTab(overviewTab, "Overview");
    
    // Product Management Tab
    QWidget* productTab = new QWidget();
    QVBoxLayout* productLayout = new QVBoxLayout(productTab);
    QLabel* productLabel = new QLabel(
        "<h3>Managing Products</h3>"
        "<p><b>Adding Products:</b></p>"
        "<ul>"
        "<li>Fill in the product details (Code, Name, Quantity, Minimum Quantity)</li>"
        "<li>Click 'Add' to insert the product</li>"
        "</ul>"
        "<p><b>Editing Products:</b></p>"
        "<ul>"
        "<li>Select the product from the table</li>"
        "<li>Modify the values in the input fields</li>"
        "<li>Click 'Update' to save changes</li>"
        "</ul>"
        "<p><b>Deleting Products:</b></p>"
        "<ul>"
        "<li>Select the product to delete</li>"
        "<li>Click 'Delete' - item will be moved to Recycle Bin</li>"
        "<li>Can be restored from Recycle Bin if needed</li>"
        "</ul>"
    );
    productLabel->setWordWrap(true);
    productLayout->addWidget(productLabel);
    tabWidget->addTab(productTab, "Product Management");
    
    // Stock Management Tab
    QWidget* stockTab = new QWidget();
    QVBoxLayout* stockLayout = new QVBoxLayout(stockTab);
    QLabel* stockLabel = new QLabel(
        "<h3>Stock Management</h3>"
        "<p><b>Low Stock Alerts:</b></p>"
        "<ul>"
        "<li>Automatic alerts when products fall below minimum quantity</li>"
        "<li>Red highlighting for critical stock levels</li>"
        "<li>Click 'View Restock List' to see all products needing restock</li>"
        "</ul>"
        "<p><b>Stock Statistics:</b></p>"
        "<ul>"
        "<li>Pie chart shows proportion of products above/below minimum stock</li>"
        "<li>Click 'Sort by Stock Level' to view products ordered by stock status</li>"
        "</ul>"
        "<p><b>Restock List:</b></p>"
        "<ul>"
        "<li>Shows products needing restock</li>"
        "<li>Calculates exact quantities needed</li>"
        "<li>Export feature for ordering</li>"
        "</ul>"
    );
    stockLabel->setWordWrap(true);
    stockLayout->addWidget(stockLabel);
    tabWidget->addTab(stockTab, "Stock Management");
    
    // Search and Sort Tab
    QWidget* searchTab = new QWidget();
    QVBoxLayout* searchLayout = new QVBoxLayout(searchTab);
    QLabel* searchLabel = new QLabel(
        "<h3>Search and Sort Features</h3>"
        "<p><b>Search Function:</b></p>"
        "<ul>"
        "<li>Enter search terms in the search box</li>"
        "<li>Searches across all product fields</li>"
        "<li>Case-insensitive matching</li>"
        "</ul>"
        "<p><b>Sorting Options:</b></p>"
        "<ul>"
        "<li>Sort by stock level difference</li>"
        "<li>Sort by product code</li>"
        "<li>Sort by quantity</li>"
        "</ul>"
    );
    searchLabel->setWordWrap(true);
    searchLayout->addWidget(searchLabel);
    tabWidget->addTab(searchTab, "Search and Sort");
    
    // Recycle Bin Tab
    QWidget* recycleTab = new QWidget();
    QVBoxLayout* recycleLayout = new QVBoxLayout(recycleTab);
    QLabel* recycleLabel = new QLabel(
        "<h3>Recycle Bin Features</h3>"
        "<p><b>Using the Recycle Bin:</b></p>"
        "<ul>"
        "<li>Deleted items are moved to Recycle Bin</li>"
        "<li>Click 'Show Recycle Bin' to view deleted items</li>"
        "<li>Select items and click 'Restore' to recover them</li>"
        "<li>'Empty Recycle Bin' permanently removes all items</li>"
        "</ul>"
        "<p><b>Safety Features:</b></p>"
        "<ul>"
        "<li>Confirmation dialogs for permanent deletion</li>"
        "<li>Tracks deletion dates</li>"
        "<li>Prevents accidental data loss</li>"
        "</ul>"
    );
    recycleLabel->setWordWrap(true);
    recycleLayout->addWidget(recycleLabel);
    tabWidget->addTab(recycleTab, "Recycle Bin");
    
    // Shortcuts Tab
    QWidget* shortcutsTab = new QWidget();
    QVBoxLayout* shortcutsLayout = new QVBoxLayout(shortcutsTab);
    QLabel* shortcutsLabel = new QLabel(
        "<h3>Quick Tips and Shortcuts</h3>"
        "<p><b>Common Tasks:</b></p>"
        "<ul>"
        "<li><b>Quick Search:</b> Type directly in search box</li>"
        "<li><b>Stock Check:</b> Click product row to view details</li>"
        "<li><b>Quick Update:</b> Double-click values to edit</li>"
        "<li><b>Export Data:</b> Available in restock list view</li>"
        "</ul>"
        "<p><b>Best Practices:</b></p>"
        "<ul>"
        "<li>Regularly check the low stock alerts</li>"
        "<li>Review restock list before ordering</li>"
        "<li>Empty recycle bin periodically</li>"
        "<li>Keep minimum quantities updated</li>"
        "</ul>"
    );
    shortcutsLabel->setWordWrap(true);
    shortcutsLayout->addWidget(shortcutsLabel);
    tabWidget->addTab(shortcutsTab, "Quick Tips");
    
    // Style the help dialog
    helpDialog->setStyleSheet(
        "QDialog { background-color: #f5f5f5; }"
        "QTabWidget::pane { border: 1px solid #cccccc; }"
        "QTabBar::tab { background-color: #e0e0e0; padding: 8px 20px; }"
        "QTabBar::tab:selected { background-color: #ffffff; }"
        "QLabel { font-size: 11pt; line-height: 1.4; }"
        "QLabel h2 { color: #2196F3; }"
        "QLabel h3 { color: #1976D2; }"
        "QLabel b { color: #1565C0; }"
    );
    
    // Create main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(helpDialog);
    mainLayout->addWidget(tabWidget);
    
    // Add close button at bottom
    QPushButton* closeButton = new QPushButton("Close Help", helpDialog);
    closeButton->setStyleSheet(
        "QPushButton { background-color: #2196F3; color: white; padding: 8px 20px; border-radius: 4px; }"
        "QPushButton:hover { background-color: #1976D2; }"
    );
    connect(closeButton, &QPushButton::clicked, helpDialog, &QDialog::accept);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);
    mainLayout->addLayout(buttonLayout);
    
    // Show the dialog
    helpDialog->exec();
    delete helpDialog;
}

void MainWindow::loadLanguage(const QString& language)
{
    // Store current language selection
    QSettings settings("YourCompany", "Alimentaire");
    settings.setValue("language", language);

    // Update UI based on language
    if (language == "fr") {
        // French translations
        ui->pushButton_279->setText("Ajouter");
        ui->pushButton_280->setText("Modifier");
        ui->pushButton_281->setText("Supprimer");
        ui->pushButton_56->setText("Rechercher");
        ui->pushButton_284->setText("Corbeille");
        ui->pushButton_2->setText("Aide");
        ui->pushButton_3->setText("Langue");
        
        // Update labels
        ui->label_427->setText("Code:");
        ui->label_428->setText("Produit:");
        ui->label_429->setText("Quantité:");
        ui->label_430->setText("Quantité Minimale:");
        
        // Update table headers
        if (QTableView* table = ui->tableView_6) {
            if (QAbstractItemModel* model = table->model()) {
                model->setHeaderData(0, Qt::Horizontal, "Code");
                model->setHeaderData(1, Qt::Horizontal, "Produit");
                model->setHeaderData(2, Qt::Horizontal, "Quantité");
                model->setHeaderData(3, Qt::Horizontal, "Quantité Minimale");
            }
        }
        
        setWindowTitle("Système de Gestion Alimentaire");
        
    } else if (language == "ar") {
        // Arabic translations
        ui->pushButton_279->setText("إضافة");
        ui->pushButton_280->setText("تعديل");
        ui->pushButton_281->setText("حذف");
        ui->pushButton_56->setText("بحث");
        ui->pushButton_284->setText("سلة المحذوفات");
        ui->pushButton_2->setText("مساعدة");
        ui->pushButton_3->setText("اللغة");
        
        // Update labels
        ui->label_427->setText("الرمز:");
        ui->label_428->setText("المنتج:");
        ui->label_429->setText("الكمية:");
        ui->label_430->setText("الحد الأدنى للكمية:");
        
        // Update table headers
        if (QTableView* table = ui->tableView_6) {
            if (QAbstractItemModel* model = table->model()) {
                model->setHeaderData(0, Qt::Horizontal, "الرمز");
                model->setHeaderData(1, Qt::Horizontal, "المنتج");
                model->setHeaderData(2, Qt::Horizontal, "الكمية");
                model->setHeaderData(3, Qt::Horizontal, "الحد الأدنى");
            }
        }
        
        setWindowTitle("نظام إدارة المواد الغذائية");
        
        // Set RTL layout
        qApp->setLayoutDirection(Qt::RightToLeft);
        
    } else {
        // English (default) translations
        ui->pushButton_279->setText("Add");
        ui->pushButton_280->setText("Update");
        ui->pushButton_281->setText("Delete");
        ui->pushButton_56->setText("Search");
        ui->pushButton_284->setText("Recycle Bin");
        ui->pushButton_2->setText("Help");
        ui->pushButton_3->setText("Language");
        
        // Update labels
        ui->label_427->setText("Code:");
        ui->label_428->setText("Product:");
        ui->label_429->setText("Quantity:");
        ui->label_430->setText("Minimum Quantity:");
        
        // Update table headers
        if (QTableView* table = ui->tableView_6) {
            if (QAbstractItemModel* model = table->model()) {
                model->setHeaderData(0, Qt::Horizontal, "Code");
                model->setHeaderData(1, Qt::Horizontal, "Product");
                model->setHeaderData(2, Qt::Horizontal, "Quantity");
                model->setHeaderData(3, Qt::Horizontal, "Minimum Quantity");
            }
        }
        
        setWindowTitle("Alimentaire Management System");
        
        // Set LTR layout
        qApp->setLayoutDirection(Qt::LeftToRight);
    }
    
    // Refresh the table view
    if (QTableView* table = ui->tableView_6) {
        table->reset();
        table->resizeColumnsToContents();
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    QDialog* langDialog = new QDialog(this);
    langDialog->setWindowTitle("Select Language - Choisir la langue - اختر اللغة");
    langDialog->setMinimumWidth(300);
    
    QVBoxLayout* layout = new QVBoxLayout(langDialog);
    
    // Create language buttons with flags
    QPushButton* englishBtn = new QPushButton("🇬🇧 English", langDialog);
    QPushButton* frenchBtn = new QPushButton("🇫🇷 Français", langDialog);
    QPushButton* arabicBtn = new QPushButton("🇹🇳 العربية", langDialog);
    
    // Style the buttons
    QString buttonStyle = "QPushButton { padding: 15px; margin: 5px; border-radius: 5px; font-size: 14px; }"
                         "QPushButton:hover { background-color: #e0e0e0; }";
    
    englishBtn->setStyleSheet(buttonStyle);
    frenchBtn->setStyleSheet(buttonStyle);
    arabicBtn->setStyleSheet(buttonStyle);
    
    // Add buttons to layout
    layout->addWidget(englishBtn);
    layout->addWidget(frenchBtn);
    layout->addWidget(arabicBtn);
    
    // Connect buttons
    connect(englishBtn, &QPushButton::clicked, [this, langDialog]() {
        loadLanguage("en");
        langDialog->accept();
    });
    
    connect(frenchBtn, &QPushButton::clicked, [this, langDialog]() {
        loadLanguage("fr");
        langDialog->accept();
    });
    
    connect(arabicBtn, &QPushButton::clicked, [this, langDialog]() {
        loadLanguage("ar");
        langDialog->accept();
    });
    
    // Show dialog
    langDialog->exec();
    delete langDialog;
}
