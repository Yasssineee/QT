#include "mainwindow.h"

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



  //employee

#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QString>


#include <QFileDialog>
#include <QTextDocument>
#include<QStringList>
#include <QListWidget>
#include <QMimeData>

#include <QDateTime>
#include <QProgressBar>

#include <QPrinter>
#include <QPainter>

/*
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
*/
//using namespace QtCharts;

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>
#include <QByteArray>
#include <QJsonArray>

#include <QCamera>
//#include <QImageCapture>
//#include <QMediaCaptureSession>
#include <QPixmap>



#include <QAction>
#include <QVBoxLayout>

#include <QKeyEvent>

#include <QProcess>

#include <QFileDialog>

  //hygiene
#include "hygiene.h"
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
#include <QSerialPort>
#include <QSerialPortInfo>

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
//chambre
#include "chambre.h" // Include the client header
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
    , serial(new QSerialPort(this))
    , isUserLoggedIn(false)

{
    ui->setupUi(this);
ui->stackedWidget->setCurrentIndex(18);
    // Initialisation des composants

    int verif_arduino = A.connect_arduino();
           switch(verif_arduino){
           case(0):qDebug()<<"arduino is available and connected to :" << A.getArduinoPortName();
               break;
           case(1):qDebug()<<"arduino is available but not connected to :" << A.getArduinoPortName();
               break;
           case(-1):qDebug()<<"arduino is not available" ;
               break;
           }

    QObject::connect(A.getSerial(), SIGNAL(readyRead()), this, SLOT(update_label()));

        QString savedPath = "C:\\Users\\jellali\\Desktop\\cropped_face.jpg";
        QPixmap savedPixmap(savedPath);
        if (!savedPixmap.isNull()) {
            ui->imageLabel_6->setPixmap(savedPixmap);
            ui->imageLabel_6->setScaledContents(true);
        }

        //QString emotion = readEmotionFromFile("C:/Users/jellali/Desktop/emotion_log.txt");
       // ui->emotionLabel_3->setText("Emotion détectée : " + emotion);



        ui->comboBox_37->addItem("Sort by Name (Alphabetical)", "name");
        ui->comboBox_37->addItem("Sort by Salary (Highest First)", "salary");



        Connection conn; // Create an instance of your Connection class
        if (conn.createconnect()) {
            QMessageBox::information(this, "Success de connexion", "success de la connexion à la base de données.");
        } else {
            QMessageBox::critical(this, "Database is not open", "Connection failed.");
        }



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



        connect(ui->gostat, &QPushButton::clicked, this, [this]() { navigateToPage(4); });
        connect(ui->back, &QPushButton::clicked, this, [this]() { navigateToPage(3); });
        connect(ui->pdf, &QPushButton::clicked, this, &MainWindow::exportToPDF);
        connect(ui->excel, &QPushButton::clicked, this, &MainWindow::exportToExcel);
        connect(ui->search_bar, &QLineEdit::textChanged, this, &MainWindow::Rechercherclient);
        connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_sortComboBox_currentIndexChanged(int)));
        connect(ui->EmotionDetection_3, &QPushButton::clicked, this, &MainWindow::onEmotionDetection_3Clicked);


        connect(ui->page3Button_4, &QPushButton::clicked, this, &MainWindow::on_page3Button_4_clicked);

        // Connect other buttons as needed


        ui->tableView_4->setModel(Etmp.afficher());
//hygiene
        ui->tableView25->setModel(Etmp2.afficher());
        this->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
        connect(ui->pushButton_pdf, &QPushButton::clicked, this, &MainWindow::exportToPDF2);
        connect(ui->pushButton_search, &QPushButton::clicked, this, &MainWindow::rechercherParQuantiteEnStock);
        connect(ui->pushButton_generateBarcode, &QPushButton::clicked, this, &MainWindow::on_pushButton_generateBarcode_clicked);

