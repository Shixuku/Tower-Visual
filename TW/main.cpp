#include "InterFace.h"
#include <QApplication>
#include <QWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    InterFace w;
    w.show();
    return a.exec();
}
