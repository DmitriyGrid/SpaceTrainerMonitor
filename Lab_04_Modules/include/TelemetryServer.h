#pragma once
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHash>
class TelemetryServer : public QObject {
    Q_OBJECT
public:
    explicit TelemetryServer(QObject *parent = nullptr);
    bool start(quint16 port = 45454);
    QString errorString() const;
signals:
    void measurement(int componentId, int parameterId, double value, int statusCode);
    void rejected(const QString &reason);
private:
    void acceptClient();
    void receive(QTcpSocket *socket);
    QTcpServer server_;
    QHash<QTcpSocket*, QByteArray> buffers_;
};
