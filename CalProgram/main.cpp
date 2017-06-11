#include "calprogram.h"
#include <QtGui/QApplication>
#include <QTextCodec>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("System"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("System"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("System"));

    CalProgram w;
    w.show();
    return a.exec();
}
