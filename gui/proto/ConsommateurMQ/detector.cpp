#include "detector.h"

Detector::Detector(double x, double y, int8_t name, int8_t network)
{
    m_x = x;
    m_y = y;
    m_detector_name = name;
    m_network = network;
}

void Detector::setHT(int8_t ht)
{
    m_ht = ht;
}

void Detector::setCorrHorloge(int64_t correctHorloge)
{
    m_correct_horloge = correctHorloge;
}

int8_t Detector::getDetectorName()
{
    return m_detector_name;
}

int64_t Detector::getCorrectHorloge()
{
    return m_correct_horloge;
}

