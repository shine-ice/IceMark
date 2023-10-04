//
// Created by Wsy on 2023/9/25.
//

#ifndef ICEMARK_ICEMARK_H
#define ICEMARK_ICEMARK_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include "marktools.h"

class IceMark : public QWidget {
    Q_OBJECT

public:
    IceMark(QWidget *parent = nullptr);
    ~IceMark();

private slots:
    void markImage();

private:
    QPushButton *chooseImageBtn;
};


#endif //ICEMARK_ICEMARK_H
