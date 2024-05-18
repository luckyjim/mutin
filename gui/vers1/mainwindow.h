#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dialogadddetector.h"
#include "infowindow.h"
#include "dialogmodifynetwork.h"
#include "valuedslider.h"
#include "tensionwidget.h"
#include "dialogadddetector.h"
#include "detector.h"
#include "messagereader.h"
#include "MQenC.h"
#include <QMainWindow>
#include <QGraphicsScene>
#include <vector>
#include <QGraphicsView>
#include <iostream>
#include <QGraphicsTextItem>
#include <QLabel>
#include <sstream>
#include <cstdlib>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void newDetect();
    void modifNetwork();

    void openInfo1();
    void openInfo2();

    void paintTableGraphics();

    void onHtChanged(MessageMQ * message);
    void onDerChanged(MessageMQ  * message);

    void save();
    void load();

signals:
    void refreshNetwork(vector<Detector>);

private:
    Ui::MainWindow *ui;
    QGraphicsScene *m_pscene;
    vector<Detector> m_detectors;
    MessageReader * mReceiver;
};

#endif // MAINWINDOW_H
