#include "window.h"
#include "musicplay.h"
#include "mplayercall.h"
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include <QTextCodec>
#include "skindialog.h"

#define PC_DEBUG 0

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    win.setFont(QFont("wqy-zenhei",28*FONT_AMP,QFont::Normal));
   // QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());

    win.construction();
    musicPlay.construction();
    mplayerCall.construction();



#if PC_DEBUG
    QGraphicsScene *scene = new QGraphicsScene;
    QGraphicsProxyWidget *p = scene->addWidget(&win);
    p->setRotation(270);
    QGraphicsView *view = new QGraphicsView(scene);
    view->resize(240,420);
    view->show();
#else
    win.show();
#endif

    return a.exec();
}
