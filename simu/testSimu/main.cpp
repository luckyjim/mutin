#include <iostream>
#include "test_simugerbe.h"
#include "test_detector.h"

using namespace std;

int main()
{
    int64_t deltaTms = 5;
    vector<Detector*> detectors;
    detectors.push_back(new Detector(1, 0, 90.5,deltaTms, 65, NULL));
    detectors.push_back(new Detector(0, 1, 103,deltaTms,66, NULL));
    detectors.push_back(new Detector(-1, 0, 85,deltaTms, 67, NULL));
    detectors.push_back(new Detector(0, -1, 123,deltaTms, 68, NULL));

    test_simugerbe * simu_gerbe = new test_simugerbe(detectors, deltaTms);
    test_detector * test_det = new test_detector();

    std::cout << "########## DEBUT test_simugerbe ##########" << std::endl;
    simu_gerbe->test_is_gerbe();
    simu_gerbe->test_gerbe();
    std::cout << "########### FIN test_simugerbe ###########" << std::endl << std::endl;

    std::cout << "########## DEBUT  test_detector ##########" << std::endl;
    test_det->test_derive();
    test_det->test_evet();
    std::cout << "########### FIN  test_detector ###########" << std::endl;
}

