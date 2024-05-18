#include "sortermessage.h"

SorterMessage::SorterMessage()
{
    m_fe = 100000000000; // [ps] Fenetre à examiner = 0,1s
    m_ms = 500000000000 ; // [ps] Marge de sécurité = 0,5s
    m_tfd = 500000; // [ps] Taille Fenetre Détection = définie dans l'IHM

    mqd_t mqd = init();

    Detector D1(0,0,101,1);
    Detector D2(0,0,102,1);
    Detector D3(0,0,103,1);
    Detector D4(0,0,104,1);
    Detector D5(0,0,105,1);
    Detector D6(0,0,106,1);
    Detector D7(0,0,107,1);
    Detector D8(0,0,108,1);

    detectors.push_back(D1);
    detectors.push_back(D2);
    detectors.push_back(D3);
    detectors.push_back(D4);
    detectors.push_back(D5);
    detectors.push_back(D6);
    detectors.push_back(D7);
    detectors.push_back(D8);

    sizelist = 0;
    sizeBuff = 0;
    g = 0;
}

SorterMessage::~SorterMessage()
{

}

void SorterMessage::run()
{
    int k = 0;
    MessageMQ mess;
    while(1)
    {
        //cout << "liste = " << receive.size() << endl;
        //usleep(100);
        mess.init(readMessage(mqd)); //initialisation des nouveaux messages entrants
        //cout << k << endl;
        routingMessages(mess);
        // Affichage du buffer
        /*cout << "============ before delete ============ " << endl;
        for (std::list<MessageMQ>::iterator it2 = receive.begin(); it2 != receive.end(); it2++)
            cout << "          name : " << static_cast<int16_t>((*it2)->name()) << " data : " << (*it2)->data() << endl;
        std::cout << endl;*/

        //########################################## Traitement des évenements stockés ##########################################
        analyse();
        //######################################## Fin traitement des évenements stockés ########################################
        //cout << "sizelist = " << receive.back()->data() << " - " << receive.front()->data() << " = " << sizelist  << "// comp to : " << sizeBuff << endl;

        //###################################### Suppression des éléments stockés inutiles ######################################

        deleteMessages();

        //#################################### Fin suppression des éléments stockés inutiles ####################################
        k++;

        /*cout << "============ after delete ============ " << endl;
        for (std::list<MessageMQ>::iterator it2 = receive.begin(); it2 != receive.end(); it2++)
            cout << "          name : " << static_cast<int16_t>((*it2).name()) << " data : " << (*it2).data() << endl;
        std::cout << endl;
        cout << "----------------------------------" << endl;*/
    }


    // ======================= test tri =============================//
    if(is_sorted(receive.begin(), receive.end(), [](MessageMQ a, MessageMQ b){
                                                    return a.data() < b.data();
                                                 }))
    {
        cout << "La liste est triée!" << endl;
    }
    else
    {
        cout << "Pas trié..." << endl;
    }
    cout << receive.back().data() - receive.front().data() << endl;
    // =====================fin test tri ============================//
}

void SorterMessage::routingMessages(MessageMQ message)
{
    if(message.type() == 0) //Si le message est de type 0 : evenement
    {
        int8_t detector = message.name();
        for(int i = 0; i < detectors.size(); i++) //Recherche du nom du detecteur correspondant pour y appliquer
        {                                         //la correction d'horloge
            if(detectors[i].getDetectorName() == detector)
            {
                //message.setData(message.data() - detectors[i].getCorrectHorloge()); //date d'évenement corrigé
            }
        }
        //cout << "à ajouter : " << pmess->data << endl;

        //######################## Ajout des evenements au buffer pour la détection des coincidences ########################
        tri(message);
        //##################################### Fin de l'ajout des evenements au buffer #####################################

    }
    else if(message.type() == 1) //messages de type 1 : correction d'horloge
    {
        int8_t detector = message.name();
        for(int i = 0; i < detectors.size(); i++)
        {
            if(detectors[i].getDetectorName() == detector)
            {
                //cout << "set new correction for : " << static_cast<int16_t>(detector) << " value : " << pmess->data <<endl;
                detectors[i].setCorrHorloge(message.data());
            }
        }
    }
    else if(message.type() == 2) // messages de type 2 : valeur de haute tension
    {
        //HT
    }
}

