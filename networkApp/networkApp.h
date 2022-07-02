#ifndef NETWORKAPP_H
#define NETWORKAPP_H

#include <QMainWindow>
#include <QtWidgets/QMainWindow>
#include <QtNetwork>
#include <QFileDialog>
#include <QPainter>
#include <QPolygon>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class networkApp; }
QT_END_NAMESPACE

class networkApp : public QMainWindow
{
    Q_OBJECT

public:
    networkApp(QWidget *parent = nullptr);
    ~networkApp();

private slots :
    void trySendFile();
    void displayError(QAbstractSocket::SocketError socketError);
    void clicked_select_file();
    void sendFile();
private:
    void paintEvent(QPaintEvent* event);
    void endSend();
    void updatePercent();
    void sendFile(const QString& filename);
    QTcpSocket* _socket;
    QFile* file;

    qint64 fileSize;
    qint64 sendSize;
    double percent;

private:
    Ui::networkApp *ui;
};
#endif // NETWORKAPP_H
