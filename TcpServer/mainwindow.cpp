#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTcpServer>
#include <QTcpSocket>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //设置连接信号
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    this->tcpServer = new QTcpServer(this);
    //       this->tcpSocket = new QTcpSocket(this);
    int port  = ui->port->toPlainText().toInt();
   tcpServer->listen(QHostAddress::Any,port);
   connect(tcpServer,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
}
void MainWindow::acceptConnection()
{
  //当有客户来访时将tcpSocket接受tcpServer建立的socket
   QTcpSocket   *tcpSocket;
   tcpSocket=new QTcpSocket();
   tcpSocket = tcpServer->nextPendingConnection();
   ClientSocketList.append(tcpSocket);
   connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readData()));
}
void MainWindow::readData()
{
    for(int i=0;i<ClientSocketList.length();i++)
    {
       QString datas =ClientSocketList[i]->readAll();
       qDebug() << datas.isEmpty()+"   "+datas.isNull();
       qDebug() << datas;
       //接收字符串数据。
       if(!(datas == "")){
          QStringList list = ClientSocketList[i]->peerAddress().toString().split(":");
          ui->dataFromClient->append("ip : "+list[list.size() - 1]+" port : "+QString::number( ClientSocketList[i]->peerPort()) +"->"+datas+"\r\n");

       }
      //      QString address = ClientSocketList[i]->peerAddress().toString();
//      int port = ClientSocketList[i]->peerPort();
//      qDebug() << address;
//      QStringList list = address.split(":");
//      qDebug() << list[list.size() - 1];
//      qDebug() << port;
     }
    //写在控件lineEdit里
}
void MainWindow::on_toAllClients_clicked()
{
    for(int i=0;i<ClientSocketList.length();i++)
     ClientSocketList[i]->write(ui->dataToClident->toPlainText().toUtf8());
}
void MainWindow::on_toSingleClient_clicked()
{
    QString ipAddress = ui->host->toPlainText();
    int port  = ui->clientPort->toPlainText().toInt();
    QTcpSocket *tcpSocket;
    tcpSocket = new QTcpSocket();
    for(int i =0; i < ClientSocketList.size(); i++){
        QStringList list = ClientSocketList[i]->peerAddress().toString().split(":");
        int port1 = ClientSocketList[i]->peerPort();
        if((list[list.size() - 1] == ipAddress) && (port == port1)){
            tcpSocket = ClientSocketList[i];
            break;
        }
    }
    tcpSocket->write(ui->dataToClident->toPlainText().toUtf8()+"\r\n");
    qDebug() << "发送数据成功";
}


