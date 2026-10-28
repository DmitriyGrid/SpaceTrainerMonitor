#include "MainWindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <QInputDialog>
#include <QLineEdit>
#include <QDateTime>
MainWindow::MainWindow(QWidget *parent):QMainWindow(parent) {
 setWindowTitle("SpaceTrainerMonitor — ЛР №4");resize(730,460);
 auto *root=new QWidget(this); auto *layout=new QVBoxLayout(root);
 valueLabel_=new QLabel("Напряжение: —",root);stateLabel_=new QLabel("Состояние: ожидание",root);
 auto *start=new QPushButton("Запустить имитатор",root);
 auto *connectDb=new QPushButton("Подключить PostgreSQL",root);
 log_=new QTextEdit(root);log_->setReadOnly(true);
 layout->addWidget(valueLabel_);layout->addWidget(stateLabel_);layout->addWidget(start);layout->addWidget(connectDb);layout->addWidget(log_);setCentralWidget(root);
 if(server_.start()) log_->append("TCP-сервер слушает 127.0.0.1:45454");
 else log_->append("Ошибка TCP: "+server_.errorString());
 connect(start,&QPushButton::clicked,this,[this]{simulator_.start();});
 connect(&server_,&TelemetryServer::measurement,this,&MainWindow::onMeasurement);
 connect(&server_,&TelemetryServer::rejected,this,[this](const QString &e){log_->append("Отклонено: "+e);});
 connect(&simulator_,&EquipmentSimulator::message,this,[this](const QString &s){log_->append(s);});
 connect(connectDb,&QPushButton::clicked,this,[this] {
     bool ok=false;
     const auto pass=QInputDialog::getText(this,"PostgreSQL","Пароль postgres:",QLineEdit::Password,{},&ok);
     if(!ok) return;
     dbConnected_=database_.connectToDatabase("127.0.0.1","space_trainer_monitor","postgres",pass);
     log_->append(dbConnected_ ? "БД подключена" : "Не удалось подключить БД (проверь QPSQL/пароль)");
 });
}
void MainWindow::onMeasurement(int component,int parameter,double value,int status) {
 if(component!=3 || parameter!=4) {log_->append("Неожиданный компонент/параметр");return;}
 const bool alarm=value<22 || value>26;
 valueLabel_->setText(QString("Напряжение: %1 В").arg(value,0,'f',1));
 stateLabel_->setText(alarm ? "Состояние: ПРЕВЫШЕНИЕ" : "Состояние: норма");
 log_->append(QDateTime::currentDateTime().toString("hh:mm:ss")+QString(" | %1 В | код %2").arg(value).arg(status));
 if(dbConnected_) {QString error;if(!database_.saveMeasurement(component,parameter,value,status,error)) log_->append("Ошибка записи: "+error);}
}
