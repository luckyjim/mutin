#ifndef INFOWINDOW_H
#define INFOWINDOW_H

#include "detector.h"
#include "messagereader.h"
#include <QWidget>
#include <QGraphicsScene>
#include <vector>
#include <deque>
#include <QGraphicsTextItem>
#include <QPen>
#include <QSlider>

using namespace std;
namespace Ui {
class InfoWindow;
}

class InfoWindow : public QWidget
{
    Q_OBJECT

public:
    explicit InfoWindow(QString network, vector<Detector> detectors, MessageReader *pMesReader, QWidget *parent = 0);
    ~InfoWindow();
    void createGrid();

public slots:
    void refresh(vector<Detector> detectors);
    void printFront(qreal x, qreal y);

private:
    Ui::InfoWindow *ui;
    QGraphicsScene *m_pscene;
    QString m_network;
    deque<pair<qreal, qreal> > positions;
    QSlider* pMySlider;

};

#endif // INFOWINDOW_H
