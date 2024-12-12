#include "chambre.h"
#include <QDebug>

chambre::chambre()
{

}

bool chambre::Ajouterchambre()
{



    QSqlQuery query;

    query.prepare("INSERT INTO chambre ( TYPE_C, PRIX_C, ETAT_DIS, VUE, CAPACITE_M, NBRLITS) VALUES ( :TYPE_C, :PRIX_C, :ETAT_DIS, :VUE, :CAPACITE_M, :NBRLITS)");

    query.bindValue(":TYPE_C", TYPE_C);
    query.bindValue(":PRIX_C", PRIX_C);
    query.bindValue(":ETAT_DIS", ETAT_DIS);
    query.bindValue(":VUE", VUE);
    query.bindValue(":CAPACITE_M", CAPACITE_M);
    query.bindValue(":NBRLITS", NBRLITS);

    return query.exec();
}

QSqlQueryModel *chambre::Afficherchambres()
{
    QSqlQueryModel *modal = new QSqlQueryModel();

    modal->setQuery("SELECT ID_C, TYPE_C, PRIX_C, ETAT_DIS, VUE, CAPACITE_M, NBRLITS, 'Update' ,'Delete' AS Action FROM chambre");
    modal->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_CHAMBRE"));
    modal->setHeaderData(1, Qt::Horizontal, QObject::tr("TYPE_C"));
    modal->setHeaderData(2, Qt::Horizontal, QObject::tr("PRIX_C"));
    modal->setHeaderData(3, Qt::Horizontal, QObject::tr("ETAT_DIS"));
    modal->setHeaderData(4, Qt::Horizontal, QObject::tr("VUE"));
    modal->setHeaderData(5, Qt::Horizontal, QObject::tr("CAPACITE_M"));
    modal->setHeaderData(6, Qt::Horizontal, QObject::tr("NBRLITS"));
    modal->setHeaderData(7, Qt::Horizontal, QObject::tr("Action"));
    modal->setHeaderData(8, Qt::Horizontal, QObject::tr("Action"));// Set header for the new column


    return modal;
}

bool chambre::Modifierchambre()
{
    QSqlQuery query;

    if (!query.prepare("UPDATE chambre SET TYPE_C = :TYPE_C, PRIX_C = :PRIX_C, ETAT_DIS = :ETAT_DIS, VUE = :VUE, CAPACITE_M = :CAPACITE_M, NBRLITS = :NBRLITS WHERE ID_C = :ID_C")) {
        qDebug() << "Prepare failed:" << query.lastError();
        return false;
    }

    query.bindValue(":TYPE_C", TYPE_C);
    query.bindValue(":PRIX_C", PRIX_C);
    query.bindValue(":ETAT_DIS", ETAT_DIS);
    query.bindValue(":VUE", VUE);
    query.bindValue(":CAPACITE_M", CAPACITE_M);
    query.bindValue(":NBRLITS", NBRLITS);
    query.bindValue(":ID_C", ID_C);

    if (!query.exec()) {
        qDebug() << "Exec failed:" << query.lastError();
        return false;
    }

    return true;
}

bool chambre::Supprimerchambre(const QString &ID_C)
{
    QSqlQuery query;

    query.prepare("DELETE FROM chambre WHERE ID_C = :ID_C");
    query.bindValue(":ID_C", ID_C);

    return query.exec();
}
QSqlQueryModel *chambre::Rechercherchambre(const QString &text)
{
    QSqlQueryModel *modal = new QSqlQueryModel();

    // Construct the SQL query with the updated column names and table name
    QString queryStr = QString("SELECT ID_C, CAPACITE_M, PRIX_C, NBRLITS, VUE, TYPE_C, ETAT_DIS, 'Update' AS Action, 'Delete' AS Action "
                               "FROM chambre WHERE ID_C LIKE '%%1%' OR CAPACITE_M LIKE '%%1%' OR PRIX_C LIKE '%%1%' OR NBRLITS LIKE '%%1%' OR VUE LIKE '%%1%' OR TYPE_C LIKE '%%1%' OR ETAT_DIS LIKE '%%1%'")
                           .arg(text);

    // Set the query for the model
    modal->setQuery(queryStr);

    // Set headers for the columns
    modal->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_C"));
    modal->setHeaderData(1, Qt::Horizontal, QObject::tr("CAPACITE_M"));
    modal->setHeaderData(2, Qt::Horizontal, QObject::tr("PRIX_C"));
    modal->setHeaderData(3, Qt::Horizontal, QObject::tr("NBRLITS"));
    modal->setHeaderData(4, Qt::Horizontal, QObject::tr("VUE"));
    modal->setHeaderData(5, Qt::Horizontal, QObject::tr("TYPE_C"));
    modal->setHeaderData(6, Qt::Horizontal, QObject::tr("ETAT_DIS"));
    modal->setHeaderData(7, Qt::Horizontal, QObject::tr("Action"));
    modal->setHeaderData(8, Qt::Horizontal, QObject::tr("Action"));

    // Return the model with the query results
    return modal;
}


