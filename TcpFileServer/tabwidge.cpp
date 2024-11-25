#include "tabwidge.h"
#include <QVBoxLayout>
#include <QTabWidget>
#include "tcpfilesender.h"
#include "tcpfileserver.h"

Tabwidge::Tabwidge(QWidget *parent) : QDialog(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    QTabWidget *tabWidget = new QTabWidget;
    tabWidget->addTab(new TcpFileSender, QStringLiteral("傳送檔案"));
    tabWidget->addTab(new TcpFileServer, QStringLiteral("接收檔案"));

    mainLayout->addWidget(tabWidget);
    setWindowTitle(QStringLiteral("TCP檔案傳輸"));
}

Tabwidge::~Tabwidge()
{
    // 釋放資源，如果有需要的話
}
