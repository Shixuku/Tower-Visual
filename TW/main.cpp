#include "InterFace.h"
#include <QApplication>
#include <QWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    InterFace w;
    w.showMaximized();//Ĭ�������ʾ
    return a.exec();
}
