#include "messagereader.h"

MessageReader::MessageReader(vector<Detector> detectors, QObject * parent)
{

    m_fe = 100000000000; // [ps] Fenetre à examiner = 0,1s
    m_ms = 500000000000 ; // [ps] Marge de sécurité = 0,5s
    m_tfd = 500000; // [ps] Taille Fenetre Détection = définie dans l'IHM

    mqd = init();

    sizelist = 0;
    sizeBuff = 0;
    //g = 0;
}

void MessageReader::run()
{
    int k = 0;
    MessageMQ mess;
    while(1)
    {
        mess.init(readMessage(mqd)); //initialisation des nouveaux messages entrants

        routingMessages(mess); //Traitement des messages par les bonnes fonctions

        //Traitement des évenements stockés pour détecter les coincidences
        analyse();

        //Suppression des éléments stockés inutiles (trop anciens)
        deleteMessages();
    }
}

void MessageReader::routingMessages(MessageMQ message)
{
    if(message.type() == 0) //Si le message est de type 0 : evenement
    {
        int8_t detector = message.name();
        for(int i = 0; i < detectors.size(); i++) //Recherche du nom du detecteur correspondant pour y appliquer
        {                                         //la correction d'horloge
            if(detectors[i].getDetectorName() == detector)
            {
                message.setData(message.data() - detectors[i].getCorrectHorloge()); //date d'évenement corrigé
            }
        }

        //Ajout des evenements au buffer pour la détection des coincidences
        tri(message);

    }
    else if(message.type() == 1) //messages de type 1 : correction d'horloge
    {
        MessageMQ*  pCmes = new(MessageMQ);
        *pCmes = message;

        cout << "RECEIVED CORRECT HORLOGE" << endl;
        for(int i = 0; i < detectors.size(); i++)
        {
            if(detectors[i].getDetectorName() == message.name())
            {
                detectors[i].setCorrHorloge(message.data());
            }
            cout << "RECEIVED CORRECT HORLOGE" << endl;
        }
        emit derChange(pCmes);
    }
    else if(message.type() == 2) // messages de type 2 : valeur de haute tension
    {
        MessageMQ*  pCmes = new(MessageMQ);
        *pCmes = message;
        cout << "RECEIVED HT" <<  pCmes->data() << endl;
        emit htChange(pCmes);
    }
}

void MessageReader::tri(MessageMQ message)
{
    if(receive.size() == 0 || message.data() < receive.front().data())
    {
        receive.push_front(message);
    }
       else if(message.data() > receive.back().data())
    {
        receive.push_back(message);
    }
    else
    {
        it = receive.end();
        while(message.data() > (*it).data())
        {
            it++;
        }
        receive.insert(it, message);
    }
    m_t = receive.back().data();
}

void MessageReader::analyse()
{
    if(receive.size() > 2)
    {
        sizelist = (receive.back().data()) - (receive.front().data());
        sizeBuff = m_ms + m_fe + m_tfd;

        if(sizelist >= sizeBuff)
        {
            int64_t time_begin_IAE = m_t - m_ms - m_fe - m_tfd;
            int64_t time_end_IAE = m_t - m_ms;
            bool begin_finded = false;
            bool end_finded = false;
            for(list<MessageMQ>::iterator it3 = receive.begin(); it3 != receive.end(); it3++)
            {
                if((*it3).data() >= time_begin_IAE && !begin_finded) //detection de la borne inferieure de IAE
                {
                    begin_IAE = it3;
                    begin_finded = true;
                }

                if((*it3).data() >= time_end_IAE && !end_finded) //detection de la borne superieure de IAE
                {
                    end_IAE = it3;
                    end_finded = true;
                }
                if(begin_finded && end_finded) break;
            }
            list<MessageMQ>::iterator pivot = begin_IAE;
            int64_t time_end_window = (*pivot).data() + m_tfd;

            for(list<MessageMQ>::iterator it3 = begin_IAE; it3 != end_IAE; ++it3)
            {
                if((*it3).data() <= time_end_window && gerbe.size() <= 8)
                {
                    gerbe.push_back(*it3);
                }
                else
                {
                    if(gerbe.size() > 2)
                    {
                        //std::ofstream fichier("testsimuGATE.txt", std::ios::out | std::ios::app);
                        //fichier << "\n==============================================================================" << std::endl ;
                        //g++;
                        gerbe.push_front(*pivot);
                        cout << "-----------------------------" << endl;

                        // Appeller ici une fonction de calcul de l'entrée du front

                        /* ================================
                         * Génère et renvoie une position aléatoire d'entreé du front de muons
                         * ================================
                         */
                        int high = 15600;
                        int low = 0;
                        int pixelMax = 400;
                        double phi = fmod(double(qrand()),(2 * M_PI));
                        qreal x = qrand() % (high + 1);
                        x = pixelMax * qSin(high * x / 10000.0);
                        x = (x * qCos(phi)+400) / 2;
                        qreal y = qrand() % (high + 1);
                        y = pixelMax * qSin(high * y / 10000.0);
                        y = (y * qSin(phi)+400) / 2;

                        cout << "sended position front" << endl;
                        emit(positionFront(x,y));

                        for(int i = 0; i < gerbe.size(); i++)
                        {
                            cout << "Gerbe : detecteur : " << gerbe[i].name() << " data : " << gerbe[i].data() << endl;
                            //std::ofstream fichier("testsimuGATE.txt", std::ios::out | std::ios::app);
                            //fichier << "detecteur : " << gerbe[i].name() << " data : " << gerbe[i].data() << " nume gerbe : " << /*g <<*/ endl;
                        }
                    }
                    gerbe.clear();
                    pivot = it3;
                    time_end_window = (*pivot).data() + m_tfd;


                }
                receive.pop_front();
            }
        }
    }
}

void MessageReader::deleteMessages()
{
    sizelist = (receive.back().data()) - (receive.front().data());
    sizeBuff = m_ms + m_fe + m_tfd;
    if(sizelist > sizeBuff)
    {

        list<MessageMQ>::iterator begin_supp = receive.begin();
        list<MessageMQ>::iterator end_supp = receive.begin();
        while(sizelist > sizeBuff)
        {
            receive.pop_front();
            sizelist = (receive.back().data()) - (receive.front().data());
        }
    }
}

void MessageReader::setTimeSearchWindows(int tfd)
{
    m_tfd = 1000*tfd;
    //qt_assert(0,__FILE__, __LINE__);
}

