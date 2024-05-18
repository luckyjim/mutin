#include <QApplication>
#include <QGraphicsScene>
#include "myrect.h"
#include <QGraphicsView>
#include <QTimer>

int main(int argc, char *argv[])
{
    // program Axel
    QApplication a(argc, argv);

    QGraphicsScene *scene = new QGraphicsScene();
    MyRect *player = new MyRect();
    player->setRect(0,0,100,100);

    scene->addItem(player);

    player->setFlag(QGraphicsItem::ItemIsFocusable);
    player->setFocus();
    QGraphicsView *view = new QGraphicsView(scene);

    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    view->setFixedSize(800,600);
    scene->setSceneRect(0,0,800,600);
    player->setPos(view->width()/2 - player->rect().width() / 2, view->height() - player->rect().height() - 5);

    view->show();

    QTimer *timer = new QTimer();
    QObject::connect(timer,SIGNAL(timeout()),player,SLOT(spawn()));
    timer->start(2000);

    return a.exec();
}
