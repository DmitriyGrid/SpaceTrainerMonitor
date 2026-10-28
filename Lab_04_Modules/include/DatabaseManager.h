#pragma once
#include <QString>
#include <QSqlDatabase>
class DatabaseManager {
public:
 bool connectToDatabase(const QString &host,const QString &db,const QString &user,const QString &password);
 bool saveMeasurement(int componentId,int parameterId,double value,int statusCode,QString &error);
private:
 QSqlDatabase db_;
};
