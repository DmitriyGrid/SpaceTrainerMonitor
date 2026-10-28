#pragma once
#include <QMainWindow>
#include <QLabel>
#include <QTextEdit>
#include "TelemetryServer.h"
#include "EquipmentSimulator.h"
#include "DatabaseManager.h"
class MainWindow : public QMainWindow {
 Q_OBJECT
public:
 explicit MainWindow(QWidget *parent=nullptr);
private:
 void onMeasurement(int component,int parameter,double value,int status);
 QLabel *valueLabel_;
 QLabel *stateLabel_;
 QTextEdit *log_;
 TelemetryServer server_;
 EquipmentSimulator simulator_;
 DatabaseManager database_;
 bool dbConnected_=false;
};