//cha,bre
        //QStackedWidget* stackedWidget = ui->stackedWidget;
        //QWidget* page = stackedWidget->widget(1);
        QTableView* tableView_3 = page->findChild<QTableView*>("tableView_3");

        if (tableView_3) {

            chambre chambre;
            Connection c;

            if (c.createconnect()) {
                QMessageBox::information(nullptr, QObject::tr("Database is open"),
                                         QObject::tr("Connection successful.\nClick Cancel to exit."),
                                         QMessageBox::Cancel);

                tableView_3->setModel(chambre.Afficherchambres());
                tableView_3->resizeColumnsToContents();
                tableView_3->horizontalHeader()->setStretchLastSection(true);




                QPalette palette;
                palette.setColor(QPalette::Background,QColor("#00FF9C"));


                tableView_3->setItemDelegateForColumn(7, new BackgroundColorDelegate(palette));


                QPalette palette2;
                palette2.setColor(QPalette::Background,QColor("#FF4545"));


                tableView_3->setItemDelegateForColumn(8, new BackgroundColorDelegate(palette2));






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


        connect(ui->search_bar_2, &QLineEdit::textChanged, this, &MainWindow::Rechercherchambre);

        connect(ui->comboBox_5, SIGNAL(currentIndexChanged(int)), this, SLOT(on_sortComboBox_currentIndexChanged2(int)));

        connect(ui->pdf_2, &QPushButton::clicked, this, &MainWindow::exportToPDF3);

        connect(ui->gostat_2, &QPushButton::clicked, this, [this]() { navigateToPage(17); });
        connect(ui->backto, &QPushButton::clicked, this, [this]() { navigateToPage(1); });
    }








void MainWindow::update_label() {

    data += A.readFromArduino();
    qDebug() << "Données reçues d'Arduino :" << data;

    int newlineIndex = data.indexOf('\n');
    if (newlineIndex != -1) {
        QString message = data.left(newlineIndex);
        data.remove(0, newlineIndex + 1);


        if (message.startsWith("CMD:")) {
                   QString commandId = message.mid(4);
                   QSqlQuery query;
                   query.prepare("SELECT name FROM command WHERE id = :id");
                   query.bindValue(":id", commandId.toInt());

                   if (query.exec() && query.next()) {
                       QString commandName = query.value(0).toString();
                         A.write_arduino((commandName + "\n").toUtf8());
                       qDebug() << "Command received: " << commandName;
                   } else {
                       qDebug() << "Command ID not found in database.";
                   }
               }
        else if (message.startsWith("TEMP:")) {
            QString tempValue = message.mid(5);
            bool conversionOk = false;
            float temp = tempValue.toFloat(&conversionOk);

            if (conversionOk) {
                ui->temp->setText(QString::number(temp, 'f', 2));


                QSqlQuery query;
                query.prepare("SELECT min, max FROM temp");
                if (query.exec() && query.next()) {
                    float minTemp = query.value(0).toFloat();
                    float maxTemp = query.value(1).toFloat();

                    // Compare temperature with the limits
                    if (temp > maxTemp) {
                        A.write_arduino("HOT\n");
                        //QMessageBox::warning(this, "Temperature Alert", "The temperature exceeds the maximum limit! It's HOT!");
                        qDebug() << "Temperature exceeds maximum. HOT signal sent.";

                    } else if (temp < minTemp) {
                        A.write_arduino("COLD\n");
                        //QMessageBox::warning(this, "Temperature Alert", "The temperature is below the minimum limit! It's COLD!");
                        qDebug() << "Temperature below minimum. COLD signal sent.";

                    }
                    else{

                    }
                } else {
                    qDebug() << "Failed to retrieve minimaum and maximaum from temp table:" << query.lastError().text();
                }
            } else {
                qDebug() << "Invalid temperature value received.";
            }
        }

        else if (message.startsWith("DIST:")) {
            QString distValue = message.mid(5);
            bool conversionOk = false;
            int distance = distValue.toFloat(&conversionOk);

            if (conversionOk) {

                QSqlQuery query;
                query.prepare("SELECT etat FROM movement");
                if (query.exec() && query.next()) {
                    int currentEtat = query.value(0).toInt();

                    int newEtat = (distance < 20) ? 1 : 0;

                    if (newEtat != currentEtat) {

                        QSqlQuery updateQuery;
                        updateQuery.prepare("UPDATE movement SET etat = :etat");
                        updateQuery.bindValue(":etat", newEtat);

                        if (updateQuery.exec()) {
                            qDebug() << "Updated movement table successfully. New etat:" << newEtat;
                            if (newEtat == 1) {

                                A.write_arduino("LED_ON\n");
                                qDebug() << "LED turned ON.";
                                QMessageBox::information(this, "Person Detection", "Person detected! LED turned ON.");
                            } else {

                                A.write_arduino("LED_OFF\n");
                                qDebug() << "LED turned OFF.";
                                QMessageBox::information(this, "Person Detection", "No person detected! LED turned OFF.");
                            }
                        } else {
                            qDebug() << "Failed to update movement table:" << updateQuery.lastError().text();
                        }
                    } else {
                        qDebug() << "No change in etat. Current etat:" << currentEtat;
                    }
                } else {
                    qDebug() << "Failed to retrieve etat from movement table:" << query.lastError().text();
                }
            } else {
                qDebug() << "Invalid distance value received.";
            }
        } else {
            qDebug() << "Unknown message type received: " << message;
        }
    }
}







        //employee


void MainWindow::onEmotionDetection_3Clicked()
{
    QString pythonPath = "C:/Users/jellali/AppData/Local/Programs/Python/Python310/python.exe";
    QString scriptPath = "C:/Users/jellali/Pictures/emotion_recognition.py";

    QProcess *process = new QProcess(this);

    connect(process, &QProcess::readyReadStandardError, [process]() {
        qDebug() << "Erreur standard : " << process->readAllStandardError();
    });

    connect(process, &QProcess::readyReadStandardOutput, [process]() {
        qDebug() << "Sortie standard : " << process->readAllStandardOutput();
    });

    process->start(pythonPath, QStringList() << scriptPath);

    if (!process->waitForStarted()) {
        qDebug() << "Erreur : " << process->errorString();
        return;
    }

    process->waitForFinished();

  //  QString emotion = readEmotionFromFile("C:/Users/jellali/Desktop/emotion_log.txt");
    //ui->emotionLabel_3->setText("Emotion détectée : " + emotion);

    QPixmap pixmap("C:/Users/jellali/Desktop/cropped_face.jpg");
    ui->imageLabel_6->setPixmap(pixmap);
    ui->imageLabel_6->setScaledContents(true);
} // Fermeture de la fonction


void MainWindow::on_add_btn_4_clicked() {
    qDebug() << "Signal d'ajout déclenché"; // Affiche à chaque fois que le signal est émis

    // Vérification si le bouton est déjà désactivé, cela évite un double clic rapide
    if (!ui->add_btn_4->isEnabled()) {
        qDebug() << "Le bouton est déjà désactivé, exit.";
        return;  // Sortie prématurée si le bouton est désactivé
    }

    ui->add_btn_4->setEnabled(false);  // Désactive le bouton pour éviter les clics multiples

    // Récupération des données de l'interface
    int id = ui->lineEdit_68->text().toInt();
    QString nom = ui->lineEdit_67->text();
    QString prenom = ui->lineEdit_66->text();
    QString mail = ui->lineEdit_email_4->text();
    QString dateembauche = ui->lineEdit_date_4->text();
    QString certificats = ui->lineEdit_certificats_4->text();
    QString diplomes = ui->lineEdit_diplomes_4->text();
    QString salaire = ui->lineEdit_salaire_5->text();
    QString assurance = ui->lineEdit_assurance_4->text();

    // Vérification que les champs ne sont pas vides
    if (nom.isEmpty() || prenom.isEmpty() || mail.isEmpty() || dateembauche.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs obligatoires.");
        ui->add_btn_4->setEnabled(true); // Réactive le bouton si un champ est vide
        return; // Sortir si les champs sont vides
    }

    // Création de l'objet employee
    employee e(id, nom, prenom, mail, dateembauche, certificats, diplomes, salaire, assurance);

    // Ajout à la base de données
    bool test = e.ajouter();

    // Mise à jour de la vue et message d'information
    if (test) {
        QMessageBox::information(this, QObject::tr("ok"), "Ajout effectué avec succès.");
    } else {
        QMessageBox::critical(this, QObject::tr("not ok"), "Ajout non effectué.");
    }

    ui->add_btn_4->setEnabled(true); // Réactive le bouton après l'ajout
    qDebug() << "Fin de l'ajout"; // Affiche ce message à la fin de l'exécution du slot


}



void MainWindow::on_delete_btn_4_clicked() {
    QModelIndexList selection = ui->tableView_4->selectionModel()->selectedRows();

    if (!selection.isEmpty()) {
        int row = selection.at(0).row();
        QModelIndex idIndex = ui->tableView_4->model()->index(row, 0);

        int id = idIndex.data().toInt(); // Récupérer l'ID de l'employé à supprimer

        // Stocker les données supprimées
        QMap<QString, QVariant> entry;
        entry["id"] = id;
        entry["nom"] = ui->tableView_4->model()->index(row, 1).data().toString();
        entry["prenom"] = ui->tableView_4->model()->index(row, 2).data().toString();
        entry["email"] = ui->tableView_4->model()->index(row, 3).data().toString(); // Ajoutez l'email
        entry["dateembauche"] = ui->tableView_4->model()->index(row, 4).data().toString(); // Ajoutez la date d'embauche
        entry["certificats"] = ui->tableView_4->model()->index(row, 5).data().toString(); // Ajoutez les certificats
        entry["diplomes"] = ui->tableView_4->model()->index(row, 6).data().toString(); // Ajoutez les diplômes
        entry["salaire"] = ui->tableView_4->model()->index(row, 7).data().toDouble(); // Ajoutez le salaire
        entry["assurance"] = ui->tableView_4->model()->index(row, 8).data().toString(); // Ajoutez l'assurance
        deletedEntries.append(entry);

        // Supprimer l'employé
        bool test = Etmp.supprimer(id);

        // Mise à jour de la vue et message d'information
        if (test) {
            ui->tableView_4->setModel(Etmp.afficher());
            QMessageBox::information(this, QObject::tr("ok"), QObject::tr("Suppression effectuée"), QMessageBox::Cancel);
        } else {
            QMessageBox::critical(this, QObject::tr("not ok"), QObject::tr("Suppression non effectuée"), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::warning(this, "Erreur", "Aucune ligne sélectionnée pour la suppression.");
    }
}







void MainWindow::on_pushButton_export_pdf_clicked()
{
    // Ouvrir une boîte de dialogue pour choisir l'emplacement de sauvegarde du PDF
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en PDF", QString(), "*.pdf");
    if (fileName.isEmpty())
        return;
    if (QFileInfo(fileName).suffix().isEmpty()) {
        fileName.append(".pdf"); // Ajouter l'extension si elle n'est pas précisée
    }

    // Créer un objet QPrinter configuré pour le mode PDF
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    // Commencer à dessiner dans le PDF
    QPainter painter(&printer);
    int rowCount = ui->tableView_4->model()->rowCount();
    int columnCount = ui->tableView_4->model()->columnCount();

    // Dessiner l'en-tête du tableau
    int yPos = 100;
    for (int column = 0; column < columnCount; column++) {
        QString headerText = ui->tableView_4->model()->headerData(column, Qt::Horizontal).toString();
        painter.drawText(column * 100, yPos, headerText);
    }

    // Dessiner les lignes du tableau
    yPos += 50;
    for (int row = 0; row < rowCount; row++) {
        for (int column = 0; column < columnCount; column++) {
            QModelIndex index = ui->tableView_4->model()->index(row, column);
            QString data = index.data().toString();
            painter.drawText(column * 100, yPos, data);
        }
        yPos += 50; // Déplacer la position verticale pour la prochaine ligne
    }

    painter.end(); // Terminer le dessin dans le PDF

    QMessageBox::information(this, "Succès", "Les données ont été exportées avec succès en PDF.");
}


void MainWindow::addEmployeeInfoToChat()
{
    for (int row = 0; row < employeeModel->rowCount(); ++row) {
        QString employeeInfo = QString("ID: %1, Nom: %2, Prénom: %3")
                                   .arg(employeeModel->data(employeeModel->index(row, 0)).toString()) // ID
                                   .arg(employeeModel->data(employeeModel->index(row, 1)).toString()) // Nom
                                   .arg(employeeModel->data(employeeModel->index(row, 2)).toString()); // Prénom

        ui->textEdit_chat_4->append(employeeInfo);
    }
}



void MainWindow::on_buttonRechercher_clicked() {
    QString nomRecherche = ui->lineEdit_67->text(); // Obtenir le texte du champ de saisie
    employee emp; // Assurez-vous d'avoir un constructeur sans argument dans `employee`
    QSqlQueryModel *model = emp.rechercherParNom(nomRecherche); // Appel de la méthode


    if (model) {
        ui->tableView_4->setModel(model); // Affiche le modèle de résultat dans tableView
    } else {
        qDebug() << "Aucun résultat trouvé ou erreur dans la requête.";
    }
}


void MainWindow::afficherTousLesEmployes() {
    QSqlQueryModel *model = new QSqlQueryModel(this);
    QSqlQuery query("SELECT * FROM TABLE4"); // Assurez-vous que 'employees' est le nom correct de votre table


        model->setQuery(std::move(query));
        ui->tableView_4->setModel(model); // Met à jour la vue avec le modèle

}


void MainWindow::trierNomsEmployes() {
    QSqlQueryModel *model = new QSqlQueryModel(this);
    QSqlQuery query("SELECT * FROM TABLE4 ORDER BY NOM ASC"); // Assurez-vous que 'nom' est le bon nom de colonne


        model->setQuery(std::move(query));
        ui->tableView_4->setModel(model); // Met à jour la vue avec le modèle

}



void MainWindow::onTableViewClicked(const QModelIndex &index) {
    // Get the row number
    int row = index.row();

    // Retrieve data from the selected row and set it to the corresponding input fields
    ui->lineEdit_68->setText(ui->tableView_4->model()->index(row, 0).data().toString()); // ID
    ui->lineEdit_67->setText(ui->tableView_4->model()->index(row, 1).data().toString()); // Nom
    ui->lineEdit_66->setText(ui->tableView_4->model()->index(row, 2).data().toString()); // Prenom
    ui->lineEdit_email_4->setText(ui->tableView_4->model()->index(row, 3).data().toString()); // Mail
    ui->lineEdit_date_4->setText(ui->tableView_4->model()->index(row, 4).data().toString()); // Date Embauche
    ui->lineEdit_certificats_4->setText(ui->tableView_4->model()->index(row, 5).data().toString()); // Certificats
    ui->lineEdit_salaire_5->setText(ui->tableView_4->model()->index(row, 6).data().toString()); // Diplomes  //salire
    ui->lineEdit_assurance_4->setText(ui->tableView_4->model()->index(row, 7).data().toString()); // Salaire   //assurance
    ui->lineEdit_diplomes_4->setText(ui->tableView_4->model()->index(row, 8).data().toString()); // Assurance  //diplomes
}


void MainWindow::on_buttonUpdate_3_clicked() {
    // Retrieve data from input fields
    int id = ui->lineEdit_68->text().toInt();
    QString nom = ui->lineEdit_67->text();
    QString prenom = ui->lineEdit_66->text();
    QString mail = ui->lineEdit_email_4->text();
    QString dateembauche = ui->lineEdit_date_4->text();
    QString certificats = ui->lineEdit_certificats_4->text();
    QString diplomes = ui->lineEdit_diplomes_4->text();
    QString salaire = ui->lineEdit_salaire_5->text();
    QString assurance = ui->lineEdit_assurance_4->text();

    // Ensure fields are not empty
    if (nom.isEmpty() || prenom.isEmpty() || mail.isEmpty() || dateembauche.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs obligatoires.");
        return;
    }

    // Create an employee object with the updated details
    employee e(id, nom, prenom, mail, dateembauche, certificats, diplomes, salaire, assurance);

    // Update the employee in the database
    bool success = e.modifier();  // Assuming `modifier` is a method in `employee` class that updates an existing record in the database.

    if (success) {
        QMessageBox::information(this, "Succès", "Mise à jour effectuée avec succès.");
        ui->tableView_4->setModel(Etmp.afficher()); // Refresh the table view
    } else {
        QMessageBox::critical(this, "Erreur", "La mise à jour a échoué.");
    }
}










void MainWindow::RechercherEmploye(const QString &text)
{
    employee e;  // Créez un objet employee
    QSqlQueryModel *model = e.RechercherEmploye(text);  // Appelle la méthode de recherche
    ui->tableView_4->setModel(model);  // Affiche les résultats dans le QTableView
}



void MainWindow::sortEmployees() {
    QString sortCriteria = ui->comboBox_37->currentData().toString();

    QSqlQuery query;
    if (sortCriteria == "name") {
        // Sort by name alphabetically
        query.prepare("SELECT * FROM TABLE4 ORDER BY NOM ASC");
    } /*else if (sortCriteria == "salary") {
        // Sort by salary in descending order
        query.prepare("SELECT * FROM TABLE4 ORDER BY SALAIRE DESC");
    }*/

    if (!query.exec()) {
        qDebug() << "Erreur lors de la récupération des employés triés :" << query.lastError().text();
        return;
    }

    // Create a new model for the sorted data
    QSqlQueryModel *model = new QSqlQueryModel();
   model->setQuery(std::move(query));

    // Set the model on your table view
    ui->tableView_4->setModel(model);

    // Set headers (optional)
    model->setHeaderData(0,Qt::Horizontal,QObject::tr("id"));
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("nom"));
    model->setHeaderData(2,Qt::Horizontal,QObject::tr("prenom"));
    model->setHeaderData(3,Qt::Horizontal,QObject::tr("email"));
    model->setHeaderData(4,Qt::Horizontal,QObject::tr("date embauche"));
    model->setHeaderData(5,Qt::Horizontal,QObject::tr("certificats"));
    model->setHeaderData(6,Qt::Horizontal,QObject::tr("salaire"));
    model->setHeaderData(7,Qt::Horizontal,QObject::tr("assurance"));
    model->setHeaderData(8,Qt::Horizontal,QObject::tr("diplomes"));
}


void MainWindow::resetTable() {
    QSqlQuery query;
    query.prepare("SELECT * FROM TABLE4"); // No ORDER BY clause to fetch unsorted data

    if (!query.exec()) {
        qDebug() << "Erreur lors de la récupération des employés non triés :" << query.lastError().text();
        return;
    }

    // Create a new model for the unsorted data
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(std::move(query));

    // Set the model on your table view
    ui->tableView_4->setModel(model);

    // Set headers (optional)
    model->setHeaderData(0, Qt::Horizontal, tr("Nom"));
    model->setHeaderData(1, Qt::Horizontal, tr("Prénom"));
    model->setHeaderData(2, Qt::Horizontal, tr("ID"));
    model->setHeaderData(3, Qt::Horizontal, tr("Adresse"));
    model->setHeaderData(4, Qt::Horizontal, tr("Genre"));
    model->setHeaderData(5, Qt::Horizontal, tr("Nationalité"));
    model->setHeaderData(6, Qt::Horizontal, tr("Email"));
    model->setHeaderData(7, Qt::Horizontal, tr("Salaire"));

    // Reset the QComboBox to the default option (optional)
    ui->comboBox_37->setCurrentIndex(-1);  // -1 can represent no selection, adjust if needed
}



void MainWindow::on_pushButton_send_4_clicked()
{
    QString question = ui->lineEdit_80->text().trimmed();

    if (question.isEmpty()) {
        return; // Skip empty input
    }

    ui->textEdit_chat_4->append("Vous : " + question);

    // Get the selected version from the combo box
    QString selectedVersion = ui->comboBox->currentText();

    QString response;

    /*if (selectedVersion == "Image Generation") {
        // Image generation logic
        response = "Generating your image...";
        generateImage(question);  // Assume question is the image description
    }*/
   if (selectedVersion == "Smart Employee Response") {
        // Smart employee response logic
        response = respondToQuestion(question);  // Respond intelligently
    }
    else if (selectedVersion == "Learning Mode") {
         //Learning mode logic (same as smart response for now)
       response = respondToQuestion(question);
    }

    ui->textEdit_chat_4->append("Chatbot : " + response);
    ui->lineEdit_80->clear(); // Clear input field
}


QString MainWindow::respondToQuestion(const QString &question) {
    // Logique de réponse ici
    if (question.contains("salut", Qt::CaseInsensitive)) {
        return "Salut! Comment puis-je vous aider?";
    }
    else if (question.contains("acceder", Qt::CaseInsensitive) && question.contains("page employee", Qt::CaseInsensitive)) {
        return "tu peux cliquer sur l'icone employee dans le sidebar  ";
    }
    else if (question.contains("ajouter", Qt::CaseInsensitive) && question.contains(" employee", Qt::CaseInsensitive)) {
        return "tu remplis les lineedit et tu clique surr addbtn  ";
    }
    return "Désolé, je n'ai pas compris la question.";
}





void MainWindow::on_stats_btn_clicked()
{
    QSqlQuery query;
    int withInsurance = 0;
    int withoutInsurance = 0;

    // Récupérer le nombre d'employés avec assurance
    if (query.exec("SELECT COUNT(*) FROM table4 WHERE assurance='oui'")) {
        if (query.next()) {
            withInsurance = query.value(0).toInt();
        }
    } else {
        qDebug() << "Erreur lors de la requête (avec assurance): " << query.lastError().text();
    }

    // Récupérer le nombre d'employés sans assurance
    if (query.exec("SELECT COUNT(*) FROM table4 WHERE assurance='non'")) {
        if (query.next()) {
            withoutInsurance = query.value(0).toInt();
        }
    } else {
        qDebug() << "Erreur lors de la requête (sans assurance): " << query.lastError().text();
    }

    // Calcul du total des employés
    int total = withInsurance + withoutInsurance;

    // Créer un widget pour afficher les statistiques
    QWidget *statWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(statWidget);

    // ProgressBar pour les employés avec assurance
    QProgressBar *progressBarWithInsurance = new QProgressBar();
    progressBarWithInsurance->setRange(0, 100);
    progressBarWithInsurance->setValue((withInsurance * 100) / total);
    progressBarWithInsurance->setTextVisible(true);
    QLabel *labelWithInsurance = new QLabel(QString("Employés avec assurance : %1/%2").arg(withInsurance).arg(total));

    // ProgressBar pour les employés sans assurance
    QProgressBar *progressBarWithoutInsurance = new QProgressBar();
    progressBarWithoutInsurance->setRange(0, 100);
    progressBarWithoutInsurance->setValue((withoutInsurance * 100) / total);
    progressBarWithoutInsurance->setTextVisible(true);
    QLabel *labelWithoutInsurance = new QLabel(QString("Employés sans assurance : %1/%2").arg(withoutInsurance).arg(total));

    // Ajouter les éléments au layout
    layout->addWidget(labelWithInsurance);
    layout->addWidget(progressBarWithInsurance);
    layout->addWidget(labelWithoutInsurance);
    layout->addWidget(progressBarWithoutInsurance);

    // Afficher la fenêtre de statistiques
    statWidget->setLayout(layout);
    statWidget->setWindowTitle("Statistiques des employés");
    statWidget->resize(300, 200);  // Taille de la fenêtre
    statWidget->show();
}


















void MainWindow::on_ajouter_clicked()
{
    QString nom = ui->nom->text();
    QString prenom = ui->prnom->text();
    QString gender = ui->comboBox_9->currentText();
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
            ui->comboBox_9->setCurrentIndex(-1);
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
                    ui->comboBox->setCurrentText(qry.value(2).toString());
                    ui->nbrphone->setText(qry.value(3).toString());
                    ui->adresse->setText(qry.value(4).toString());
                    ui->nationality->setText(qry.value(5).toString());
                    ui->mail->setText(qry.value(6).toString());
                    ui->Date_reser->setDate(qry.value(7).toDate());
                    ui->id->setText(value);
                    ui->stackedWidget->setCurrentIndex(3);
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
    QString gender = ui->comboBox->currentText();
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
        ui->comboBox->setCurrentIndex(-1);
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
    ui->stackedWidget->setCurrentIndex(5);
}

void MainWindow::on_Back_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
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
            ui->tableView25->setModel(Etmp2.afficher());  // Refresh after deletion
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

void MainWindow::exportToPDF2() {
    // Ask user to choose where to save the PDF
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save PDF"), "", tr("PDF Files (*.pdf)"));
    if (filePath.isEmpty()) {
        return; // Cancelled by user
    }

    // Initialize PDF writer
    QPdfWriter pdfWriter(filePath);
    //pdfWriter.setPageSize(QPageSize::A4);
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
    QSqlQueryModel *model = Etmp2.rechercherParQuantite(quantiteEnStock);

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
    ui->stackedWidget->setCurrentIndex(16);
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
    ui->centralwidget->setLayout(layout);

}


void MainWindow::on_pushButton_mail_clicked()
{
    ui->stackedWidget->setCurrentIndex(13);
}

void MainWindow::on_pushButton_retour_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}
void MainWindow::on_pushButton_retourstat_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}
void MainWindow::on_pushButton_retour3_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}
void MainWindow::on_pushButton_retour5_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}
void MainWindow::on_pushButton_barcode_clicked()
{
    ui->stackedWidget->setCurrentIndex(14);
}
void MainWindow::on_pushButton_arduino_clicked()
{
    ui->stackedWidget->setCurrentIndex(15);
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

            // Calculate text width for centering
            QString text = QString(c);
            int textWidth = painter.fontMetrics().width(text); // Utilisation de `width()` pour Qt < 5.11

            // Draw the corresponding number below the bar
            painter.drawText(xPos + (barWidth - textWidth) / 2, barcodeHeight + textHeight - 10, text);
        }
        xPos += barWidth; // Move to the next bar position
    }

    painter.end();

    // Set the barcode image to the QLabel
    ui->label_barcode->setPixmap(QPixmap::fromImage(barcodeImage).scaled(ui->label_barcode->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->label_barcode->setAlignment(Qt::AlignCenter);
}
void MainWindow::connectToArduino() {
    // Initialisation de l'objet QSerialPort
    QSerialPort *serial = new QSerialPort(this);

    // Configuration du port série
    serial->setPortName("COM6"); // Remplace par le port COM de ton Arduino
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    // Tentative de connexion
    if (serial->open(QIODevice::ReadWrite)) {
        ui->statusLabel->setText("Connected to Arduino");
    } else {
        QMessageBox::critical(this, "Error", "Failed to connect to Arduino.");
        delete serial; // Libère la mémoire si la connexion échoue
        return;
    }

    // Connecte le signal readyRead à une fonction de lecture
    connect(serial, &QSerialPort::readyRead, this, &MainWindow::on_readFromArduino);
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

void MainWindow::on_page4Button_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}
void MainWindow::on_page1Button_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}
void MainWindow::on_page3Button_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}
void MainWindow::on_page2Button_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}






