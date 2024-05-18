#ifndef DIALOGADDDETECTOR_H
#define DIALOGADDDETECTOR_H

#include <QDialog>
#include <QString>
#include <iostream>
#include <vector>
#include "detector.h"
//#include "mainwindow.h"
using namespace std;
namespace Ui {
class DialogAddDetector;
}

class DialogAddDetector : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddDetector(vector<Detector> detectors, QWidget *parent = 0);
    ~DialogAddDetector();
    double getX();
    double getY();
    QString getDetector();
    QString getNetwork();
    void done(int r);

private:
    Ui::DialogAddDetector *ui;
    double m_x;
    double m_y;
    QString m_detectorName;
    QString m_network;
    vector<Detector> m_detectors;
};

#endif // DIALOGADDDETECTOR_H
