#include "tensionwidget.h"

TensionWidget::TensionWidget(Detector *detector, QWidget *parent) : QWidget(parent)
{
    m_pdetector = detector;

    //Création de la grille
    QGridLayout * grid = new QGridLayout(this);
    grid->setMargin(0);

    m_pCurrentTens = new QLabel();
    m_pCurrentTens->setText(QString::fromStdString(std::to_string(m_pdetector->getHT())));

    m_pHT = new QLineEdit();
    m_pHT->setText(m_pdetector->getCommandHT());
    m_pHT->setInputMask("0000;");


    QString str = "OK";
    m_pbutton = new QPushButton(str);

    grid->addWidget(m_pCurrentTens, 0, 0);
    grid->addWidget(m_pHT, 0, 1);
    grid->addWidget(m_pbutton, 0, 3);

    connect(m_pHT, SIGNAL(returnPressed()), this, SLOT(setCommandHT()));
    connect(m_pbutton, SIGNAL(clicked(bool)), this, SLOT(setCommandHT()));
}

void TensionWidget::setCommandHT()
{
    QString text = "";
    if((text = m_pHT->text()) != m_pdetector->getCommandHT() && !m_pHT->text().isEmpty())
    {//Vérifications ici
        m_pdetector->setCommandHT(text);
        std::cout << text.toStdString() << std::endl;
    }
}

QLabel* TensionWidget::getLabel()
{
    return m_pCurrentTens;
}

Detector* TensionWidget::getDetector()
{
    return m_pdetector;
}




