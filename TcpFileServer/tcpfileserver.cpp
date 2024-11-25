#include "tcpfileserver.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QDebug>

#define tr QStringLiteral

TcpFileServer::TcpFileServer(QWidget *parent)
    : QDialog(parent), totalBytes(0), byteReceived(0), fileNameSize(0), localFile(nullptr)
{
    // 新增 IP 與埠號的輸入框
    ipAddressEdit = new QLineEdit("127.0.0.1"); // 預設為本機地址
    portNumberEdit = new QLineEdit("16998");   // 預設埠號
    ipAddressEdit->setPlaceholderText(tr("輸入 IP 地址"));
    portNumberEdit->setPlaceholderText(tr("輸入埠號"));

    serverProgressBar = new QProgressBar;
    serverStatusLabel = new QLabel(tr("伺服器端就緒"));
    startButton = new QPushButton(tr("接收"));
    quitButton = new QPushButton(tr("退出"));
    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(startButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(serverProgressBar);
    mainLayout->addWidget(serverStatusLabel);
    mainLayout->addWidget(ipAddressEdit);
    mainLayout->addWidget(portNumberEdit);
    mainLayout->addStretch();
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
    setWindowTitle(tr("接收檔案"));

    connect(startButton, &QPushButton::clicked, this, &TcpFileServer::start);
    connect(quitButton, &QPushButton::clicked, this, &TcpFileServer::close);
    connect(&tcpServer, &QTcpServer::newConnection, this, &TcpFileServer::acceptConnection);
    connect(&tcpServer, QOverload<QAbstractSocket::SocketError>::of(&QTcpServer::acceptError),
            this, &TcpFileServer::displayError);
}

TcpFileServer::~TcpFileServer()
{
    if (localFile) {
        localFile->close();
        delete localFile;
    }
}

void TcpFileServer::start()
{
    startButton->setEnabled(false);
    byteReceived = 0;
    fileNameSize = 0;

    QString ipAddress = ipAddressEdit->text();
    quint16 port = portNumberEdit->text().toUShort();

    QHostAddress address(ipAddress);
    if (!tcpServer.isListening() && !tcpServer.listen(address, port)) {
        QMessageBox::StandardButton ret = QMessageBox::critical(
            this,
            tr("啟動錯誤"),
            tr("無法啟動伺服器: %1.").arg(tcpServer.errorString()),
            QMessageBox::Retry | QMessageBox::Cancel);

        if (ret == QMessageBox::Cancel) {
            startButton->setEnabled(true);
            return;
        }
    }
    serverStatusLabel->setText(tr("監聽中於 %1:%2").arg(ipAddress).arg(port));
}

void TcpFileServer::acceptConnection()
{
    tcpServerConnection = tcpServer.nextPendingConnection();
    connect(tcpServerConnection, &QTcpSocket::readyRead, this, &TcpFileServer::updateServerProgress);
    connect(tcpServerConnection, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
            this, &TcpFileServer::displayError);
    serverStatusLabel->setText(tr("接受連線"));
    tcpServer.close(); // 暫停接受客戶端連線
}

void TcpFileServer::updateServerProgress()
{
    QDataStream in(tcpServerConnection);
    in.setVersion(QDataStream::Qt_4_6);
    if (byteReceived <= sizeof(qint64) * 2) {
        if ((fileNameSize == 0) && (tcpServerConnection->bytesAvailable() >= sizeof(qint64) * 2)) {
            in >> totalBytes >> fileNameSize;
            byteReceived += sizeof(qint64) * 2;
        }
        if ((fileNameSize != 0) && (tcpServerConnection->bytesAvailable() >= fileNameSize)) {
            in >> fileName;
            byteReceived += fileNameSize;
            localFile = new QFile(fileName);
            if (!localFile->open(QFile::WriteOnly)) {
                QMessageBox::warning(this, QStringLiteral("應用程式"),
                                     QStringLiteral("無法讀取檔案 %1：\n%2.").arg(fileName)
                                         .arg(localFile->errorString()));
                return;
            }
        } else {
            return;
        }
    }

    if (byteReceived < totalBytes) {
        byteReceived += tcpServerConnection->bytesAvailable();
        inBlock = tcpServerConnection->readAll();
        localFile->write(inBlock);
        inBlock.resize(0);
    }

    serverProgressBar->setMaximum(totalBytes);
    serverProgressBar->setValue(byteReceived);
    qDebug() << byteReceived;
    serverStatusLabel->setText(QStringLiteral("已接收 %1 Bytes").arg(byteReceived));

    if (byteReceived == totalBytes) {
        tcpServerConnection->close();
        startButton->setEnabled(true);
        localFile->close();
        serverStatusLabel->setText(tr("接收完成"));
    }
}

void TcpFileServer::displayError(QAbstractSocket::SocketError /*socketError*/)
{
    QObject *server = qobject_cast<QObject *>(sender());
    if (server == tcpServerConnection) qDebug() << "Hi I am QTcpSocket";
    if (server == &tcpServer) qDebug() << "Hi I am QTcpServer";

    QMessageBox::information(this, tr("網絡錯誤"),
                             tr("產生如下錯誤: %1.")
                                 .arg(tcpServerConnection->errorString()));
    if (tcpServerConnection) {
        tcpServerConnection->close();
    }
    serverProgressBar->reset();
    serverStatusLabel->setText(tr("伺服器就緒"));
    startButton->setEnabled(true);
}
