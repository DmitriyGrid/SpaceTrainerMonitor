#pragma once
#include <QObject>
#include <QTcpSocket>
#include <QTimer>
class EquipmentSimulator : public QObject {
 Q_OBJECT
public:
 explicit EquipmentSimulator(QObject *parent=nullptr);
 void start(const QString &host="127.0.0.1", quint16 port=45454);
 void stop();
signals:
 void message(const QString &text);
private:
 void sendSample();
 QTcpSocket socket_;
 QTimer timer_;
 int counter_=0;
};
