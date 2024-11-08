#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSqlTableModel>
#include <QListWidget>
#include <QMainWindow>
/*#include <Qt3DCore/QEntity>
#include <Qt3DCore/QComponent>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DCore/QTransform>*/

#include <QAction>  // Inclure QAction pour les actions de la barre d'outils

#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

#include "employee.h"
#include <QComboBox>
#include<QTableView>
#include<QStackedWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void sortEmployees();

    void resetTable();

private slots:
    void on_buttonRechercher_clicked();
    void on_add_btn_2_clicked();
    //void on_sort_btn_clicked();
    void on_delete_btn_2_clicked();
    void on_pushButton_export_pdf_clicked();
    void on_pushButton_send_2_clicked();
    void onTableViewClicked(const QModelIndex &index);


    void RechercherEmploye(const QString &text);
    void on_buttonUpdate_clicked();
    //void on_buttonUpdate_clicked();
    //void sortEmployees();
    // Ajoutez d'autres slots si nécessaire

private:


    Ui::MainWindow *ui;
    employee Etmp;
    QSqlTableModel *employeeModel;  // Pour récupérer les données des employés
    QStringList previousQuestions;  // Stocker les questions précédentes pour "l'apprentissage"
    QMap<QString, QString> knowledgeBase;




    QString respondToQuestion(const QString &question);



    // Fonction pour répondre aux questions
    void addEmployeeInfoToChat();    // Ajouter les informations des employés au chat
    void saveKnowledgeBase();
    void loadKnowledgeBase();
    void afficherTousLesEmployes();
    void trierNomsEmployes();
    //void updateEmployee();

    // Membre pour la barre d'outils
    // Pointeur pour la barre d'outils
    QList<QMap<QString, QVariant>> deletedEntries;
};

#endif // MAINWINDOW_H
