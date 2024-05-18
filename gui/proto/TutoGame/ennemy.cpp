#include "ennemy.h"
#include <QTimer>
#include <QGraphicsScene>
#include <QList>
#include <stdlib.h>

Ennemy::Ennemy(): QObject(), QGraphicsRectItem()
{
    int random_number = rand() % 700;
    setPos(random_number, 0);
    setRect(0,0,100,100);
    QTimer * timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this,SLOT(move()));

    timer->start(50);
}

void Ennemy::move()
{
    setPos(x(), y()+5);
}
