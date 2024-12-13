#include "alimentaire.h"
#include <QSqlError>
#include <QDebug>

Alimentaire::Alimentaire() : code(0), qtt(0), qttmin(0), prd("") {}

Alimentaire::Alimentaire(int code, int qtt, int qttmin, QString prd)
    : code(code), qtt(qtt), qttmin(qttmin), prd(prd) {}

bool Alimentaire::add() {
    QSqlQuery query;
    query.prepare("INSERT INTO TABLEALIMENTATION (code, qtt, qttmin, prd) VALUES (:code, :qtt, :qttmin, :prd)");
    query.bindValue(":code", code);
    query.bindValue(":qtt", qtt);
    query.bindValue(":qttmin", qttmin);
    query.bindValue(":prd", prd);
    return query.exec();
}

bool Alimentaire::remove(int code) {
    QSqlQuery query;
    query.prepare("DELETE FROM TABLEALIMENTATION WHERE code = :code");
    query.bindValue(":code", code);

    if (!query.exec()) {
        qDebug() << "SQL Error:" << query.lastError();
        return false;
    }

    return true;
}

bool Alimentaire::update() {
    QSqlQuery query;
    query.prepare("UPDATE TABLEALIMENTATION SET qtt = :qtt, qttmin = :qttmin, prd = :prd WHERE code = :code");
    query.bindValue(":qtt", qtt);
    query.bindValue(":qttmin", qttmin);
    query.bindValue(":prd", prd);
    query.bindValue(":code", code);
    return query.exec();
}

std::vector<Alimentaire> Alimentaire::getSortedByQuantityDifference() {
    std::vector<Alimentaire> items;
    QSqlQuery query;
    
    // Fetch all items from the database
    query.prepare("SELECT * FROM ALIMENTAIRE");
    if (query.exec()) {
        while (query.next()) {
            Alimentaire item(
                query.value(0).toInt(),      // code
                query.value(1).toInt(),      // qtt
                query.value(2).toInt(),      // qttmin
                query.value(3).toString()    // prd
            );
            items.push_back(item);
        }
    }
    
    // Sort items by the difference between qtt and qttmin (ascending order)
    std::sort(items.begin(), items.end(), 
        [](const Alimentaire& a, const Alimentaire& b) {
            int diffA = a.getQtt() - a.getQttmin();
            int diffB = b.getQtt() - b.getQttmin();
            return diffA < diffB;
        });
    
    return items;
}
