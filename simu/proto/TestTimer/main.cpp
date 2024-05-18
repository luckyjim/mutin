#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>
#include <thread>

using namespace std;


int main_first() {
    cout << "!!! Hello World !!!" << endl; // prints !!!Hello World!!!
    int a=0;
    chrono::milliseconds dura( 500);
    chrono::milliseconds duraExtra( 100);
//	auto curTime = chrono::duration_cast< chrono::milliseconds >(chrono::high_resolution_clock::now().time_since_epoch());
//	cout << curTime.time_since_epoch() << endl;
    auto t = chrono::high_resolution_clock::now().time_since_epoch();
    auto tcastStart =  chrono::duration_cast<chrono::milliseconds>(t);
    cout << t.count() << " " << tcastStart.count() <<"\n";
    auto tlast = tcastStart;

    while(1)
    {
        a += 1;
        t = chrono::high_resolution_clock::now().time_since_epoch();
        auto tm = chrono::duration_cast<chrono::milliseconds>(t);
        auto duree =  tm - tlast;
        auto dureeStart =  tm - tcastStart;

        tlast = tm;
        cout << a << " "<< duree.count() << "ms, depuis debut: "<<  dureeStart.count() << "ms"<<  "\n";
        this_thread::sleep_for( dura );
        if (a%3 == 0) {
            this_thread::sleep_for( duraExtra );
        }
    }
    return 0;
}

int main() {
    int a=0;
    chrono::milliseconds timerPeriod( 500);
    chrono::milliseconds duraExtra( 100);
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

    while(1)
    {
        a++;

        this_thread::sleep_until(tNext);
        tNext += timerPeriod;
        // check time wake up
        t = chrono::high_resolution_clock::now().time_since_epoch();
        tm = chrono::duration_cast<chrono::milliseconds>(t);
        cout << "\ntime wake up: " << tm.count()%100000 << "ms" << "\n";
        duree =  tm - tlast;
        dureeStart =  tm - tcastStart;
        tlast = tm;
        cout << a << " "<< duree.count() << "ms, depuis debut: "<<  dureeStart.count() << "ms"<<  "\n";
        // do some work
        if (a%3 == 0)
        {
            this_thread::sleep_for( duraExtra );
            cout << "do long work" << "\n";
        }
        // check time sleep
        t = chrono::high_resolution_clock::now().time_since_epoch();
        tm = chrono::duration_cast<chrono::milliseconds>(t);
        cout << "time sleep  : " << tm.count()%100000 << "ms" << "\n";
    }
    return 0;
}
