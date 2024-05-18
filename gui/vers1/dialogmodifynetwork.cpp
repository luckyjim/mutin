#include "dialogmodifynetwork.h"
#include "ui_dialogmodifynetwork.h"


DialogModifyNetwork::DialogModifyNetwork(vector<Detector> *pdetectors, QWidget *parent) : QDialog(parent), ui(new Ui::DialogModifyNetwork)
{
    ui->setupUi(this);
    this->m_pdetectors = pdetectors;
    int row = 1;
    while(row < ui->gridLayout->rowCount())
    {
        for(int col = 0; col < ui->gridLayout->columnCount(); col++ )
        {

            QLayoutItem* item = ui->gridLayout->itemAtPosition(row,col);
            QWidget* widget = item->widget();
            if (widget) {
                if(row > m_pdetectors->size() && row < 9) {
                    widget->setEnabled(false);
                }
                else if(row <= m_pdetectors->size() && row > 0)
                {
                    std::pair<double, double> coords = (*m_pdetectors)[row-1].getCoords();
                    if(col == 0)
                    {
                        QSpinBox *le = qobject_cast<QSpinBox*>(widget);
                        le->setEnabled(true);
                        le->setValue((*m_pdetectors)[row-1].getDetectorName());
                    }
                    if(col == 1)
                    {
                        QDoubleSpinBox *ds = qobject_cast<QDoubleSpinBox*>(widget);
                        ds->setEnabled(true);
                        ds->setValue(coords.first);
                    }
                    if(col == 2)
                    {
                        QDoubleSpinBox *ds = qobject_cast<QDoubleSpinBox*>(widget);
                        ds->setEnabled(true);
                        ds->setValue(coords.second);
                    }
                    if(col == 3)
                    {
                        QComboBox *ds = qobject_cast<QComboBox*>(widget);
                        ds->setEnabled(true);
                        ds->setCurrentText((*m_pdetectors)[row-1].getNetwork());
                    }
                    if(col == 4)
                    {
                        widget->setEnabled(true);
                    }
                }
            }
        }
        row++;
    }

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(onAccept()));

}

DialogModifyNetwork::~DialogModifyNetwork()
{
    delete ui;
}

void DialogModifyNetwork::onAccept()
{
    double x = 0;
    double y = 0;
    int row = 1;
    int supp = 0;
    int row_max = m_pdetectors->size();
    while(row <= row_max)
    {
        for(int col = ui->gridLayout->columnCount()-1; col >= 0; col-- )
        {
            QLayoutItem* item = ui->gridLayout->itemAtPosition(row,col);
            QWidget* widget = item->widget();
            if (widget)
            {
                if(col == 4)
                {
                    QCheckBox *cb = qobject_cast<QCheckBox*>(widget);
                    if(cb->isChecked())
                    {
                        m_pdetectors->erase(m_pdetectors->begin()+row -1 - supp);
                        supp++;
                        break;
                    }
                }
                else if(col == 3)
                {
                    QComboBox *ds = qobject_cast<QComboBox*>(widget);
                    (*m_pdetectors)[row-1].setNetwork(ds->currentText());
                }
                else if(col == 2)
                {
                    QDoubleSpinBox *ds = qobject_cast<QDoubleSpinBox*>(widget);
                    y = ds->value();
                }
                else if(col == 1)
                {
                    QDoubleSpinBox *ds = qobject_cast<QDoubleSpinBox*>(widget);
                    x = ds->value();
                }
                else if(col == 0)
                {
                    QSpinBox *le = qobject_cast<QSpinBox*>(widget);
                    (*m_pdetectors)[row-1].setDetectorName(le->text().toInt());
                    (*m_pdetectors)[row-1].setCoords(x, y);
                }

            }
        }
        row++;
    }
}
