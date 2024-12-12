#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QMessageBox>
#include <QDate>


#include "arduino.h"
//employee

#include <QSqlTableModel>
#include <QListWidget>
#include <QMainWindow>
/*#include <Qt3DCore/QEntity>
#include <Qt3DCore/QComponent>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DCore/QTransform>*/

#include <QAction>  // Inclure QAction pour les actions de la barre d'outils
#include <QCamera>
//#include <QCameraImageCapture>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

#include "employee.h"

#include "client.h"
#include <QComboBox>
#include<QTableView>
#include<QStackedWidget>




#include <QProcess>
//hygiene
#include <QMainWindow>
#include <QSerialPort>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDateTime>
#include <QMainWindow>
#include <QSqlQueryModel>
#include "hygiene.h"


class client;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void sortEmployees();
    //void runEmotionDetection();
    void resetTable();
    QString respondToQuestion(const QString &question);
//hygiene
    void mailSent(QString status);
    void box();
private slots:
    void on_ajouter_clicked();
    void on_tableView_activated(const QModelIndex &index);
    void on_modifier_clicked();
    void update_label();


    void navigateToPage(int pageIndex);
    void exportToPDF();
    void exportToExcel();
    void Rechercherclient(const QString &text);
    void on_sortComboBox_currentIndexChanged(int index);
    void displayCharts_clientByGender();
    void on_gostat_clicked();


    void on_calendrier_reserv_clicked(const QDate &date);
    void updateCalendarAndEventDetails(const QDate &date);

    void on_calendrier_clicked();

    void on_Back_clicked();

    void on_supprimerbtn_clicked();
    // autentification chambre

    void on_pushButton_login_clicked();                      // Vérifier la connexion des utilisateurs
    void on_pushButton_cancel_clicked();                     // Annuler la connexion ou quitter
void navigateToPage33(int pageIndex);
    // Gestion des pages
    void on_stackedWidget_currentChanged(int index);



        //employee


    void on_buttonRechercher_clicked();
    void on_add_btn_4_clicked();
    //void on_sort_btn_clicked();
    void on_delete_btn_4_clicked();
    void on_pushButton_export_pdf_clicked();
    void on_pushButton_send_4_clicked();
    void onTableViewClicked(const QModelIndex &index);

    void onEmotionDetection_3Clicked();


    //void displayCroppedImage(const QString &imagePath);


    void RechercherEmploye(const QString &text);
    void on_buttonUpdate_3_clicked();
    void on_stats_btn_clicked();
    //void onImageGenerated();

    //void on_buttonUpdate_3_clicked();
    //void sortEmployees();
    // Ajoutez d'autres slots si nécessaire


    //hygiene
    void on_pushButton_92_clicked();  // Ensure this is declared here
    void on_pushButton_93_clicked();
    void on_pushButton_94_clicked();
    void exportToPDF2();
    void rechercherParQuantiteEnStock();
    void on_pushButton_trier_clicked();  // Sort by quantity in stock
    void on_pushButton_trier_2_clicked();  // Sort by quantity in stock

    void on_pushButton_retour_clicked();
    void on_pushButton_retourstat_clicked();
    void on_pushButton_retour3_clicked();
    void on_pushButton_retour5_clicked();
    void connectToArduino();
    void on_readFromArduino();
    void on_checkClient(const QString &clientId);
    void on_pushButton_arduino_clicked();

    void on_tableView25_clicked(const QModelIndex &index);

    void on_pushButton_statistique_clicked();

    void on_pushButton_mail_clicked(); //mail

    void on_pushButton_2_clicked();

    void on_pushButton_barcode_clicked(); //barcode

    void on_pushButton_generateBarcode_clicked();
    void on_page4Button_4_clicked();
    void on_page3Button_4_clicked();
    void on_page2Button_4_clicked();

//    void on_page2Button_4_clicked();
    void on_page1Button_4_clicked();
    //chambre
    void on_ajouter_2_clicked();
    void on_tableView_3_activated(const QModelIndex &index);
    void on_modifier_2_clicked();
    //void navigateToPage(int pageIndex);
    void Rechercherchambre(const QString &text);
    void on_sortComboBox_currentIndexChanged2(int index);
    void exportToPDF3();
   void displayCharts_roomsByType();
    void on_gostat_2_clicked();

private:
    Ui::MainWindow *ui;

    employee Etmp;
    QSqlTableModel *employeeModel;  // Pour récupérer les données des employés
    QStringList previousQuestions;  // Stocker les questions précédentes pour "l'apprentissage"
    QMap<QString, QString> knowledgeBase;
    Arduino A;
    QString data;
    QProcess *process;
    QString test = "1";
//hygiene
    QSerialPort *serial;
    hygiene Etmp2;
    QString m_selectedId;
    QSqlQueryModel *model;
    void grantAccess(const QString &clientId);
    void denyAccess(const QString &clientId);
    QSqlDatabase db;
    //CHAMBRE

                           // Pointer to the UI object (defined only once)
       bool isUserLoggedIn;                            // Track login status








    // Fonction pour répondre aux questions
    void addEmployeeInfoToChat();    // Ajouter les informations des employés au chat
    void saveKnowledgeBase();
    void loadKnowledgeBase();
    void afficherTousLesEmployes();
    void trierNomsEmployes();




    //void keyPressEvent(QKeyEvent *event);


    //void generateAnswerFromAPI(const QString &question);
    //void onAPIResponse(QNetworkReply *reply);
    //void generateImage(const QString &description);
    //void updateEmployee();

    // Membre pour la barre d'outils
    // Pointeur pour la barre d'outils
    QList<QMap<QString, QVariant>> deletedEntries;
};

#endif // MAINWINDOW_H
