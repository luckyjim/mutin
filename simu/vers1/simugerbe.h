#ifndef SIMUGERBE_H
#define SIMUGERBE_H

#include "detector.h"
#include "MQenC.h"
#include <vector>
#include <stdint.h>
#include <random>

class SimuGerbe
{
public:
    SimuGerbe(std::vector<Detector*> det, int dt /* [ms] */);
    std::vector<Message> simu_gerbe(int64_t t);
    bool is_gerbe();
    Detector *firstTouch();

    double triggerGerbe() const;
    void setTriggerGerbe(double triggerGerbe);

private:
    std::vector<Detector *> m_detectors;
    std::vector<Message> m_gerbe;

    // nombre de gerbe par seconde
    double m_triggerGerbe = 2.5; // [Hz]

    double m_A;     //azimuth [rad]
    int m_h;        //hauteur [rad]
    double m_a;     //?? du vecteur n
    double m_b;     //?? du vecteur n
    int m_i;        // Nombre de passage dans la simulation
    double m_speed; //vitesse du front de muons (0,997c) [m/s]
    double m_dt_ms;

    int m_SizeInterval;
    std::mt19937 m_mt;
    std::uniform_int_distribution<int32_t> m_intDist;
    int m_fact = 10;



};

#endif // SIMUGERBE_H
