#ifndef VALUEDSLIDER_H
#define VALUEDSLIDER_H

#include "detector.h"
#include <QSlider>
#include <QLabel>
#include <iostream>
#include <QGridLayout>

class ValuedSlider : public QWidget
{
    Q_OBJECT

public:
    ValuedSlider(Qt::Orientation orientation, Detector *detector, QWidget *parent = 0);
    void setPM1(int value);
    void setPM2(int value);

private slots:
    void valueChangePM1(int value);
    void valueChangePM2(int value);
    void sendValuePM1();
    void sendValuePM2();

private:
    QLabel *m_pLabPm1;
    QLabel *m_pLabPm2;
    QSlider *m_pSliderPm1;
    QSlider *m_pSliderPm2;

    Detector * m_pdetector;
};

#endif // VALUEDSLIDER_H
