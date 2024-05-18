#ifndef ENNEMY_H
#define ENNEMY_H
#include <QObject>
#include <QGraphicsRectItem>

class Ennemy: public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    Ennemy();

public slots:
    void move();
};

#endif // ENNEMY_H
