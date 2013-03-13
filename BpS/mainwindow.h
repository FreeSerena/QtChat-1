#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui/QMainWindow>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <QPushButton>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QTime>
#include <QKeyEvent>

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setupUI();

private:
    QTcpServer *server;
    QTcpSocket *clientSocket;
    QTime now;
    QString chatLog;

    QPushButton *sendButton;
    QPushButton *exitButton;

    QTextEdit *displayTextEdit;
    QTextEdit *inputTextEdit;

    QVBoxLayout *mainVLayout;
    QHBoxLayout *botHLayout;

    QWidget *widget;

public slots:
    void recvMsg();
    void sendMsg();
    void sockDisconnected();
    void setupNewConn();

protected:
    bool eventFilter(QObject *obj, QEvent *e);
};

#endif // MAINWINDOW_H
