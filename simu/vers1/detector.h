#ifndef DETECTOR_H
#define DETECTOR_H

//#include <cstring>
#include <functional>
#include <random>
#include <cstdint>
#include "MQenC.h"

class Detector
{
public:
    Detector(double m_x, double m_y, double m_tauxTrigger, int dt, int8_t m_name, mqd_t mqd);

    bool evet();
    void infos();

    double derive(double t0);
    void writeMQ(int64_t date, int type);
    void writeFile(int64_t date, int type, int idGerbe);
    void writeHTMQ(int ht);
    void writeHTFile(int ht);
    void writeMQDer(double t);
    void writeFileDer(double t);

    double x() const;
    void setX(double x);
    double y() const;
    void setY(double y);
    int8_t name() const;
    void setName(const int8_t &name);
    double dt_ms() const;
    void setDt_ms(double dt_ms);
    double tauxTrigger() const;
    void setTauxTrigger(double tauxTrigger);

private:

    double m_x;             //Position x [m]
    double m_y;             //Position y [m]
    int8_t m_name;          //Nom du detecteur
    double m_dt_ms;         // [ms]
    double m_tauxTrigger;   // taux de trigger [Hz]

    // model correction clock constants
    int m_A ; // amplitude en ns
    int m_B ; // mean value en ns
    double m_T ; // periode en s
    double m_w; // pulsation

    int gen;
    int dis;
    int np;
    int a;
    // Génération aléatoire
    std::mt19937 m_mt;
    std::uniform_int_distribution<int32_t> m_intDist;
    int m_fact = 10; //Facteur permettant d'apporter plus de précision
    int m_sizeInterval; //taille de l'interval aléatoire [ms]

    mqd_t m_mqd; //identifiant du MQ
};

#endif // DETECTOR_H
