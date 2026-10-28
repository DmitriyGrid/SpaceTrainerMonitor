#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>

bool DatabaseManager::connectToDatabase(const QString &host, const QString &db,
                                         const QString &user, const QString &password) {
    if (!QSqlDatabase::isDriverAvailable("QPSQL")) return false;
    // Reuse the named connection when the operator presses Connect repeatedly.
    if (db_.isValid() && db_.isOpen()) db_.close();
    if (!db_.isValid()) {
        db_ = QSqlDatabase::contains("spacetrainer")
                ? QSqlDatabase::database("spacetrainer", false)
                : QSqlDatabase::addDatabase("QPSQL", "spacetrainer");
    }
    db_.setHostName(host);
    db_.setPort(5432);
    db_.setDatabaseName(db);
    db_.setUserName(user);
    db_.setPassword(password);
    return db_.open();
}

bool DatabaseManager::saveMeasurement(int componentId,int parameterId,double value,int statusCode,QString &error) {
    if(!db_.isOpen()) {error="БД не подключена"; return false;}
    QSqlQuery lookup(db_);
    lookup.prepare("SELECT 1 FROM parameters WHERE id=:p AND component_id=:c");
    lookup.bindValue(":p",parameterId);lookup.bindValue(":c",componentId);
    if(!lookup.exec()||!lookup.next()) {error="Параметр не принадлежит компоненту";return false;}
    QSqlQuery q(db_);
    q.prepare("INSERT INTO measurements(parameter_id,value,status_code) VALUES(:p,:v,:s)");
    q.bindValue(":p",parameterId);q.bindValue(":v",value);q.bindValue(":s",statusCode);
    if(!q.exec()){error=q.lastError().text();return false;}
    return true;
}
