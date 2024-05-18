#ifndef TEST_DETECTOR_H
#define TEST_DETECTOR_H
#include "../vers1/detector.h"

class test_detector
{
public:
    test_detector();
    bool test_evet();
    double test_derive();

private:
    Detector *det;
};

#endif // TEST_DETECTOR_H
