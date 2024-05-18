#include "test_detector.h"
#include <fstream>
#include <iostream>

test_detector::test_detector()
{
    det = new Detector(0,0,120,5,68,NULL);
}

bool test_detector::test_evet()
{
    std::cout << "============ DEBUT  test_evet ============" << std::endl;
    double triggAsked = det->tauxTrigger();
    int nb_gerbe = 0;
    int temps_ms = 600000; // 10min
    int nb_it = temps_ms / det->dt_ms();
    std::cout << "nombre iterations : " << nb_it << std::endl;

    for(int i=0; i < nb_it; i++)
    {
        if(det->evet())
        {
            nb_gerbe++;
        }
    }
    std::cout << "nombre evenements : " << nb_gerbe << std::endl;

    double result = nb_gerbe / (temps_ms/1000.0);
    double result_prct;
    if(result < triggAsked)
    {
        result_prct = (result / triggAsked) * 100;
    }
    else
    {
        result_prct = (triggAsked / result) * 100;
    }
    //std::cout << "---------------- RESULTAT ----------------" << std::endl;

    std::cout << "Fréquence d'évenements : " << result << "Hz / Fréquence demandée : "
              << triggAsked << std::endl;
    std::cout << "Soit un taux de reussite de : " << result_prct << "%" << std::endl;
    std::cout << "============= FIN  test_evet =============" << std::endl;
}

double test_detector::test_derive()
{
    std::cout << "=========== DEBUT  test_derive ===========" << std::endl;
    int64_t P = 600*2*1000;
    int64_t deltaT = 1000;
    int64_t nbit = P / deltaT;
    double t = 0;
    std::ofstream fichier("testsimuGATE_Detector.txt", std::ios::out | std::ios::trunc);
    for(int64_t i = 0; i < nbit; i++)
    {
        t += deltaT*1e6;
        double derive = det->derive(t);

        std::ofstream fichier("testsimuGATE_Detector.txt", std::ios::out | std::ios::app);
        fichier << t << " " << derive << std::endl;
    }
    std::cout << "============ FIN  test_derive ============" << std::endl;
}
