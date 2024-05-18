#include "detector.h"
#include "dialogadddetector.h"

#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QString>
#include <iostream>
#include <QPainter>

using namespace std;

Detector::Detector(double x_dialog, double y_dialog, int8_t detector_name, QString network, QObject *parent)
{
    setCoords(x_dialog, y_dialog);
    m_detector_name = detector_name;
    m_network = network;
    m_triggPM1 = 0;
    m_triggPM2 = 0;

    //QGraphicsEllipseItem * led = new QGraphicsEllipseItem(40 - 12,2,10,10,this);


}

int8_t Detector::getDetectorName()
{
    return m_detector_name;
}

QString Detector::getNetwork()
{
    return m_network;
}

QString Detector::getParticipation()
{
    return m_participation;
}

QString Detector::getTauxTrigger()
{
    return m_taux_trigger;
}

int64_t Detector::getTension()
{
    return m_ht;
}

int64_t Detector::getCorrectHorloge()
{
    return m_correct_horloge;
}

void Detector::setDetectorName(int8_t name)
{
    m_detector_name = name;
}

void Detector::setNetwork(QString network)
{
    m_network = network;
}

void Detector::setCoords(double x_set, double y_set)
{
    std::pair<double, double> pair(metersToPix(x_set, y_set));
    m_x = pair.first;
    m_y = pair.second;
}

void Detector::setHT(int64_t ht)
{
    m_ht = ht;
}

void Detector::setCorrHorloge(int64_t correctHorloge)
{
    m_correct_horloge = correctHorloge;
}

std::pair<double,double> Detector::getCoords()
{
    return pixToMeters(m_x, m_y);
}

std::pair<double,double> Detector::getCoordsGraphics()
{
    std::pair<double, double> pair(m_x, m_y);
    return pair;
}

double Detector::getTriggPM1()
{
    return m_triggPM1;
}

double Detector::getTriggPM2()
{
    return m_triggPM2;
}

QString Detector::getCommandHT()
{
    return m_commandht;
}

int64_t Detector::getHT()
{
    return m_ht;
}

void Detector::setCommandHT(QString HT)
{
    m_commandht = HT;
}

void Detector::setTriggPM1(double triggPM1)
{
    m_triggPM1 = triggPM1;
}

void Detector::setTriggPM2(double triggPM2)
{
    m_triggPM2 = triggPM2;
}

std::pair<double,double> Detector::metersToPix(double x_meter, double y_meter)
{
    std::pair<double, double> pair(x_meter * 35, -y_meter * 35);
    return pair;
}

std::pair<double,double> Detector::pixToMeters(double x_pix, double y_pix)
{
    std::pair<double, double> pair(x_pix / 35, - y_pix / 35);
    return pair;
}
