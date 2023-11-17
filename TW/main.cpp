#include "InterFace.h"
#include <QApplication>
#include <QWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    InterFace w;
    w.showMaximized();//默认最大显示
    return a.exec();
}
