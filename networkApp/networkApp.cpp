#include "networkapp.h"
#include "ui_networkapp.h"

#define BUFFER_SIZE 1024

networkApp::networkApp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::networkApp)
{
    ui->setupUi(this);

    percent = 0;
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(clicked_select_file()));

    _socket = new QTcpSocket(this);
    connect(_socket, SIGNAL(connected()), this, SLOT(trySendFile()));
    connect(_socket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
}


void networkApp::displayError(QAbstractSocket::SocketError socketError) {
    qDebug() << "Error";
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        QMessageBox::information(this, "Error", "The host closed");
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, "Error", "The host was not found");
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, "Error", "The connection was refused by the peer.");
        break;
    default :
        QMessageBox::information(this, "Error", "The following error occurred: " + _socket->errorString());
    }
}

void networkApp::clicked_select_file() {
    qDebug() << "try to connect";
    _socket->connectToHost(QHostAddress::LocalHost, 6666);
}

void networkApp::trySendFile() {
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), "/home");

    if (!filename.isEmpty()) {
        sendFile(filename);
    }
}

void networkApp::endSend() {
    _socket->close();

    qDebug() << "disconnect";
}

void networkApp::sendFile(const QString& fileName) {
    if (file != NULL) {
        return;
    }
    file = new QFile(fileName);

    sendSize = 0;
    updatePercent();

    if (file->open(QFile::ReadOnly)) {
        QByteArray data;
        QDataStream out(&data, QIODevice::WriteOnly);

        QFileInfo fileInfo(fileName);
        fileSize = file->size();

        out << file->size() << fileInfo.fileName();
        qDebug() << file->size() << fileInfo.fileName();

        _socket->write(data);
        _socket->waitForBytesWritten();
        connect(_socket, SIGNAL(bytesWritten(qint64)), this, SLOT(sendFile()));
        sendFile();
    }
}

void networkApp::sendFile() {
    char buffer[BUFFER_SIZE];
    memset(buffer, '\0', BUFFER_SIZE);

    if (!file->atEnd()) {
        qint64 in = file->read(buffer, BUFFER_SIZE);
        sendSize += in;
        _socket->write(buffer, in);
        updatePercent();
    }
    else {
        sendSize = 0;
        file->close();
        file = NULL;
        disconnect(_socket, SIGNAL(bytesWritten(qint64)), this, SLOT(sendFile()));
        endSend();
    }
}

void networkApp::updatePercent() {
    percent = static_cast<double> (sendSize) / static_cast<double> (fileSize);
    //qDebug() << percent << "percent";
    this->update();
}

void networkApp::paintEvent(QPaintEvent* event) {
    QPainter painter;
    painter.begin(this);
    painter.setPen(Qt::gray);
    QRect rec (ui->centralwidget->x() + 10, ui->centralwidget->y() + 10, ui->centralwidget->x() + 410, ui->centralwidget->y() + 30);
    painter.drawRect(rec);
    painter.setBrush(Qt::green);
    painter.drawPolygon(QPolygon() << QPoint(rec.x() + rec.width() * percent, rec.y()) << QPoint(rec.x(), rec.y()) << QPoint(rec.x(), rec.y() + rec.height()) << QPoint(rec.x() + rec.width() * percent, rec.y() + rec.height()));
    painter.end();
}

networkApp::~networkApp()
{
    delete ui;
}

