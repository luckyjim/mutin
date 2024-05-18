#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    mReceiver = new MessageReader(m_detectors, this);
    connect(mReceiver, SIGNAL(htChange(MessageMQ *)), this, SLOT(onHtChanged(MessageMQ*)));
    connect(mReceiver, SIGNAL(derChange(MessageMQ *)), this, SLOT(onDerChanged(MessageMQ *)));
    mReceiver->start();

    //Init basics of the window
    ui->setupUi(this);
    m_pscene = new QGraphicsScene(this);
    ui->centralWidget->setWindowTitle("Gate");
    ui->graphicsView->setScene(m_pscene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    //Init tableWidget
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setRowCount(0);

    //Raccourcis
    ui->actionQuiter->setShortcut(QKeySequence("Ctrl+Q"));
    ui->actionAjouter_d_tecteur->setShortcut(QKeySequence("Ctrl+N"));
    ui->actionModifier_r_seau->setShortcut(QKeySequence("Ctrl+M"));
    ui->actionEnregistrer->setShortcut(QKeySequence("Ctrl+S"));

    //Connectors to actions
    connect(ui->actionAjouter_d_tecteur, SIGNAL(triggered()), this, SLOT(newDetect()));
    connect(ui->actionModifier_r_seau, SIGNAL(triggered()), this, SLOT(modifNetwork()));
    connect(ui->actionQuiter, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(ui->actionInformations_r_seau_1, SIGNAL(triggered()), this, SLOT(openInfo1()));
    connect(ui->actionInformation_r_seau_2, SIGNAL(triggered()), this, SLOT(openInfo2()));
    connect(ui->actionEnregistrer, SIGNAL(triggered()), this, SLOT(save()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newDetect()
{
    //ajout du détecteur dans la liste
    DialogAddDetector * pdialog = new DialogAddDetector(m_detectors, this);
    pdialog->exec();
    if(pdialog->result() == 1) {
        Detector d(pdialog->getX(), pdialog->getY(), pdialog->getDetector().toInt(), pdialog->getNetwork(), this);
        m_detectors.push_back(d);
        paintTableGraphics();
    }
    emit refreshNetwork(m_detectors);
}

void MainWindow::modifNetwork()
{
    DialogModifyNetwork * pmodif = new DialogModifyNetwork(&m_detectors);
    pmodif->exec();
    paintTableGraphics();
    emit refreshNetwork(m_detectors);
}

void MainWindow::openInfo1()
{
    InfoWindow * info1 = new InfoWindow("1", m_detectors, mReceiver);
    info1->setWindowTitle("Informations réseau 1");
    info1->show();
    connect(this, SIGNAL(refreshNetwork(vector<Detector>)), info1, SLOT(refresh(vector<Detector>)));
    connect(mReceiver, SIGNAL(positionFront(qreal,qreal)), info1, SLOT(printFront(qreal,qreal)));
}

void MainWindow::openInfo2()
{
    InfoWindow * info2 = new InfoWindow("2", m_detectors,mReceiver);
    info2->setWindowTitle("Informations réseau 2");
    info2->show();
    connect(this, SIGNAL(refreshNetwork(vector<Detector>)), info2, SLOT(refresh(vector<Detector>)));
    connect(mReceiver, SIGNAL(positionFront(qreal,qreal)), info2, SLOT(printFront(qreal,qreal)));
}

void MainWindow::paintTableGraphics() {

    m_pscene->clear();

    ui->tableWidget->setRowCount(m_detectors.size());
    ui->tableWidget->setSelectionMode(QTableWidget::NoSelection);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    for(int i = 0; i < m_detectors.size(); i++)
    {
        Detector current = m_detectors[i];


        QGraphicsRectItem * prect = new QGraphicsRectItem(current.getCoordsGraphics().first , current.getCoordsGraphics().second, 40, 25);

        if(current.getNetwork() == "1")
        {
            prect->setBrush(QBrush(QColor(21, 155, 210, 255)));
        }
        else
        {
            prect->setBrush(QBrush(QColor(210, 155, 21, 255)));
        }
        QGraphicsTextItem * ptext = new QGraphicsTextItem(QString::fromStdString(to_string(current.getDetectorName())), prect);
        ptext->setPos(current.getCoordsGraphics().first , current.getCoordsGraphics().second);

        m_pscene->addItem(prect);


        //ptext->setPos(coords.first, coords.second);
        // m_pscene->addItem(ptext);


        //ajout du detecteur dans le tableau
        //Colonne 0 : Nom détecteur
        QTableWidgetItem * pitem = new QTableWidgetItem(QString::fromStdString(to_string(current.getDetectorName())));
        if(current.getNetwork() == "1")
        {
            pitem->setBackgroundColor(QColor(21, 155, 210, 255));
        }
        else
        {
            pitem->setBackgroundColor(QColor(210, 155, 21, 255));
        }
        ui->tableWidget->setItem(i, 0, pitem);

        //Colonne 1 : participation
        QTableWidgetItem * pitemParticip = new QTableWidgetItem("test");
        ui->tableWidget->setItem(i, 1, pitemParticip);

        //Colonne 2 : trigger
        QTableWidgetItem * pitemTrigg = new QTableWidgetItem("testTrigg");
        ui->tableWidget->setItem(i, 2, pitemTrigg);

        //Colonne 3 : tension courante
        TensionWidget * ptension = new TensionWidget(&current);
        ui->tableWidget->setCellWidget(i, 3, ptension);

        //Colonne 4 : horloge
        QTableWidgetItem * pitemHorloge = new QTableWidgetItem(QString::fromStdString(to_string(current.getCorrectHorloge())));
        ui->tableWidget->setItem(i, 4, pitemHorloge);

        //Colonne 5 : pm1 / pm2
        ValuedSlider * ppms = new ValuedSlider(Qt::Horizontal, &current);
        ui->tableWidget->setCellWidget(i, 5, ppms);
    }
}

void MainWindow::onHtChanged(MessageMQ *message)
{
    cout << "onHtChanged = " << message->data() << endl;
    std::ostringstream o;
    string str;
    TensionWidget * tw;
    //cout << "--------------------------------------------------------" << endl;
    for(int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        str = "";
        o.str("");
        //cout << "value i = " << i << endl;
        //cout << ui->tableWidget->item(i,0)->text().toStdString() << endl;
        cout << "value test : " << ui->tableWidget->item(i,0)->text().toInt() << " vs " << static_cast<int>(message->name()) << endl;
        if(ui->tableWidget->item(i,0)->text().toInt() == static_cast<int>(message->name()))
        {
            cout << "Matched!!!" << endl;
            tw = qobject_cast<TensionWidget *>(ui->tableWidget->cellWidget(i,3));
            tw->getLabel()->setText(QString::fromStdString(to_string(message->data())));
            tw->getDetector()->setHT(message->data());
            //cout << "set value " << str << " to " << i << endl;
        }
    }
    delete message;
}

void MainWindow::onDerChanged(MessageMQ *message)
{
    std::ostringstream o;
    string str;
    QString detName;
    QString data;
    double derive;
    cout << "--------------------------------------------------------" << endl;
    for(int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        str = "";
        o.str("");
        cout << "value i = " << i << endl;
        cout << ui->tableWidget->item(i,0)->text().toStdString() << endl;
        cout << "value test : " << ui->tableWidget->item(i,0)->text().toInt() << " vs " << static_cast<int>(message->name()) << endl;
        if((detName = ui->tableWidget->item(i,0)->text()).toInt() == static_cast<int>(message->name()))
        {
            cout << "Matched!" << endl;
            derive = message->data()/1000.0;
            data = QString("%1").setNum(derive,'f',3);
            ui->tableWidget->item(i,4)->setText(data);
            cout << "set derive value " << str << " to " << i << endl;
        }
    }
    delete message;
}

void MainWindow::save()
{

}

void MainWindow::load()
{

}
