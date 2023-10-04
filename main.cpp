#include <QApplication>
#include "IceMark.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    IceMark w;
    w.show();
    return a.exec();
}
