#include "InterFace.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    InterFace w;
    w.show();
    return a.exec();
}
