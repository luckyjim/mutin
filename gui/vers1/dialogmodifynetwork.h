#ifndef DIALOGMODIFYNETWORK_H
#define DIALOGMODIFYNETWORK_H

#include "detector.h"
#include <QDialog>
#include <vector>
#include <iostream>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QCheckBox>
using namespace std;
namespace Ui {
class DialogModifyNetwork;
}

class DialogModifyNetwork : public QDialog
{
    Q_OBJECT

public:
    explicit DialogModifyNetwork(vector<Detector> * pdetectors, QWidget *parent = 0);
    ~DialogModifyNetwork();

public slots:
    void onAccept();

private:
    Ui::DialogModifyNetwork *ui;
    vector<Detector> * m_pdetectors;
};

#endif // DIALOGMODIFYNETWORK_H
