#ifndef ALIMENTAIRE_H
#define ALIMENTAIRE_H

#include <QString>
#include <QSqlQuery>
#include <vector>

class Alimentaire {
    int code;
    int qtt;
    int qttmin;
    QString prd;

public:
    Alimentaire();
    Alimentaire(int code, int qtt, int qttmin, QString prd);

    // Getters
    int getCode() const { return code; }
    QString getPrd() const { return prd; }
    int getQtt() const { return qtt; }
    int getQttmin() const { return qttmin; }

    // Setters
    void setCode(int c) { code = c; }
    void setPrd(QString p) { prd = p; }
    void setQtt(int q) { qtt = q; }
    void setQttmin(int qm) { qttmin = qm; }

    // CRUD methods
    bool add();
    bool remove(int code);
    bool update();

    // Static methods
    static std::vector<Alimentaire> getSortedByQuantityDifference();
};

#endif // ALIMENTAIRE_H
