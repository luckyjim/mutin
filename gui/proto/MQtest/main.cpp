#include <iostream>
#include "MQenC.h"

using namespace std;


int main()
{
    cout << "\n========> DEBUT" << endl;

    sendMessageExample("test");
    readMessageExample();

    cout << "\n========> FIN" << endl;
    return 0;
}


