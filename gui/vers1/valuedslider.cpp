#include "valuedslider.h"

ValuedSlider::ValuedSlider(Qt::Orientation orientation, Detector *detector, QWidget *parent) : QWidget(parent)
{
    m_pdetector = detector;

    //Création de la grille
    QGridLayout * grid = new QGridLayout(this);
    grid->setMargin(0);

    //Création du slider et du label du PM1
    m_pSliderPm1 = new QSlider(orientation);
    m_pSliderPm1->setValue(m_pdetector->getTriggPM1());

    m_pLabPm1 = new QLabel();
    m_pLabPm1->setText(QString::number(m_pdetector->getTriggPM1()));

    //Création du slider et du label du PM2
    m_pSliderPm2 = new QSlider(orientation);
    m_pSliderPm2->setValue(detector->getTriggPM2());

    m_pLabPm2 = new QLabel();
    m_pLabPm2->setText(QString::number(detector->getTriggPM2()));

    //Ajout à la grille
    grid->addWidget(m_pSliderPm1, 0, 1);
    grid->addWidget(m_pLabPm1, 0, 0);
    grid->addWidget(m_pSliderPm2, 0, 3);
    grid->addWidget(m_pLabPm2, 0, 2);

    //Connecteurs
    connect(m_pSliderPm1, SIGNAL(valueChanged(int)), this, SLOT(valueChangePM1(int)));
    connect(m_pSliderPm2, SIGNAL(valueChanged(int)), this, SLOT(valueChangePM2(int)));
    connect(m_pSliderPm1, SIGNAL(sliderReleased()), this, SLOT(sendValuePM1()));
    connect(m_pSliderPm2, SIGNAL(sliderReleased()), this, SLOT(sendValuePM2()));
}

void ValuedSlider::valueChangePM1(int value)
{
    m_pLabPm1->setText(QString::number(value));
}

void ValuedSlider::valueChangePM2(int value)
{
    m_pLabPm2->setText(QString::number(value));
    //m_pdetector->setTriggPM2(value);
}

void ValuedSlider::sendValuePM1()
{
    m_pdetector->setTriggPM1(m_pSliderPm1->value());
}

void ValuedSlider::sendValuePM2()
{
    m_pdetector->setTriggPM2(m_pSliderPm2->value());
}

