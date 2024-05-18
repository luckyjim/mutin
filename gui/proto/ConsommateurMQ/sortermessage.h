#ifndef SORTERMESSAGE_H
#define SORTERMESSAGE_H

#include "MQenC.h"
#include "detector.h"
#include "messagemq.h"
#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <vector>
#include <stdio.h>
#include <fstream>
#include <list>
#include <algorithm>
#include <deque>

using namespace std;

class SorterMessage
{
public:
    SorterMessage();
    ~SorterMessage();
    void run();
    void routingMessages(MessageMQ message);
    void tri(MessageMQ message);
    void analyse();
    void deleteMessages();
    //bool contains(deque<MessageMQ> gerbe)

private:
    int64_t m_fe; // [ps] Fenetre à examiner = 0,1s
    int64_t m_ms; // [ps] Marge de sécurité = 0,5s
    int64_t m_tfd; // [ps] Taille Fenetre Détection = définie dans l'IHM
    int64_t m_t; // Temps présent = dernier message reçu

    deque<MessageMQ> gerbe;

    mqd_t mqd = init();

    vector<Detector> detectors;

    list<MessageMQ> receive;
    list<MessageMQ>::iterator it;

    int64_t sizelist;
    int64_t sizeBuff;
    list<MessageMQ>::iterator begin_IAE;
    list<MessageMQ>::iterator end_IAE;
    list<MessageMQ>::iterator it_last;

    int g;

};

#endif // SORTERMESSAGE_H