MainWindow::~MainWindow() {
    delete ui;  // Libère la mémoire allouée pour l'UI
    m_selectedId = "";

}
void MainWindow::on_ajouter_2_clicked()
{
    QString type_c = ui->comboBox_1->currentText(); // Using QComboBox for type_c
    QString prixStr = ui->prix->text(); // Assuming prix_c is a QLineEdit
    double prix_c = prixStr.toDouble();
    QString vue = ui->comboBox_3->currentText(); // Using QComboBox for vue

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
            ui->tableView_3->setModel(chambre.Afficherchambres());
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
            ui->comboBox_3->setCurrentIndex(-1); // Clear combobox selection
            ui->capacite->clear();
            ui->nbrlits->clear();
        } else {
            QMessageBox::critical(this, tr("Erreur"), tr("Erreur lors de l'ajout de la chambre."), QMessageBox::Cancel);
        }
    }
}
void MainWindow::on_tableView_3_activated(const QModelIndex &index)
{
    if (!index.isValid()) {
        qDebug() << "Invalid index";
        return;
    }
    QString value = ui->tableView_3->model()->data(ui->tableView_3->model()->index(index.row(), 0)).toString();

    Connection c;
    c.closeconnection();
    QSqlQuery qry;

    qry.prepare("SELECT TYPE_C, PRIX_C, ETAT_DIS, VUE, CAPACITE_M, NBRLITS FROM chambre WHERE ID_C = :ID_C");
    qry.bindValue(":ID_C", value);

    if(qry.exec())
    {
        if(qry.next())
        {
            ui->id_2->setText(value);
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
                    ui->comboBox_3->setCurrentText(qry.value(3).toString());
                    ui->capacite->setText(qry.value(4).toString());
                    ui->nbrlits->setText(qry.value(5).toString());
                    ui->stackedWidget->setCurrentIndex(2);
                }
            }
        }
    }

    if (index.column() == 8) {
        QString chambreId = ui->tableView_3->model()->data(ui->tableView_3->model()->index(index.row(), 0)).toString();

        // Confirm the deletion
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Delete Chambre", "Are you sure you want to delete this chambre?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            chambre chambreToDelete; // Create an instance of chambre
            if (chambreToDelete.Supprimerchambre(chambreId)) {
                QMessageBox::information(this, "Deleted", "Chambre successfully deleted.");
                ui->tableView_3->setModel(chambreToDelete.Afficherchambres()); // Refresh the table view
            } else {
                QMessageBox::critical(this, "Error", "Failed to delete chambre.");
            }
        }
    }
    // Close the database connection
    c.closeconnection();
}
void MainWindow::on_modifier_2_clicked()
{
    QModelIndexList selectedIndexes = ui->tableView_3->selectionModel()->selectedIndexes();

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
    QString vue = ui->comboBox_3->currentText();
    int capacite = ui->capacite->text().toInt();
    int nbrlits = ui->nbrlits->text().toInt();

    if (type_c.isEmpty() || ui->prix->text().isEmpty() || dispo.isEmpty() || vue.isEmpty() || ui->capacite->text().isEmpty() || ui->nbrlits->text().isEmpty()) {
        QMessageBox::critical(this, tr("Erreur"), tr("Veuillez remplir tous les champs."), QMessageBox::Cancel);
        return;
    }

    chambre chambre(ID_C, type_c, prix, dispo, vue, capacite, nbrlits);

    if (chambre.Modifierchambre()) {
        ui->tableView_3->setModel(chambre.Afficherchambres());
        QMessageBox::information(this, tr("Succès"), tr("Modification effectuée."), QMessageBox::Cancel);
        ui->comboBox_1->setCurrentIndex(-1);
        ui->prix->clear();
        ui->radioButton_1->setChecked(false);
        ui->radioButton_2->setChecked(false);
        ui->comboBox_3->setCurrentIndex(-1);
        ui->capacite->clear();
        ui->nbrlits->clear();
    } else {
        QMessageBox::critical(this, tr("Erreur"), tr("Erreur lors de la modification du client."), QMessageBox::Cancel);
    }
}
QSqlQueryModel *chambreModel;


