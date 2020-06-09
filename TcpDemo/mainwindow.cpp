#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    /*这个代码块用于执行初始化*/
    ui->setupUi(this);
    tcpSocket = new QTcpSocket();
    /*设置连接信号*/
    QObject::connect(tcpSocket,&QTcpSocket::readyRead,this,&MainWindow::socket_Read_Data);
    QObject::connect(tcpSocket,&QTcpSocket::disconnected,this,&MainWindow::socket_Disconnected);

    ui->send->setEnabled(false);
    ui->host->setText("127.0.0.1");
    ui->port->setText("8090");

}

MainWindow::~MainWindow()
{
    delete this->tcpSocket;
    delete ui;
}

void MainWindow::on_connect_clicked(){


    if(ui->connect->text() == tr("connect")){
        QString ip; //ip地址
        int port;  //端口号
        ip = ui->host->text();
        port = ui->port->text().toInt();

        tcpSocket->abort();  //取消已有连接
        tcpSocket->connectToHost(ip,port,QTcpSocket::ReadWrite);  //连接到服务器
//        if(!tcpSocket->waitForConnected(30000)){ //连接超时
//            qDebug() << "Connect to server failed!!!";
//            ui->connectmessage->setText("Connect to server failed!!!");
//            return;
//        }
        qDebug() << "Connect to server success!!!";
        ui->connectmessage->setText("Connect to server success");
        ui->send->setEnabled(true);
        ui->connect->setText("cancel connect");

    }else{
        tcpSocket->disconnectFromHost();  //断开连接
        ui->connect->setText("connect");
        ui->send->setEnabled(false);
        ui->connectmessage->setText("disconneted from server!!!");
    }

}

void MainWindow::on_send_clicked(){
    qDebug() << "send : " << ui->sendmessage;
    //以UTF-8编码格式发送数据.
    tcpSocket->write(ui->sendmessage->toPlainText().toUtf8());
    tcpSocket->flush();  //刷新缓存区
}

void MainWindow::socket_Disconnected(){

    //发送按键失能
    ui->send->setEnabled(false);
    //修改按键文字
    ui->connect->setText("connect");
    qDebug() << "Disconnected from server!";
    //QString str = ui->connectmessage->toPlainText();
   // str += "\r\n disconnected from server";
    ui->connectmessage->setText("disconnected from server");
    ui->sendmessage->setText("");
}

void MainWindow::socket_Read_Data(){
    QByteArray buffer;
    //读取缓冲区数据
    buffer = tcpSocket->readAll();
    if(!buffer.isEmpty())
    {
        QString str = ui->connectmessage->toPlainText();
        str += "\r\n";
        str+=tr(buffer);
        //刷新显示
        ui->connectmessage->setText(str);
    }
}

