#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QTcpServer   *tcpServer;
    QList<QTcpSocket *> ClientSocketList;

private slots:
    void on_pushButton_clicked();
    void acceptConnection();
    void readData();
    void on_toAllClients_clicked();
    void on_toSingleClient_clicked();

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
