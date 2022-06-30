#pragma once

#include <QtWidgets/QMainWindow>
#include <QtNetwork>
#include <QFileDialog>
#include <QPainter>
#include <QPolygon>
#include <QMessageBox>
#include "ui_networkApp.h"

class networkApp : public QMainWindow
{
    Q_OBJECT

public:
    networkApp(QWidget *parent = nullptr);
    ~networkApp();
    void paintEvent(QPaintEvent* event);

private slots :
    void trySendFile();
    void displayError(QAbstractSocket::SocketError socketError);
    void clicked_select_file();
    void sendFile();
private:
    void endSend();
    void updatePercent();
    void sendFile(const QString& filename);
    QTcpSocket* _socket;
    QFile* file;

    qint64 fileSize;
    qint64 sendSize;
    double percent;

    Ui::networkAppClass ui;
};
