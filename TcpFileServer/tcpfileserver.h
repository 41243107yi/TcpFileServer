#ifndef TCPFILESERVER_H
#define TCPFILESERVER_H

#include <QDialog>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QProgressBar>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QLineEdit>

class TcpFileServer : public QDialog
{
    Q_OBJECT

public:
    TcpFileServer(QWidget *parent = nullptr);
    ~TcpFileServer();

private slots:
    void start();
    void acceptConnection();
    void updateServerProgress();
    void displayError(QAbstractSocket::SocketError socketError);

private:
    QTcpServer tcpServer;
    QTcpSocket *tcpServerConnection;
    QFile *localFile;
    QProgressBar *serverProgressBar;
    QLabel *serverStatusLabel;
    QPushButton *startButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;
    QLineEdit *ipAddressEdit;
    QLineEdit *portNumberEdit;
    QString fileName;
    qint64 totalBytes;
    qint64 byteReceived;
    qint64 fileNameSize;
    QByteArray inBlock;
};

#endif // TCPFILESERVER_H
