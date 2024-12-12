#ifndef CHAMBRE_H
#define CHAMBRE_H

#include "connection.h"
#include <QString>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>

class chambre
{
private:
    int ID_C;
    QString TYPE_C;
    int PRIX_C;
    QString ETAT_DIS;
    QString VUE;
    int CAPACITE_M;
    int NBRLITS;

public:
    chambre();
    chambre(int ID_C, QString TYPE_C, int PRIX_C, QString ETAT_DIS, QString VUE, int CAPACITE_M, int NBRLITS)
    {
        this->ID_C = ID_C;
        this->TYPE_C = TYPE_C;
        this->PRIX_C = PRIX_C;
        this->ETAT_DIS = ETAT_DIS;
        this->VUE = VUE;
        this->CAPACITE_M = CAPACITE_M;
        this->NBRLITS = NBRLITS;
    }

    // Setters
    void setID_CHAMBRE(int id_c) { ID_C = id_c; }
    void setType(QString type_c) { TYPE_C = type_c; }
    void setPrix(int prix_c) { PRIX_C = prix_c; }
    void setEtat(QString etat_dis) { ETAT_DIS = etat_dis; }
    void setVue(QString vue) { VUE = vue; }
    void setCapacite(int capacite_m) { CAPACITE_M = capacite_m; }
    void setNbrlits(int nbrlits) { NBRLITS = nbrlits; }

    // Getters
    int getID_CHAMBRE() { return ID_C; }
    QString getType() { return TYPE_C; }
    int getPrix() { return PRIX_C; }
    QString getEtat() { return ETAT_DIS; }
    QString getVue() { return VUE; }
    int getCapacite() { return CAPACITE_M; }
    int getNbrlits() { return NBRLITS; }

    bool Ajouterchambre();
    QSqlQueryModel* Afficherchambres();
    bool Modifierchambre();
    bool Supprimerchambre(const QString &ID_C);
    QSqlQueryModel *Rechercherchambre(const QString &text);

};

#endif // CHAMBRE_H
