#ifndef HYGIENE_H
#define HYGIENE_H
#include <QSqlQueryModel>
#include <QString>

class hygiene {
    QString typeproduith;
    QString quantiteenstockh;
    QString quantiteminimalh;
    QString id_produith;  // Change from int to QString
    QString quantiteapresajouth;

public:
    hygiene() {}
    hygiene(QString typeproduith,QString quantiteenstockh, QString quantiteminimalh,QString id_produith , QString quantiteapresajouth );
    QString gettypeproduith() { return typeproduith; }
    QString getquantiteenstockh() { return quantiteenstockh; }
    QString getquantiteminimalh() { return quantiteminimalh; }

    QString getid_produith() { return id_produith; }
    QString getquantiteapresajouth() { return quantiteapresajouth; }

    void settypeproduith(QString typeproduith) { this->typeproduith = typeproduith; }
    void setquantiteenstockh(QString quantiteenstockh) { this->quantiteenstockh = quantiteenstockh; }

    void setquantiteminimalh(QString quantiteminimalh) { this->quantiteminimalh = quantiteminimalh; }
    void setid_produith(QString id_produith) { this->id_produith = id_produith; }

    void setquantiteapresajouth(QString quantiteapresajouth) { this->quantiteapresajouth = quantiteapresajouth; }

    bool ajouter();
    bool supprimer(const QString &id);
    QSqlQueryModel* afficher();
    bool modifier();
    QSqlQueryModel* rechercherParQuantite(const QString &quantiteEnStock);
    QSqlQueryModel* trierParQuantite(); // Function for sorting by QUANTITEENSTOCKH
    QSqlQueryModel* trierParTypeDeProduit();
};

#endif // HYGIENE_H
