#include "sortermessage.h"
using namespace std;

int main()
{   
    std::ofstream fichier("testsimuGATE.txt", std::ios::out | std::ios::trunc);
    fichier << "" << endl;
    SorterMessage * sm = new SorterMessage();
    sm->run();
    delete sm;
}
