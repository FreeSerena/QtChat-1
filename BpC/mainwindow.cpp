#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    clientSocket = new QTcpSocket();
    clientSocket->connectToHost("127.0.0.1", 8888);

    connect(clientSocket, SIGNAL(connected()), this, SLOT(sockConnected()));
    connect(clientSocket, SIGNAL(disconnected()), this, SLOT(sockDisconnected()));
    connect(sendButton, SIGNAL(clicked()), this, SLOT(sendMsg()));
    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(recvMsg()));
    connect(exitButton, SIGNAL(clicked()), this, SLOT(close()));
    inputTextEdit->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::setupUI()
{
    sendButton = new QPushButton("Send");
    exitButton = new QPushButton("Exit");

    displayTextEdit = new QTextEdit();
    inputTextEdit = new QTextEdit();

    mainVLayout = new QVBoxLayout();
    botHLayout = new QHBoxLayout();

    botHLayout->addStretch();
    botHLayout->addWidget(sendButton);
    botHLayout->addWidget(exitButton);

    mainVLayout->addWidget(displayTextEdit);
    mainVLayout->addWidget(inputTextEdit);
    mainVLayout->addLayout(botHLayout);

    widget = new QWidget(this);
    widget->setLayout(mainVLayout);
    setCentralWidget(widget);

    sendButton->setAutoDefault(true);
    sendButton->setDefault(true);
    sendButton->setDisabled(true);

    displayTextEdit->setText("connecting...");
    displayTextEdit->setReadOnly(true);

    inputTextEdit->setDisabled(true);
    setWindowTitle("Bump Chat (\"Connecting...\")");
    inputTextEdit->setMaximumHeight(100);
    inputTextEdit->setMinimumHeight(100);

    resize(460, 360);
}

void MainWindow::sockConnected()
{
    displayTextEdit->setText("");
    setWindowTitle("Bump Chat (\"Connected!\")");
    sendButton->setDisabled(false);
    inputTextEdit->setDisabled(false);
}

void MainWindow::sockDisconnected()
{
    displayTextEdit->setText("Server Disconnected!");
    setWindowTitle("Bump Chat (\"Disconnected!\")");
    sendButton->setDisabled(true);
    inputTextEdit->setDisabled(true);
}

void MainWindow::sendMsg()
{
    QString s = inputTextEdit->toPlainText();
    if(s != "")
    {
        clientSocket->write(s.toLocal8Bit());
        displayTextEdit->append(QString("<font color=\"gray\">你说(%1)</font>").arg(now.currentTime().toString()));
        displayTextEdit->append(QString("<font color=\"blue\">%1</font>\n").arg(s));
        inputTextEdit->setText("");
    }
}

void MainWindow::recvMsg()
{
    QByteArray s;
    s = clientSocket->readAll();
    displayTextEdit->append(QString("<font color=\"gray\">对方说(%1)</font>").arg(now.currentTime().toString()));
    displayTextEdit->append(QString("<font color=\"green\">%1</font>\n").arg(QString(s)));
}

bool MainWindow::eventFilter(QObject *obj, QEvent *e)
{
    Q_ASSERT(obj == inputTextEdit);
    if (e->type() == QEvent::KeyPress)
    {
        QKeyEvent *event = static_cast<QKeyEvent*>(e);
        if (event->key() == Qt::Key_Return && (event->modifiers() & Qt::ControlModifier))
        {
            sendMsg();
            return true;
        }
    }
    return false;
}
