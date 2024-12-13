#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QModelIndex>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Slots liés à la gestion des chambres
    void on_ajouter_clicked();                               // Ajouter une nouvelle chambre
    void on_tableView_activated(const QModelIndex &index);   // Gestion des clics dans la vue TableView
    void on_modifier_clicked();                              // Modifier une chambre existante
    void navigateToPage(int pageIndex);                      // Naviguer entre les pages
    void Rechercherchambre(const QString &text);             // Recherche dynamique des chambres
    void on_sortComboBox_currentIndexChanged(int index);     // Trier les chambres

    // Exportation et statistiques
    void exportToPDF();                                      // Exporter les données en PDF
    void displayCharts_roomsByType();                        // Afficher des graphiques des chambres par type
    void on_gostat_clicked();                                // Gérer les statistiques

    // Gestion des utilisateurs
    void on_pushButton_login_clicked();                      // Vérifier la connexion des utilisateurs
    void on_pushButton_cancel_clicked();                     // Annuler la connexion ou quitter

    // Gestion des pages
    void on_stackedWidget_currentChanged(int index);         // Gestion des changements de page

private:
    Ui::MainWindow *ui;                                      // Interface utilisateur
};

#endif // MAINWINDOW_H
