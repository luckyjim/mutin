#include "dialogadddetector.h"
#include "ui_dialogadddetector.h"

DialogAddDetector::DialogAddDetector(vector<Detector> detectors, QWidget *parent) : QDialog(parent), ui(new Ui::DialogAddDetector)
{
    ui->setupUi(this);
    m_detectors = detectors;
}

DialogAddDetector::~DialogAddDetector()
{
    delete ui;
}

double DialogAddDetector::getX() {
    return m_x;
}

double DialogAddDetector::getY() {
    return m_y;
}

QString DialogAddDetector::getDetector()
{
    return m_detectorName;
}

QString DialogAddDetector::getNetwork()
{
    return m_network;
}

void DialogAddDetector::done(int r)
{
    if(QDialog::Accepted == r)
    {
        bool detExist = false;
        for(int i = 0; i < m_detectors.size(); i++)
        {
            m_detectors[i].getDetectorName();
            if(QString("%1").setNum(ui->spinBox->value()) == QString::fromStdString(to_string(m_detectors[i].getDetectorName())))
            {
                detExist = true;
            }
        }
        if(!detExist)
        {
            m_x = ui->doubleSpinBox->value();
            m_y = ui->doubleSpinBox_2->value();
            m_detectorName = QString("%1").setNum(ui->spinBox->value());
            m_network = ui->comboBox->currentText();
            QDialog::done(r);
            return;
        }
        else
        {
            ui->label_7->setText("Ce nom existe déjà.");
        }

    }
    else
    {
        QDialog::done(r);
        return;
    }
}
