#include <QApplication>
#include "tabwidge.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Tabwidge tabWidget;
    Tabwidge tabWidget2;
    tabWidget.show();

    tabWidget2.show();
   tabWidget.move(25, 25);
    return app.exec();
}
