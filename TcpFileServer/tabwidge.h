#ifndef TABWIDGE_H
#define TABWIDGE_H

#include <QDialog>

class Tabwidge : public QDialog
{
    Q_OBJECT

public:
    Tabwidge(QWidget *parent = nullptr);
    ~Tabwidge(); // 添加析構函數的聲明
};

#endif // TABWIDGE_H
