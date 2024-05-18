#ifndef TENSIONWIDGET_H
#define TENSIONWIDGET_H

#include "detector.h"
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QString>
#include <iostream>

class TensionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TensionWidget(Detector *detector, QWidget *parent = 0);
    QLabel *getLabel();
    Detector *getDetector();

signals:

private slots:
    void setCommandHT();

private:
    QLabel *m_pCurrentTens;
    QLineEdit *m_pHT;
    QPushButton *m_pbutton;
    Detector *m_pdetector;
};

#endif // TENSIONWIDGET_H