void SorterMessage::tri(MessageMQ message)
{
    if(receive.size() == 0 || message.data() < receive.front().data())
    {
        receive.push_front(message);
        //cout << "size null add : " << static_cast<int16_t>(pmess->telescope) << " , " << pmess->data << endl;
    }
       else if(message.data() > receive.back().data())
    {
        receive.push_back(message);
        //cout << "add back : " << static_cast<int16_t>(pmess->telescope) << " , " << pmess->data << endl;
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
    //cout << "Content = "<<endl;
}

void SorterMessage::analyse()
{
    if(receive.size() > 2)
    {
        //cout << "here 1"<< endl;
        sizelist = (receive.back().data()) - (receive.front().data());
        sizeBuff = m_ms + m_fe + m_tfd;

        if(sizelist >= sizeBuff)
        {
            //cout << "here 2"<< endl;
            //cout << "sizeList : " << sizelist << " , SizeBuff : " << sizeBuff << endl;
            int64_t time_begin_IAE = m_t - m_ms - m_fe - m_tfd;
            int64_t time_end_IAE = m_t - m_ms;
            bool begin_finded = false;
            bool end_finded = false;
            for(list<MessageMQ>::iterator it3 = receive.begin(); it3 != receive.end(); it3++)
            {
               // cout << "here 3"<< endl;
                if((*it3).data() >= time_begin_IAE && !begin_finded) //detection de la borne inferieure de IAE
                {
                    //cout << "here 4"<< endl;
                    begin_IAE = it3;
                    //cout << "begin finded : " << (*begin_IAE)->name() << " , " << (*begin_IAE)->data() << endl;
                    begin_finded = true;
                }

                if((*it3).data() >= time_end_IAE && !end_finded) //detection de la borne superieure de IAE
                {
                    //cout << "here 5"<< endl;
                    end_IAE = it3;
                    //cout << "end finded" << (*end_IAE)->name() << " , " << (*end_IAE)->data() << endl;
                    end_finded = true;
                }
                if(begin_finded && end_finded) break;
            }
            //cout << "here 6"<< endl;
            list<MessageMQ>::iterator pivot = begin_IAE;
            int64_t time_end_window = (*pivot).data() + m_tfd;

            for(list<MessageMQ>::iterator it3 = begin_IAE; it3 != end_IAE; ++it3)
            {
                //if(!contains(gerbe, it3))
                //cout << static_cast<int16_t>((*it3).name()) << endl;
                if((*it3).data() <= time_end_window && gerbe.size() <= 8)
                {
                    //cout << "add : name : " << (*it3).name() << " data : " << (*it3).data() << endl;
                    gerbe.push_back(*it3);
                }
                else
                {
                    //cout << (*it3).data() << " ? " << time_end_window << endl;
                    //cout << "New pivot" << endl;
                    if(gerbe.size() > 2)
                    {
                        std::ofstream fichier("testsimuGATE.txt", std::ios::out | std::ios::app);
                        fichier << "\n==============================================================================" << std::endl ;
                        g++;
                        gerbe.push_front(*pivot);
                        cout << "-----------------------------" << endl;

                        for(int i = 0; i < gerbe.size(); i++)
                        {

                            cout << "Gerbe : detecteur : " << gerbe[i].name() << " data : " << gerbe[i].data() << endl;
                            std::ofstream fichier("testsimuGATE.txt", std::ios::out | std::ios::app);
                            fichier << "detecteur : " << gerbe[i].name() << " data : " << gerbe[i].data() << " nume gerbe : " << g << endl;
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

void SorterMessage::deleteMessages()
{
    //cout << "gn?" << endl;
    //cout << "sizelist = " << sizelist << endl;
    sizelist = (receive.back().data()) - (receive.front().data());
    sizeBuff = m_ms + m_fe + m_tfd;
    if(sizelist > sizeBuff)
    {

        list<MessageMQ>::iterator begin_supp = receive.begin();
        list<MessageMQ>::iterator end_supp = receive.begin();
        //receive.erase(--it_last);
        while(sizelist > sizeBuff)
        {
            //cout<< "delete name : " << receive.front().name() << " value : " << receive.front().data() << endl;
            receive.pop_front();
            //cout << "supp" << endl;
            sizelist = (receive.back().data()) - (receive.front().data());
            //cout << "sizelist = " << receive.back().data() << " - " << receive.front().data() << " = " << sizelist  << "// comp to : " << sizeBuff << endl;
        }
    }
}

