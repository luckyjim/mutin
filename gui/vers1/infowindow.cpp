#include "infowindow.h"
#include "ui_infowindow.h"

InfoWindow::InfoWindow(QString network, vector<Detector> pdetectors, MessageReader* pMesReader, QWidget *parent) : QWidget(parent), ui(new Ui::InfoWindow)
{
    ui->setupUi(this);

    pMySlider = ui->horizontalSlider;
    m_network = network;
    m_pscene = new QGraphicsScene(this);
    ui->graphicsView->setScene(m_pscene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    createGrid();
    refresh(pdetectors);

    connect(pMySlider,
            SIGNAL(valueChanged(int)),
            pMesReader,
            SLOT(setTimeSearchWindows(int)));
    //qt_assert(0, __FILE__, __LINE__);
}

InfoWindow::~InfoWindow()
{
    delete ui;
}

void InfoWindow::createGrid()
{
    QPen green(QColor(128,255,0));
    QPen yellow(QColor(255,255,0));
    QPen orange(QColor(255,128,0));
    QPen red(QColor(255,0,0));
    m_pscene->addEllipse(150, 150, 100, 100, green);
    m_pscene->addEllipse(100, 100, 200, 200, yellow);
    m_pscene->addEllipse(50, 50, 300, 300, orange);
    m_pscene->addEllipse(0, 0, 400, 400, red);
    m_pscene->addLine(0, 200, 400, 200);
    m_pscene->addLine(200,0,200,400);
}

void InfoWindow::refresh(vector<Detector> detectors)
{
    //Création de la liste des détécteurs présents dans le sous réseau
    QString text = "";
    for (int i = 0; i < detectors.size(); i++) {
        Detector current = (detectors)[i];
        if(current.getNetwork() == m_network) {
            text += current.getDetectorName();

            if(i != detectors.size()-1) {
                text += "/";
            }
        }
    }

    ui->label_4->setText(text);
}

void InfoWindow::printFront(qreal x, qreal y)
{
    pair<qreal,qreal> pos = make_pair(x,y);
    positions.push_back(pos);
    if(positions.size() > 5)
    {
        positions.pop_front();
    }
    m_pscene->clear();
    createGrid();
    QBrush brush(Qt::black);
    QPen pen(Qt::black);
    pen.setWidth(5);
    for(pair<qreal, qreal> p : positions)
    {
        m_pscene->addEllipse(p.first,p.second,1,1,pen,brush);
    }
}
