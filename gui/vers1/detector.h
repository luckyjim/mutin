#ifndef DETECTOR_H
#define DETECTOR_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QString>

class Detector
{
    //Q_OBJECT
public:
    //constructeur
    explicit Detector(double x_dialog, double y_dialog, int8_t detector_name, QString network, QObject *parent = 0);

    //Fonctions de calcul des coordonées
    std::pair<double, double> metersToPix(double x_meter, double y_meter);
    std::pair<double, double> pixToMeters(double x_pix, double y_pix);

    //getters
    int8_t getDetectorName();
    QString getNetwork();
    std::pair<double, double> getCoords();
    std::pair<double, double> getCoordsGraphics();
    QString getParticipation();
    QString getTauxTrigger();
    int64_t getTension();
    int64_t getCorrectHorloge();
    double getTriggPM1();
    double getTriggPM2();
    QString getCommandHT();
    int64_t getHT();

    //setters (slots inclus)
    void setDetectorName(int8_t name);
    void setNetwork(QString network);
    void setCoords(double x_set, double y_set);
    void setHT(int64_t ht);
    void setCorrHorloge(int64_t correctHorloge);

public slots:
    void setCommandHT(QString HT);
    void setTriggPM1(double triggPM1);
    void setTriggPM2(double triggPM2);

private:
    double m_x;
    double m_y;
    int8_t m_detector_name;
    QString m_network;
    QString m_participation;
    QString m_taux_trigger;
    int64_t m_ht;
    QString m_commandht;
    int64_t m_correct_horloge;
    double m_triggPM1;
    double m_triggPM2;
};

#endif // DETECTOR_H
