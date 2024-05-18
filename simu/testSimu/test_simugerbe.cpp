#include "test_simugerbe.h"
#include <algorithm>

test_simugerbe::test_simugerbe(vector<Detector*> det, int dt)
{
    test_simu = new SimuGerbe(det, dt);
    dt_ms = dt;
}

bool test_simugerbe::test_is_gerbe()
{
    std::cout << "========== DEBUT  test_is_gerbe ==========" << std::endl;
    double triggAsked = test_simu->triggerGerbe();
    int nb_gerbe = 0;
    int temps_ms = 600000; // 10min
    int nb_it = temps_ms / dt_ms;
    std::cout << "nombre iterations : " << nb_it << std::endl;

    for(int i=0; i < nb_it; i++)
    {
        if(test_simu->is_gerbe())
        {
            nb_gerbe++;
        }
    }
    std::cout << "nombre gerbes : " << nb_gerbe << std::endl;

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

    std::cout << "Fréquence de gerbe : " << result << "Hz / Fréquence demandée : "
              << triggAsked << std::endl;
    std::cout << "Soit un taux de reussite de : " << result_prct << "%" << std::endl;
    std::cout << "=========== FIN  test_is_gerbe ===========" << std::endl;
}

void test_simugerbe::test_gerbe()
{
    std::cout << "============ DEBUT test_gerbe ============" << std::endl;
    int nb_it = 1000;
    double fail = 0.0;
    for(int i = 0; i < nb_it; i++)
    {
        vector<Message> gerbe = test_simu->simu_gerbe(100);
        sort(gerbe.begin(), gerbe.end(), [](const Message a, const Message b)
        {
            return a.data < b.data;
        });
        string order = "";
        for(int j = 0; j < gerbe.size(); j++)
        {
            order += gerbe[j].telescope;
        }
        cout << "ordre : " << order << endl;
        for(Message m : gerbe)
        {
            cout << "nom" << static_cast<int16_t>(m.telescope) << " valeur : " << m.data << endl;
        }
        if(i%4 == 0)
        {
            if(order != "ABDC" && order != "ADBC")
            {
                fail += 1;
                //cout << "Erreur, la gerbe : " << i << " donne : " << order << " attendu : ABDC" << endl;
            }
        }
        else if(i%4 == 1)
        {
            if(order != "BACD" && order != "BCAD")
            {
                fail += 1;
                //cout << "Erreur, la gerbe : " << i << " donne : " << order << " attendu : BACD" << endl;
            }
        }
        else if(i%4 == 2)
        {
            if(order != "CBDA" && order != "CDBA")
            {
                fail += 1;
                //cout << "Erreur, la gerbe : " << i << " donne : " << order << " attendu : CBDA" << endl;
            }
        }
        else if(i%4 == 3)
        {
            if(order != "DCAB" && order != "DACB")
            {
                fail += 1;
                //cout << "Erreur, la gerbe : " << i << " donne : " << order << " attendu : DCAB" << endl;
            }
        }
    }

    cout << "Réussite de " << nb_it - fail << " tests sur " << nb_it << endl;
    cout << "Soit " << ((nb_it - fail) / nb_it) * 100 << "%" << endl;

    std::cout << "============= FIN test_gerbe =============" << std::endl;
}


