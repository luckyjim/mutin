#include <QApplication>
#include <QtGui>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPen>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    // jmc toto
    QGraphicsScene scene;
    //scene.addText("Hello world !");
    scene.addEllipse(0, 0, 100, 100);
    QPen pen(Qt::blue);
    scene.addEllipse(-50,0,100,100, pen);
    scene.addEllipse(50, 0, 100, 100);
    scene.addEllipse(0, 50, 100, 100);
    scene.addEllipse(0, -50, 100, 100);
    scene.addEllipse(35, 35, 100, 100);
    scene.addEllipse(-35, -35, 100, 100);
    //scene.addEllipse(0, 35, 100, 100);
    //scene.addEllipse(35, 35, 100, 100);

    QGraphicsView vue(&scene);
    vue.show();

    return app.exec();
}

