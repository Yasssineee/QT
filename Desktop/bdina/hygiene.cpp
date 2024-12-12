#include "hygiene.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QVariant>
#include <QSqlError>
#include <QDebug>

hygiene::hygiene(QString typeproduith, QString quantiteenstockh, QString quantiteminimalh, QString id_produith , QString quantiteapresajouth) {
    this->typeproduith = typeproduith;
    this->quantiteenstockh = quantiteenstockh;
    this->quantiteminimalh = quantiteminimalh;
    this->id_produith = id_produith;
    this->quantiteapresajouth = quantiteapresajouth;


}

bool hygiene::ajouter() {
    QSqlQuery query;
    query.prepare("INSERT INTO GS_HYGIENE (TYPEDEPRODUITH, QUANTITEENSTOCKH, QUANTITEMINIMALH, ID_PRODUITH, QUANTITEAPRESAJOUTH) "
                  "VALUES (:TYPEDEPRODUITH, :QUANTITEENSTOCKH, :QUANTITEMINIMALH, :ID_PRODUITH, :QUANTITEAPRESAJOUTH)");

    // Bind values
    query.bindValue(":TYPEDEPRODUITH", typeproduith);
    query.bindValue(":QUANTITEENSTOCKH", quantiteenstockh);
    query.bindValue(":QUANTITEMINIMALH", quantiteminimalh);
    query.bindValue(":ID_PRODUITH", id_produith);
    query.bindValue(":QUANTITEAPRESAJOUTH", quantiteapresajouth);

    if (!query.exec()) {
        qDebug() << "Erreur d'insertion : " << query.lastError().text();
        return false;
    }

    return true;
}

QSqlQueryModel* hygiene::afficher() {
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;

    // Use exact column names as defined in SQL schema
    query.prepare("SELECT TYPEDEPRODUITH, QUANTITEENSTOCKH, QUANTITEMINIMALH, ID_PRODUITH, QUANTITEAPRESAJOUTH FROM GS_HYGIENE");
    if (!query.exec()) {
        qDebug() << "Erreur SQL : " << query.lastError().text();
        delete model;
        return nullptr;
    }

    model->setQuery(query);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Type de Produit"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Quantité en Stock"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Quantité Minimale"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("ID Produit"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Quantité Après Ajout"));

    return model;
}

bool hygiene::supprimer(const QString &id) {
    QString trimmedId = id.trimmed();

    if (trimmedId.isEmpty()) {
        qDebug() << "Invalid ID: Empty string after trimming";
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM GS_HYGIENE WHERE ID_PRODUITH = :id");
    query.bindValue(":id", trimmedId);

    qDebug() << "Executing DELETE query: " << query.lastQuery();

    if (query.exec()) {
        qDebug() << "Hygiene entry with ID" << trimmedId << "deleted successfully.";
        return true;
    } else {
        qDebug() << "Deletion failed:" << query.lastError();
        return false;
    }
}

bool hygiene::modifier() {
    QSqlQuery query;
    query.prepare("UPDATE GS_HYGIENE SET TYPEDEPRODUITH = :typeproduith, "
                  "QUANTITEENSTOCKH = :quantiteenstockh,  QUANTITEMINIMALH = :quantiteminimalh , QUANTITEAPRESAJOUTH = :quantiteapresajouth "
                  "WHERE ID_PRODUITH = :id_produith");

    // Bind values
    query.bindValue(":typeproduith", this->typeproduith);
    query.bindValue(":quantiteenstockh", this->quantiteenstockh);
    query.bindValue(":quantiteminimalh", this->quantiteminimalh);
    query.bindValue(":id_produith", this->id_produith);
    query.bindValue(":quantiteapresajouth", this->quantiteapresajouth);

    if (!query.exec()) {
        qDebug() << "Échec de la mise à jour :" << query.lastError();
        return false;
    }
    return true;
}
QSqlQueryModel* hygiene::rechercherParQuantite(const QString &quantiteEnStock) {
    QSqlQueryModel *model = new QSqlQueryModel;

    QSqlQuery query;
    query.prepare("SELECT * FROM GS_HYGIENE WHERE QUANTITEENSTOCKH = :quantiteEnStock");
    query.bindValue(":quantiteEnStock", quantiteEnStock);

    if (query.exec()) {
        model->setQuery(query);
    } else {
        qDebug() << "Query failed: " << query.lastError();
    }

    return model;
}
QSqlQueryModel* hygiene::trierParQuantite()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    QSqlQuery query;

    // Use CAST to convert QUANTITEENSTOCKH to INTEGER for proper numerical sorting
    query.prepare("SELECT * FROM GS_HYGIENE ORDER BY CAST(QUANTITEENSTOCKH AS INTEGER) ASC");
    if (query.exec()) {
        model->setQuery(query);
    } else {
        qDebug() << "Query failed: " << query.lastError();
    }

    return model;
}
QSqlQueryModel* hygiene::trierParTypeDeProduit()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    QSqlQuery query;

    // Sort alphabetically by the TYPEDEPRODUITH column
    query.prepare("SELECT * FROM GS_HYGIENE ORDER BY TYPEDEPRODUITH ASC");

    if (query.exec()) {
        model->setQuery(query);
    } else {
        qDebug() << "Query failed: " << query.lastError();
    }

    return model;
}