void MainWindow::exportToPDF3()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save PDF"), QDir::currentPath(), tr("PDF files (*.pdf)"));
    if (filePath.isEmpty()) {
        return;
    }

    QAbstractItemModel *model = ui->tableView_3->model();
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
void MainWindow::Rechercherchambre(const QString &text)
{
    chambre c;
    QSqlQueryModel *model = c.Rechercherchambre(text);
    ui->tableView_3->setModel(model);
}
void MainWindow::on_gostat_2_clicked()
{
    displayCharts_roomsByType();
    ui->stackedWidget->setCurrentIndex(17);
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


void MainWindow::on_sortComboBox_currentIndexChanged2(int index)
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
    ui->tableView_3->setModel(chambreModel);
}
//chambre
void MainWindow::on_pushButton_login_clicked() {
    QString username = ui->lineEdit_User_Name->text();  // Username entered by the user
    QString password = ui->lineEdit_Password->text();   // Password entered by the user

    // Check if the password contains only numbers
    QRegularExpression re("^[0-9]+$");  // Regex for numeric password
    QRegularExpressionMatch match = re.match(password);

    // Check the database for username (ID in TABLE4)
    QSqlQuery query;
    query.prepare("SELECT ID FROM TABLE4 WHERE ID = :username");
    query.bindValue(":username", username);

    if (query.exec() && query.next() && match.hasMatch()) {
        // If username exists in TABLE4 and password is numeric, login successful
        isUserLoggedIn = true;
        QMessageBox::information(this, "Login Successful", "Welcome!");

        // Navigate to the `Gestion Chambre` page (page 2)
        navigateToPage(2);
    } else {
        // Login failed
        QMessageBox::warning(this, "Login Failed", "Invalid username or password! Password must be numeric.");
        isUserLoggedIn = false;
    }
}

void MainWindow::on_pushButton_cancel_clicked() {
    // Confirm exit
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Exit Application", "Are you sure you want to exit?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        QApplication::quit();  // Close the application
    }
}

void MainWindow::on_stackedWidget_currentChanged(int index) {
    // Ensure the user is redirected to the login page (page 19) if not logged in
    if (!isUserLoggedIn && index != 18) {
        QMessageBox::warning(this, "Access Denied", "You must log in to access this page.");
        ui->stackedWidget->setCurrentIndex(18); // Redirect to login page
    }
}

void MainWindow::navigateToPage33(int pageIndex)
{
    if (!isUserLoggedIn && pageIndex != 18) {
        QMessageBox::warning(this, "Access Denied", "You must log in to access this page.");
        ui->stackedWidget->setCurrentIndex(18);  // Go back to login page if not logged in
        return;
    }

    ui->stackedWidget->setCurrentIndex(pageIndex);
}