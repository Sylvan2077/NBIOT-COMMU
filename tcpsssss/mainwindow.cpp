#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
     port = 8888;
     ui->lineEdit->setText(QString::number(port));//界面中端口默认显示8888

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonCreateChattingRoom_clicked()
{
    server  = new Server(this, port);
    connect(server, &Server::updateserver, this, &MainWindow::slotupdateserver);
    ui->pushButtonCreateChattingRoom->setEnabled(false);
}

void MainWindow::slotupdateserver(QString msg)
{
    int clr;
   // tcpclientsocketlist.at(i)
    if(msg=="1:Enter") {
        ui->textEdit->append(msg);
    }
  //      clr=1;
    if(msg=="2:Enter") {
        ui->textEdit_2->append(msg);
    }
    else
        ui->textEdit->append(msg);
        //clr=2;
    /*
    switch(clr)
       {
       case 1:
    {
       ui->textEdit->append(msg);
          break;
    }

       case 2:
    {
        ui->textEdit_2->append(msg);
          break;
    }
    //ui->textEdit->append(msg1);
    // ui->textEdit_2->append(msg2);
    default:
        ui->textEdit->append(msg);

}
*/
}
