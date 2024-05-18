#ifndef DETECTOR_H
#define DETECTOR_H

#include <cinttypes>

class Detector
{
public:
    Detector(double x, double y, int8_t name, int8_t network);

    void setHT(int8_t ht);
    void setCorrHorloge(int64_t correctHorloge);

    int8_t getDetectorName();
    int64_t getCorrectHorloge();

private:
    double m_x;
    double m_y;
    int8_t m_detector_name;
    int8_t m_network;
    double m_participation;
    double m_taux_trigger;
    int8_t m_ht;
    int64_t m_correct_horloge;
};

#endif // DETECTOR_H
