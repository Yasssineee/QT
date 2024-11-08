#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connection.h"
#include <QMessageBox>
#include <QString>


#include <QFileDialog>
#include <QTextDocument>
#include<QStringList>
#include <QListWidget>
#include <QMimeData>

#include <QDateTime>


#include <QPrinter>
#include <QPainter>




#include <QAction>
#include <QVBoxLayout>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->comboBox_25->addItem("Sort by Name (Alphabetical)", "name");
    ui->comboBox_25->addItem("Sort by Salary (Highest First)", "salary");
    Connection conn; // Create an instance of your Connection class
    if (conn.createconnect()) {
        QMessageBox::critical(this, "success de connexion", "success de la connexion à la base de données.");
        // Optionally, you can close the application or disable certain features
    }


    connect(ui->comboBox_25, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::sortEmployees);

    connect(ui->btnReset, &QPushButton::clicked, this, &MainWindow::resetTable);





    connect(ui->tableView_2, &QTableView::clicked, this, &MainWindow::onTableViewClicked);
    //connect(ui->buttonUpdate, &QPushButton::clicked, this, &MainWindow::updateEmployee);


    // Connect the sort button's clicked signal to the sorting slot
    // connect(ui->sort_btn, &QPushButton::clicked, this, &MainWindow::sortEmployees);
    connect(ui->buttonUpdate, &QPushButton::clicked, this, &MainWindow::on_buttonUpdate_clicked);

    connect(ui->buttonTrierNoms, &QPushButton::clicked, this, &MainWindow::trierNomsEmployes);


    connect(ui->btnReset, &QPushButton::clicked, this, &MainWindow::afficherTousLesEmployes);
    connect(ui->add_btn_2, SIGNAL(clicked()), this, SLOT(on_add_btn_2_clicked()));


    connect(ui->lineEditNom, SIGNAL(textChanged(const QString &)), this, SLOT(RechercherEmploye(const QString &)));


    connect(ui->pushButton_send_2, &QPushButton::clicked, this, &MainWindow::on_pushButton_send_2_clicked);
    connect(ui->delete_btn_2, &QPushButton::clicked, this, &MainWindow::on_delete_btn_2_clicked);

    connect(ui->export_pdf_2, &QPushButton::clicked, this, &MainWindow::on_pushButton_export_pdf_clicked);
    connect(ui->page1Button_2, &QPushButton::clicked, this, [=]() { ui->stackedWidget->setCurrentIndex(0); });
    connect(ui->page2Button_2, &QPushButton::clicked, this, [=]() { ui->stackedWidget->setCurrentIndex(1); });
    connect(ui->page3Button_2, &QPushButton::clicked, this, [=]() { ui->stackedWidget->setCurrentIndex(2); });
    connect(ui->page4Button_2, &QPushButton::clicked, this, [=]() { ui->stackedWidget->setCurrentIndex(3); });
    connect(ui->page5Button_2, &QPushButton::clicked, this, [=]() { ui->stackedWidget->setCurrentIndex(4); });

    ui->tableView_2->setModel(Etmp.afficher());
    this->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);

    employeeModel = new QSqlTableModel(this);
    employeeModel->setTable("TABLE4");
    employeeModel->select();

    knowledgeBase.insert("What is your name?", "I am your assistant.");
    knowledgeBase.insert("How can I help you?", "You can ask me about the employees.");
    knowledgeBase.insert("Tell me about the employees.", "Here are the details of the employees.");
}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::on_add_btn_2_clicked() {
    qDebug() << "Signal d'ajout déclenché"; // Affiche à chaque fois que le signal est émis

    // Vérification si le bouton est déjà désactivé, cela évite un double clic rapide
    if (!ui->add_btn_2->isEnabled()) {
        qDebug() << "Le bouton est déjà désactivé, exit.";
        return;  // Sortie prématurée si le bouton est désactivé
    }

    ui->add_btn_2->setEnabled(false);  // Désactive le bouton pour éviter les clics multiples

    // Récupération des données de l'interface
    int id = ui->lineEdit_26->text().toInt();
    QString nom = ui->lineEdit_25->text();
    QString prenom = ui->lineEdit_24->text();
    QString mail = ui->lineEdit_email_2->text();
    QString dateembauche = ui->lineEdit_date_2->text();
    QString certificats = ui->lineEdit_certificats_2->text();
    QString diplomes = ui->lineEdit_diplomes_2->text();
    QString salaire = ui->lineEdit_salaire_3->text();
    QString assurance = ui->lineEdit_assurance_2->text();

    // Vérification que les champs ne sont pas vides
    if (nom.isEmpty() || prenom.isEmpty() || mail.isEmpty() || dateembauche.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs obligatoires.");
        ui->add_btn_2->setEnabled(true); // Réactive le bouton si un champ est vide
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

    ui->add_btn_2->setEnabled(true); // Réactive le bouton après l'ajout
    qDebug() << "Fin de l'ajout"; // Affiche ce message à la fin de l'exécution du slot
}



void MainWindow::on_delete_btn_2_clicked() {
    QModelIndexList selection = ui->tableView_2->selectionModel()->selectedRows();

    if (!selection.isEmpty()) {
        int row = selection.at(0).row();
        QModelIndex idIndex = ui->tableView_2->model()->index(row, 0);

        int id = idIndex.data().toInt(); // Récupérer l'ID de l'employé à supprimer

        // Stocker les données supprimées
        QMap<QString, QVariant> entry;
        entry["id"] = id;
        entry["nom"] = ui->tableView_2->model()->index(row, 1).data().toString();
        entry["prenom"] = ui->tableView_2->model()->index(row, 2).data().toString();
        entry["email"] = ui->tableView_2->model()->index(row, 3).data().toString(); // Ajoutez l'email
        entry["dateembauche"] = ui->tableView_2->model()->index(row, 4).data().toString(); // Ajoutez la date d'embauche
        entry["certificats"] = ui->tableView_2->model()->index(row, 5).data().toString(); // Ajoutez les certificats
        entry["diplomes"] = ui->tableView_2->model()->index(row, 6).data().toString(); // Ajoutez les diplômes
        entry["salaire"] = ui->tableView_2->model()->index(row, 7).data().toDouble(); // Ajoutez le salaire
        entry["assurance"] = ui->tableView_2->model()->index(row, 8).data().toString(); // Ajoutez l'assurance
        deletedEntries.append(entry);

        // Supprimer l'employé
        bool test = Etmp.supprimer(id);

        // Mise à jour de la vue et message d'information
        if (test) {
            ui->tableView_2->setModel(Etmp.afficher());
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
    int rowCount = ui->tableView_2->model()->rowCount();
    int columnCount = ui->tableView_2->model()->columnCount();

    // Dessiner l'en-tête du tableau
    int yPos = 100;
    for (int column = 0; column < columnCount; column++) {
        QString headerText = ui->tableView_2->model()->headerData(column, Qt::Horizontal).toString();
        painter.drawText(column * 100, yPos, headerText);
    }

    // Dessiner les lignes du tableau
    yPos += 50;
    for (int row = 0; row < rowCount; row++) {
        for (int column = 0; column < columnCount; column++) {
            QModelIndex index = ui->tableView_2->model()->index(row, column);
            QString data = index.data().toString();
            painter.drawText(column * 100, yPos, data);
        }
        yPos += 50; // Déplacer la position verticale pour la prochaine ligne
    }

    painter.end(); // Terminer le dessin dans le PDF

    QMessageBox::information(this, "Succès", "Les données ont été exportées avec succès en PDF.");
}


void MainWindow::on_pushButton_send_2_clicked()
{
    QString question = ui->lineEdit_39->text().trimmed();

    if (question.isEmpty()) {
        return; // Skip empty input
    }

    ui->textEdit_chat_2->append("Vous : " + question);

    // Get the selected version from the combo box
    QString selectedVersion = ui->comboBox_2->currentText();

    QString response;

    if (selectedVersion == "Image Generation") {
        // Image generation logic
        //generateImage(question); // Assume question is the image description
        response = "Generating your image...";
    }
    else if (selectedVersion == "Smart Employee Response") {
        // Smart employee response logic
        response = respondToQuestion(question);
    }
    else if (selectedVersion == "Learning Mode") {
        // Learning mode logic
        response = respondToQuestion(question);
    }

    ui->textEdit_chat_2->append("Chatbot : " + response);
    ui->lineEdit_39->clear(); // Clear input field
}

/*QString MainWindow::generateImage(const QString &description)
{
    // Placeholder for image generation logic
    QString imageUrl = callImageGenerationAPI(description); // Implement this function
    if (!imageUrl.isEmpty()) {
        QPixmap pixmap(imageUrl);
        ui->imageLabel->setPixmap(pixmap); // Assuming you have a QLabel named imageLabel
        return "Image generated successfully.";
    }
    return "Could not generate image.";
}*/

//QString MainWindow::callImageGenerationAPI(const QString &description)
//{
// Simple placeholder: return a fake image path
// Replace this logic with actual image generation API call
//return "/path/to/generated/image.png"; // Replace with actual image generation logic
//}

QString MainWindow::respondToQuestion(const QString &question)
{
    // Check if the question is in the knowledge base
    if (knowledgeBase.contains(question)) {
        return knowledgeBase[question]; // Return known answer
    }

    // If not known, ask the user for the answer
    QString userResponse = QInputDialog::getText(this, "Learning Mode",
                                                 "I don't know the answer. What should I respond?");
    if (!userResponse.isEmpty()) {
        knowledgeBase[question] = userResponse; // Save new response
        return "Thanks! I've learned a new response.";
    }

    return "I'm not sure how to answer that.";
}

void MainWindow::addEmployeeInfoToChat()
{
    for (int row = 0; row < employeeModel->rowCount(); ++row) {
        QString employeeInfo = QString("ID: %1, Nom: %2, Prénom: %3")
                                   .arg(employeeModel->data(employeeModel->index(row, 0)).toString()) // ID
                                   .arg(employeeModel->data(employeeModel->index(row, 1)).toString()) // Nom
                                   .arg(employeeModel->data(employeeModel->index(row, 2)).toString()); // Prénom

        ui->textEdit_chat_2->append(employeeInfo);
    }
}



void MainWindow::on_buttonRechercher_clicked() {
    QString nomRecherche = ui->lineEditNom->text(); // Obtenir le texte du champ de saisie
    employee emp; // Assurez-vous d'avoir un constructeur sans argument dans `employee`
    QSqlQueryModel *model = emp.rechercherParNom(nomRecherche); // Appel de la méthode


    if (model) {
        ui->tableView_2->setModel(model); // Affiche le modèle de résultat dans tableView
    } else {
        qDebug() << "Aucun résultat trouvé ou erreur dans la requête.";
    }
}


void MainWindow::afficherTousLesEmployes() {
    QSqlQueryModel *model = new QSqlQueryModel(this);
    QSqlQuery query("SELECT * FROM TABLE4"); // Assurez-vous que 'employees' est le nom correct de votre table


        model->setQuery(query);
        ui->tableView_2->setModel(model); // Met à jour la vue avec le modèle

}


void MainWindow::trierNomsEmployes() {
    QSqlQueryModel *model = new QSqlQueryModel(this);
    QSqlQuery query("SELECT * FROM TABLE4 ORDER BY NOM ASC"); // Assurez-vous que 'nom' est le bon nom de colonne


        model->setQuery(query);
        ui->tableView_2->setModel(model); // Met à jour la vue avec le modèle

}



void MainWindow::onTableViewClicked(const QModelIndex &index) {
    // Get the row number
    int row = index.row();

    // Retrieve data from the selected row and set it to the corresponding input fields
    ui->lineEdit_26->setText(ui->tableView_2->model()->index(row, 0).data().toString()); // ID
    ui->lineEdit_25->setText(ui->tableView_2->model()->index(row, 1).data().toString()); // Nom
    ui->lineEdit_24->setText(ui->tableView_2->model()->index(row, 2).data().toString()); // Prenom
    ui->lineEdit_email_2->setText(ui->tableView_2->model()->index(row, 3).data().toString()); // Mail
    ui->lineEdit_date_2->setText(ui->tableView_2->model()->index(row, 4).data().toString()); // Date Embauche
    ui->lineEdit_certificats_2->setText(ui->tableView_2->model()->index(row, 5).data().toString()); // Certificats
    ui->lineEdit_diplomes_2->setText(ui->tableView_2->model()->index(row, 6).data().toString()); // Diplomes
    ui->lineEdit_salaire_3->setText(ui->tableView_2->model()->index(row, 7).data().toString()); // Salaire
    ui->lineEdit_assurance_2->setText(ui->tableView_2->model()->index(row, 8).data().toString()); // Assurance
}


void MainWindow::on_buttonUpdate_clicked() {
    // Retrieve data from input fields
    int id = ui->lineEdit_26->text().toInt();
    QString nom = ui->lineEdit_25->text();
    QString prenom = ui->lineEdit_24->text();
    QString mail = ui->lineEdit_email_2->text();
    QString dateembauche = ui->lineEdit_date_2->text();
    QString certificats = ui->lineEdit_certificats_2->text();
    QString diplomes = ui->lineEdit_diplomes_2->text();
    QString salaire = ui->lineEdit_salaire_3->text();
    QString assurance = ui->lineEdit_assurance_2->text();

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
        ui->tableView_2->setModel(Etmp.afficher()); // Refresh the table view
    } else {
        QMessageBox::critical(this, "Erreur", "La mise à jour a échoué.");
    }
}







/*void MainWindow::updateEmployee() {
    // Vérifier si un employé est sélectionné
    QItemSelectionModel *selectionModel = ui->tableView_2->selectionModel();
    if (!selectionModel->hasSelection()) {
        QMessageBox::warning(this, "Erreur", "Aucun employé sélectionné !");
        return;
    }

    int selectedRow = selectionModel->currentIndex().row();
    QSqlQueryModel *model = qobject_cast<QSqlQueryModel *>(ui->tableView_2->model());
    int employeeId = model->data(model->index(selectedRow, 0)).toInt(); // ID dans la 1ère colonne

    // Récupérer les nouvelles valeurs
    QString newName = ui->lineEdit_25->text();
    QString newSurname = ui->lineEdit_24->text();
    QString newEmail = ui->lineEdit_email_2->text();
    QString newHireDate = ui->lineEdit_date_2->text();
    QString newCertificates = ui->lineEdit_certificats_2->text();
    QString newDiplomas = ui->lineEdit_diplomes_2->text();
    QString newSalary = ui->lineEdit_salaire_3->text(); // S'assurer que c'est un QString
    QString newInsurance = ui->lineEdit_assurance_2->text();

    // Préparer la requête de mise à jour
    QSqlQuery query;
    query.prepare("UPDATE TABLE4 SET nom = :nom, prenom = :prenom, email = :email, "
                  "date_embauche = :date, certificats = :certificats, diplomes = :diplomes, "
                  "salaire = :salaire, assurance = :assurance WHERE id = :id");
    query.bindValue(":nom", newName);
    query.bindValue(":prenom", newSurname);
    query.bindValue(":email", newEmail);
    query.bindValue(":date", newHireDate);
    query.bindValue(":certificats", newCertificates);
    query.bindValue(":diplomes", newDiplomas);
    query.bindValue(":salaire", newSalary);
    query.bindValue(":assurance", newInsurance);
    query.bindValue(":id", employeeId);

    // Exécuter la requête de mise à jour


    // Recharger les données dans le modèle pour voir les changements
    model->setQuery("SELECT * FROM TABLE4");
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Prénom");
    model->setHeaderData(3, Qt::Horizontal, "Email");
    model->setHeaderData(4, Qt::Horizontal, "Date d'embauche");
    model->setHeaderData(5, Qt::Horizontal, "Certificats");
    model->setHeaderData(6, Qt::Horizontal, "Diplômes");
    model->setHeaderData(7, Qt::Horizontal, "Salaire");
    model->setHeaderData(8, Qt::Horizontal, "Assurance");

    // Met à jour le QTableView
    ui->tableView_2->setModel(model);
}


*/

/*void MainWindow::on_buttonUpdate_clicked()
{
    QModelIndexList selectedIndexes = ui->tableView_2->selectionModel()->selectedIndexes();

    if (selectedIndexes.size() == 0) {
        QMessageBox::warning(this, tr("Aucune sélection"), tr("Veuillez sélectionner une ligne à modifier."), QMessageBox::Ok);
        return;
    } else if (selectedIndexes.size() > 1) {
        QMessageBox::warning(this, tr("Sélection multiple"), tr("Veuillez sélectionner une seule ligne à modifier."), QMessageBox::Ok);
        return;
    }

    int Id_employee = selectedIndexes.at(0).sibling(selectedIndexes.at(0).row(), 0).data().toInt();

    QString nom = ui->lineEdit_25->text();
    QString prenom = ui->lineEdit_24->text();
    QString mail = ui->lineEdit_email_2->text();
    QString dateembauche = ui->lineEdit_date_2->text();
    QString certificats = ui->lineEdit_certificats_2->text();
    QString diplomes = ui->lineEdit_diplomes_2->text();
    QString salaire = ui->lineEdit_salaire_3->text();
    QString assurance = ui->lineEdit_assurance_2->text();

    if (nom.isEmpty()) {
        QMessageBox::critical(this, tr("Erreur"), tr("Veuillez remplir tous les champs."), QMessageBox::Cancel);
        return;
    }

    // Create an instance of employee using the entered data
    employee e(Id_employee, nom, prenom, mail, dateembauche, certificats, diplomes, salaire, assurance);

    // Call the Modifieremployee method on the object 'e'
    if (e.Modifieremployee()) {
        // Update the table view with the modified data
        ui->tableView_2->setModel(e.afficher());
        QMessageBox::information(this, tr("Succès"), tr("Modification effectuée."), QMessageBox::Cancel);

        // Clear the input fields
        ui->lineEdit_25->clear();
        ui->lineEdit_24->clear();
        ui->lineEdit_email_2->clear();
        ui->lineEdit_date_2->clear();
        ui->lineEdit_certificats_2->clear();
        ui->lineEdit_diplomes_2->clear();
        ui->lineEdit_salaire_3->clear();
        ui->lineEdit_assurance_2->clear();
    } else {
        QMessageBox::critical(this, tr("Erreur"), tr("Erreur lors de la modification du client."), QMessageBox::Cancel);
    }
}

*/



void MainWindow::RechercherEmploye(const QString &text)
{
    employee e;  // Créez un objet employee
    QSqlQueryModel *model = e.RechercherEmploye(text);  // Appelle la méthode de recherche
    ui->tableView_2->setModel(model);  // Affiche les résultats dans le QTableView
}



void MainWindow::sortEmployees() {
    QString sortCriteria = ui->comboBox_25->currentData().toString();

    QSqlQuery query;
    if (sortCriteria == "name") {
        // Sort by name alphabetically
        query.prepare("SELECT * FROM client ORDER BY NOM ASC");
    } else if (sortCriteria == "salary") {
        // Sort by salary in descending order
        query.prepare("SELECT * FROM client ORDER BY SALAIRE DESC");
    }

    if (!query.exec()) {
        qDebug() << "Erreur lors de la récupération des employés triés :" << query.lastError().text();
        return;
    }

    // Create a new model for the sorted data
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(query);

    // Set the model on your table view
    ui->tableView_2->setModel(model);

    // Set headers (optional)
    model->setHeaderData(0, Qt::Horizontal, tr("Nom"));
    model->setHeaderData(1, Qt::Horizontal, tr("Prénom"));
    model->setHeaderData(2, Qt::Horizontal, tr("ID"));
    model->setHeaderData(3, Qt::Horizontal, tr("Adresse"));
    model->setHeaderData(4, Qt::Horizontal, tr("Genre"));
    model->setHeaderData(5, Qt::Horizontal, tr("Nationalité"));
    model->setHeaderData(6, Qt::Horizontal, tr("Email"));
    model->setHeaderData(7, Qt::Horizontal, tr("Salaire"));
}


void MainWindow::resetTable() {
    QSqlQuery query;
    query.prepare("SELECT * FROM client"); // No ORDER BY clause to fetch unsorted data

    if (!query.exec()) {
        qDebug() << "Erreur lors de la récupération des employés non triés :" << query.lastError().text();
        return;
    }

    // Create a new model for the unsorted data
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(query);

    // Set the model on your table view
    ui->tableView_2->setModel(model);

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
    ui->comboBox_25->setCurrentIndex(-1);  // -1 can represent no selection, adjust if needed
}


