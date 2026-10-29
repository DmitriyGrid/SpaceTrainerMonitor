#include "EquipmentSimulator.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QtEndian>
#include <QDateTime>
EquipmentSimulator::EquipmentSimulator(QObject *parent):QObject(parent) {
 timer_.setInterval(1000);
 connect(&timer_, &QTimer::timeout, this, &EquipmentSimulator::sendSample);
 connect(&socket_, &QTcpSocket::connected, this, [this]{ timer_.start(); emit message("Имитатор подключён"); });
 connect(&socket_, &QTcpSocket::disconnected, &timer_, &QTimer::stop);
}
void EquipmentSimulator::start(const QString &host, quint16 port) {
 if(socket_.state()!=QAbstractSocket::UnconnectedState) return;
 socket_.connectToHost(host,port);
}
void EquipmentSimulator::stop() {timer_.stop();socket_.disconnectFromHost();}
void EquipmentSimulator::sendSample() {
 const double voltage = (++counter_%7==0) ? 26.8 : 24.0 + (counter_%5)*0.1;
 QJsonObject o{{"component_id",3},{"parameter_id",4},{"timestamp",QDateTime::currentDateTimeUtc().toString(Qt::ISODateWithMs)},{"value",voltage},{"status_code",voltage>26 ? 1:0}};
 const QByteArray bytes=QJsonDocument(o).toJson(QJsonDocument::Compact);
 QByteArray frame(4, '\0');qToBigEndian<quint32>(quint32(bytes.size()),reinterpret_cast<uchar*>(frame.data()));frame.append(bytes);
 socket_.write(frame);
}
