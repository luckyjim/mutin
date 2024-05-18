#include <iostream>
#include <cstdlib>
#include <cmath>
#include "detector.h"
#include <fstream>
#include <assert.h>

Detector::Detector(double x, double y, double tauxTrigger, int dt, int8_t name, mqd_t mqd)
{
    m_x = x;
    m_y = y;
    m_tauxTrigger = tauxTrigger;
    m_name = name;
    m_dt_ms = dt;
    m_sizeInterval = 1000.0/(dt);
    m_intDist = std::uniform_int_distribution<int32_t>(0, m_sizeInterval*m_fact);
    m_mt.seed(name);
    m_A = 3;
    m_B = 10;
    //T = 600000000000;
    m_T = 6e11; //10 min en ns
    m_w = (2 * M_PI) / m_T ;
    //std::cout << "w = " << w;

    m_mqd = mqd;
}


double Detector::derive(double t)
{
    double c = m_A * cos(m_w * t) + m_B;
    //std::cout << "t = " << t << " w = " << m_w << " derive = " << c << std::endl;
    return c;
}


void Detector::writeMQ(int64_t date, int type)
{
    Message message;
    message.identifiant = type;
    message.telescope = m_name;
    message.data = date;
    sendMessage(message, m_mqd);
    std::cout << "send " << static_cast<int16_t>(m_name) << " value " << date << std::endl;
}

void Detector::writeFile(int64_t date, int type, int idGerbe)
{
    std::ofstream fichier("testsimuGATE.txt", std::ios::out | std::ios::app);
    fichier << "\n" << static_cast<int16_t>(m_name) << "\t" << date << "\t" << type << "\t" << idGerbe ;
}

void Detector::writeHTMQ(int ht)
{
    Message message;
    message.identifiant = 2;
    message.telescope = m_name;
    message.data = ht;
    //std::cout << "send " << m_name << " value " << ht << std::endl;
    sendMessage(message, m_mqd);
}

void Detector::writeHTFile(int ht)
{
    std::ofstream fichier("testsimuGATE.txt", std::ios::out | std::ios::app);
    fichier << "\n" << static_cast<int16_t>(m_name) << "\t" << ht << "\t" << 2 ;
}

void Detector::writeMQDer(double t)
{
    Message message;
    message.identifiant = 1;
    message.telescope = m_name;
    message.data = derive(t)*1000;
    sendMessage(message, m_mqd);
}

void Detector::writeFileDer(double t)
{
    std::ofstream fichier("testsimuGATE.txt", std::ios::out | std::ios::app);
    fichier << "\n" << static_cast<int16_t>(m_name) << "\t" << derive(t) << "\t" << 1 ;
}



bool Detector::evet()
{
    int32_t r = m_intDist(m_mt);
    return (m_tauxTrigger*m_fact > r);
}
double Detector::tauxTrigger() const
{
    return m_tauxTrigger;
}

void Detector::setTauxTrigger(double tauxTrigger)
{
    m_tauxTrigger = tauxTrigger;
}

double Detector::dt_ms() const
{
    return m_dt_ms;
}

void Detector::setDt_ms(double dt_ms)
{
    m_dt_ms = dt_ms;
}

int8_t Detector::name() const
{
    return m_name;
}

void Detector::setName(const int8_t &name)
{
    m_name = name;
}

double Detector::y() const
{
    return m_y;
}

void Detector::setY(double y)
{
    m_y = y;
}

double Detector::x() const
{
    return m_x;
}

void Detector::setX(double x)
{
    m_x = x;
}

