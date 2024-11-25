#ifndef TCPFILESENDER_H
#define TCPFILESENDER_H

#include <QDialog>
#include <QTcpSocket>
#include <QFile>
#include <QProgressBar>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QDataStream>
#include <QHostAddress>
#include <QLineEdit>

class TcpFileSender : public QDialog
{
    Q_OBJECT

public:
    TcpFileSender(QWidget *parent = nullptr);
    ~TcpFileSender();

private slots:
    void openFile();
    void start();
    void startTransfer();
    void updateClientProgress(qint64 numBytes);

private:
    QTcpSocket tcpClient;
    QFile *localFile;
    QProgressBar *clientProgressBar;
    QLabel *clientStatusLabel;
    QPushButton *startButton;
    QPushButton *quitButton;
    QPushButton *openButton;
    QDialogButtonBox *buttonBox;
    QLineEdit *ipAddressEdit;
    QLineEdit *portNumberEdit;
    QString fileName;
    qint64 totalBytes;
    qint64 bytesWritten;
    qint64 bytesToWrite;
    qint64 loadSize;
    QByteArray outBlock;
};

#endif // TCPFILESENDER_H
