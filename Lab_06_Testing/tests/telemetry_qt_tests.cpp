#include <QtTest/QtTest>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QtEndian>
#include "TelemetryServer.h"
static QByteArray frame(const QByteArray &payload) {
 QByteArray b(4,'\0'); qToBigEndian<quint32>(quint32(payload.size()),reinterpret_cast<uchar*>(b.data()));return b+payload;
}
static QByteArray valid() {
 return QJsonDocument(QJsonObject{{"component_id",3},{"parameter_id",4},{"value",24.1},{"status_code",0}}).toJson(QJsonDocument::Compact);
}
class TcpTests:public QObject {Q_OBJECT
private slots:
 void validAndInvalidFrames() {
  TelemetryServer server;
  quint16 port=47111;
  while(port<47130 && !server.start(port)) ++port;
  QVERIFY(port<47130);
  QSignalSpy accepted(&server,&TelemetryServer::measurement);
  QSignalSpy rejected(&server,&TelemetryServer::rejected);
  QTcpSocket sock; sock.connectToHost(QHostAddress::LocalHost,port);
  QVERIFY(sock.waitForConnected(3000));
  const auto bytes=frame(valid());
  sock.write(bytes.left(2)); sock.flush();
  QTest::qWait(60); QCOMPARE(accepted.size(),0);
  sock.write(bytes.mid(2)); sock.flush();
  QTRY_COMPARE_WITH_TIMEOUT(accepted.size(),1,3000);
  QCOMPARE(accepted.first().at(0).toInt(),3);
  QCOMPARE(accepted.first().at(1).toInt(),4);
  QCOMPARE(accepted.first().at(3).toInt(),0);
  sock.write(frame("not-json"));sock.flush();
  QTRY_COMPARE_WITH_TIMEOUT(rejected.size(),1,3000);
  sock.write(frame(QJsonDocument(QJsonObject{{"component_id",3},{"parameter_id",4},{"value",24.0}}).toJson()));sock.flush();
  QTRY_COMPARE_WITH_TIMEOUT(rejected.size(),2,3000);
  auto bad=frame(valid()); bad[0]=char(0x01);bad[1]=char(0x00);bad[2]=char(0x00);bad[3]=char(0x01);
  sock.write(bad);sock.flush();
  QTRY_VERIFY_WITH_TIMEOUT(rejected.size()>=3,3000);
 }
};
QTEST_GUILESS_MAIN(TcpTests)
#include "telemetry_qt_tests.moc"
