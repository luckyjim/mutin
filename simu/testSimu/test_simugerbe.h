#ifndef TEST_SIMUGERBE_H
#define TEST_SIMUGERBE_H

#include <cstdint>
#include "../vers1/simugerbe.h"
#include "../vers1/detector.h"
#include <vector>
#include <iostream>

using namespace std;

class test_simugerbe
{
public:
    test_simugerbe(vector<Detector*> det, int dt);
    bool test_is_gerbe();
    void test_gerbe();
    Detector* firstTouch();
    static bool compStruct(const Message &a, const Message &b);

private:
    SimuGerbe *test_simu;
    int64_t dt_ms;
};

#endif // TEST_SIMUGERBE_H
