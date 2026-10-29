#include "TelemetryServer.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QtEndian>
#include <cmath>
TelemetryServer::TelemetryServer(QObject *parent): QObject(parent) {
    connect(&server_, &QTcpServer::newConnection, this, &TelemetryServer::acceptClient);
}
bool TelemetryServer::start(quint16 port) { return server_.listen(QHostAddress::LocalHost, port); }
QString TelemetryServer::errorString() const { return server_.errorString(); }
void TelemetryServer::acceptClient() {
    while (auto *s = server_.nextPendingConnection()) {
        buffers_.insert(s, {});
        connect(s, &QTcpSocket::readyRead, this, [this,s] { receive(s); });
        connect(s, &QTcpSocket::disconnected, this, [this,s] { buffers_.remove(s); s->deleteLater(); });
    }
}
void TelemetryServer::receive(QTcpSocket *s) {
    auto &buf = buffers_[s];
    buf.append(s->readAll());
    while (buf.size() >= 4) {
        const quint32 len = qFromBigEndian<quint32>(reinterpret_cast<const uchar*>(buf.constData()));
        if (len == 0 || len > 65536) { emit rejected("Недопустимая длина пакета"); s->disconnectFromHost(); return; }
        if (buf.size() < static_cast<qsizetype>(len) + 4) return;
        const auto json = buf.mid(4, len);
        buf.remove(0, 4 + len);
        QJsonParseError e;
        const auto doc = QJsonDocument::fromJson(json, &e);
        if (e.error != QJsonParseError::NoError || !doc.isObject()) { emit rejected("Неверный JSON"); continue; }
        const auto o = doc.object();
        if (!o.value("component_id").isDouble() || !o.value("parameter_id").isDouble() || !o.value("value").isDouble() || !o.value("status_code").isDouble()) { emit rejected("Отсутствуют числовые поля"); continue; }
        const double c=o.value("component_id").toDouble(), p=o.value("parameter_id").toDouble(), v=o.value("value").toDouble(), status=o.value("status_code").toDouble();
        if (c<1 || p<1 || c>2147483647 || p>2147483647 || std::floor(c)!=c || std::floor(p)!=p || !std::isfinite(v) || (status!=0 && status!=1)) { emit rejected("Некорректные значения"); continue; }
        emit measurement(int(c),int(p),v,int(status));
    }
}
