#include <iostream>
#include <unistd.h>
#include <vector>
#include <stdio.h>
#include <fstream>

#include <ctime>
#include <ratio>
#include <chrono>
#include <thread>

#include "detector.h"
#include "simugerbe.h"
#include "MQenC.h"

#include <mqueue.h>

#include <algorithm>

using namespace std;

int main() {
    mqd_t mqd = init();
    int64_t deltaTms = 5;
    int m_ngerbe = 0;
    // JMC possibilite d'utiliser fonctionnalite C++11 de vector emplace_back
    // pour avoir une structure de donnees sans pointeur
    // vector<Detector> detectors;

    vector<Detector*> detectors;
    // JMC si tous les Detector ont le mqd il est possible de le
    // passer en variable statique
    detectors.push_back(new Detector(0, 10, 103,deltaTms,101, mqd));
    detectors.push_back(new Detector(10, 0, 85,deltaTms, 102, mqd));
    detectors.push_back(new Detector(20, 0, 123,deltaTms, 103, mqd));
    detectors.push_back(new Detector(10, 20, 115,deltaTms, 104, mqd));
    detectors.push_back(new Detector(20, 20, 108,deltaTms, 105, mqd));
    detectors.push_back(new Detector(0, 0, 100,deltaTms, 106, mqd));
    detectors.push_back(new Detector(5, -10, 90.5,deltaTms, 107, mqd));
    detectors.push_back(new Detector(-10, 10, 98.2,deltaTms, 108, mqd));
    SimuGerbe simu = SimuGerbe(detectors, deltaTms);

    chrono::milliseconds timerPeriod( deltaTms);
    chrono::seconds second( 1);

    // wait to next second
    auto tStart_0 = chrono::high_resolution_clock::now();
    auto tcastStart_0 =  chrono::duration_cast<chrono::microseconds>(tStart_0.time_since_epoch());
    cout << "Time start: " << tcastStart_0.count() << "\n";
    auto tcastStartSecondp =  chrono::duration_cast<chrono::seconds>(tcastStart_0) + second;
    auto tcastStartSecondpmicro =  chrono::duration_cast<chrono::microseconds>(tcastStartSecondp);
    this_thread::sleep_for(tcastStartSecondpmicro - tcastStart_0);
    // next start
    auto tStart = chrono::high_resolution_clock::now();
    auto tcastStart =  chrono::duration_cast<chrono::milliseconds>(tStart.time_since_epoch());
    cout << "new time start: " << tcastStart.count() << "\n";
    auto tNext = tStart + timerPeriod;


    auto tlast = tcastStart;
    auto t = chrono::high_resolution_clock::now().time_since_epoch();
    auto tm = chrono::duration_cast<chrono::milliseconds>(t);
    auto duree =  tm - tlast;
    auto dureeStart =  tm - tcastStart;
    tlast = tm;

    std::ofstream fichier("testsimuGATE.txt", std::ios::out | std::ios::trunc);
    fichier << "" << endl;
    //fichier << "Nom détecteur" << "\t" << "Temps (ps)" << "\tType message" << "\tN° gerbe"<< std::endl ;

    // time random in deltaT [ps]
    auto randTime_ps = std::uniform_int_distribution<int64_t>(0, deltaTms*1000000000-1);
    std::mt19937 generatorTime;
    generatorTime.seed(10);
    int64_t dureeStart_ps;
    int64_t evetTime_ps;
    int ht;
    uniform_int_distribution<int> distr = uniform_int_distribution<int>(0,10);
    mt19937 mt;
    mt.seed(0);
    while(1)
    {
        //sleep(1);
        this_thread::sleep_until(tNext);
        tNext += timerPeriod;
        // check time wake up
        t = chrono::high_resolution_clock::now().time_since_epoch();
        tm = chrono::duration_cast<chrono::milliseconds>(t);
        //cout << "\ntime wake up: " << tm.count()%100000 << "ms" << "\n";
        duree =  tm - tlast;
        dureeStart =  tm - tcastStart;
        tlast = tm;

        //std::ofstream fichier("testsimuGATE.txt", std::ios::out | std::ios::app);
        //fichier << "\n==============================================================================" << std::endl ;

        dureeStart_ps = static_cast<int64_t>(dureeStart.count());
        dureeStart_ps *= 1000000000;

        // JMC nouvelle syntaxe C++11 pour parcourir un contener
        // for(auto detec : detectors)
        for(int i = 0; i < detectors.size(); i++)        
        {
            evetTime_ps = dureeStart_ps + randTime_ps(generatorTime);
            if(detectors[i]->evet())
            {
                detectors[i]->writeMQ(evetTime_ps, 0);
                detectors[i]->writeFile(evetTime_ps, 0, 0);
            }
            //cout <<"\ntime = " << (dureeStart.count() - duree.count())%1000 << endl;
            // JMC
            // auto diff_duree = (dureeStart.count() - duree.count())%1000
            if((dureeStart.count() - duree.count())%1000 == 0)
            {
                ht = (i+1) * 100 + distr(mt);
                detectors[i]->writeHTMQ(ht);
                detectors[i]->writeHTFile(ht);
            }
            if((dureeStart.count() - duree.count())%1000 == 500)
            {
                detectors[i]->writeMQDer(evetTime_ps);
                detectors[i]->writeFileDer(evetTime_ps);
            }
        }
        if (simu.is_gerbe())
        {
            std::cout << "gerbe" << std::endl;
            m_ngerbe++;
            evetTime_ps = dureeStart_ps + randTime_ps(generatorTime);
            vector<Message> gerbe = simu.simu_gerbe(evetTime_ps);
            std::ofstream fichier("testsimuGATE.txt", std::ios::out | std::ios::app);
            //fichier << "\n==============================================================================" << std::endl ;
            for(Message m : gerbe)
            {
                std::cout << "taille gerbe : " << gerbe.size() << std::endl;
                sendMessage(m, mqd);
                std::cout << m_ngerbe << std::endl;
                std::cout << "gerbe send : " << m.telescope << " value : " << m.data << std::endl;
                std::cout << m_ngerbe << endl;

                sort(gerbe.begin(), gerbe.end(), [](Message a, Message b){return a.data < b.data;});
                for(Detector* d : detectors)
                {
                    if(d->name() == m.telescope)
                    {
                        d->writeFile(m.data, m.identifiant, m_ngerbe);
                    }
                }
                cout << "";
                //std::ofstream fichier("testsimuGATE.txt", std::ios::out | std::ios::app);
                //fichier << static_cast<int16_t>(m.telescope) << "aaaaaaaaaaa\t\t\t\t\t" << m.data << "\t\t\t" << m.identifiant << "\t\t\t" << m_ngerbe << std::endl ;
            }
        }

        t = chrono::high_resolution_clock::now().time_since_epoch();
        tm = chrono::duration_cast<chrono::milliseconds>(t);
    }
    return 0;
}

