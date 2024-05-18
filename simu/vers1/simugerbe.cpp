#include "simugerbe.h"

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <stdio.h>
#include <string>
#include <sstream>

#include <fstream>

SimuGerbe::SimuGerbe(std::vector<Detector *> det, int dt)
{
    // JMC commentaire sur les unites
    m_detectors = det;

    m_dt_ms = dt;

    m_SizeInterval = 1000.0/(dt);
    m_intDist = std::uniform_int_distribution<int32_t>(0, m_SizeInterval*m_fact);
    m_mt.seed( m_detectors[0]->x() );
    m_i = 0;
    m_h = 0;
    m_speed = 0.997*299792458;
    m_speed *= 1e-12; // [m/ps]
}

bool SimuGerbe::is_gerbe()
{
    int32_t r = m_intDist(m_mt);
    return m_triggerGerbe*m_fact > r;
}

std::vector<Message> SimuGerbe::simu_gerbe(int64_t t)
{
    // JMC commentaire sur les unites
    Message message;
    // JMC passer gerbe en données membre m_gerbe
    // et vider ici
    m_gerbe.clear();
    int dir;
    if (m_i % 4 == 0)
    {
        dir = 0;
    }
    else if (m_i % 4 == 1)
    {
        dir = 90;
    }
    else if (m_i % 4 == 2)
    {
        dir = 180;
    }
    else if (m_i % 4 == 3)
    {
        dir = 270;
    }
    m_i++;
    //std::cout << "Direction : " << dir << std::endl;
    m_A = (M_PI*dir)/180;
    //A = (M_PI*(rand() % 360))/180;
    //int h = rand() % 91;
    m_a = cos(m_A) * cos(m_h);
    m_b = sin(m_A) * cos(m_h);

    double tfirst = t + (m_dt_ms/2);

    Detector* first = firstTouch();

    for(int i = 0; i < m_detectors.size(); i++)
    {
        int64_t tiR = (((m_a * (first->x() - m_detectors[i]->x()) + m_b * (first->y() - m_detectors[i]->y()))) / m_speed);
        //std::cout << "\nDetector : " << static_cast<int16_t>(detectors[i]->name) << "\n Delta T = " << tiR << std::endl;

        tiR += tfirst;
        //tiR +=  m_detectors[i]->derive(tfirst);

        message.identifiant = 0;
        message.telescope = m_detectors[i]->name();
        message.data = tiR;
        m_gerbe.push_back(message);
    }
    return m_gerbe;
}

Detector* SimuGerbe::firstTouch()
{
    bool out = false;
    int first = 0;
    for(int i = 0; i < m_detectors.size(); i++)
    {
        out = true;
        for(int j = 0; j < m_detectors.size(); j++)
        {
            if(j != i)
            {
                //std::cout << "a = " << a << " b = " << b << std::endl;
                double x = m_detectors[j]->x() - m_detectors[i]->x();
                double y = m_detectors[j]->y() - m_detectors[i]->y();
                double res = x * m_a + y * m_b;
                //std::cout << detectors[i]->name << detectors[j]->name << " : " << "\n\tx :" << detectors[i]->x << " y : " << detectors[i]->y
                //          << "\n\tres = " << res << " current : " << detectors[i]->name << std::endl;
                if(res > 0)
                {
                    //std::cout << "break" << std::endl;
                    out = false;
                    break;
                }
            }
        }
        if(out)
        {
            first = i;
            break;
        }
    }

    //std::cout << "First : " << detectors[first]->name << std::endl;
    return m_detectors[first];
}
double SimuGerbe::triggerGerbe() const
{
    return m_triggerGerbe;
}

void SimuGerbe::setTriggerGerbe(double triggerGerbe)
{
    m_triggerGerbe = triggerGerbe;
}



