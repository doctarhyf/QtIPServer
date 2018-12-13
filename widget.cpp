#include "widget.h"
#include "ui_widget.h"
#define LAST_IP 0
#define NEW_IP 1
#define TIMER_DELAY 1000

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    connected = false;
    counter = 0;

    ui->labelOnline->setVisible(!connected);
    ui->labelOffline->setVisible(connected);

    prog = "cmd";

    process = new QProcess(this);


    connect(process, SIGNAL(started()), this, SLOT(onProcessStarted()));
    connect(this, SIGNAL(newIP(QString&)), this, SLOT(onNewIP(QString&)));
    timerid = startTimer(TIMER_DELAY);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::loadIP()
{
    if(counter > 1) counter = 0;
    ip = "";

    QNetworkConfigurationManager mgr;
    QList<QNetworkConfiguration> activeConfigs = mgr.allConfigurations(QNetworkConfiguration::Active);


    for(int i = 0; i < activeConfigs.size(); i++){
        QNetworkSession session(activeConfigs.at(i), this);

        if( session.interface().flags() & QNetworkInterface::IsUp){

            QNetworkInterface intf = session.interface();

            if(intf.humanReadableName() == "Wi-Fi"){
                //qDebug() << intf.index();

                QList<QNetworkAddressEntry> entrz = intf.addressEntries();

                for(QNetworkAddressEntry adde : entrz){

                   if( adde.ip().protocol() == QAbstractSocket::IPv4Protocol){
                       ip = adde.ip().toString();


                   }


                }

            }


        }

    }


    ips.insert(counter, ip);

    QString ip1 = ips.value(LAST_IP);
    QString ip2 = ips.value(NEW_IP);


    qDebug() << "ip1 : " << ip1;
    qDebug() << "ip2 : " << ip2;

    qDebug() << "Checking";

    if(ip1 != ip2){


        connected = ip == "" ? false : true;

        QString connState = connected ? "online" : "offline";
        qDebug() << "Conn State Changed : " << connState;
        emit newIP(ip);


    }



    counter ++;

}

void Widget::closeEvent(QCloseEvent *event)
{
    if(process->state() == QProcess::Running){
        process->close();
    }
    event->accept();
}

void Widget::timerEvent(QTimerEvent *event)
{
    loadIP();
    //qDebug() << "timerEvent : " << ip;
    ui->labelOnline->setVisible(connected);
    ui->labelOffline->setVisible(!connected);
}

void Widget::onNewIP(QString &newIP)
{
    ip = newIP;
    ui->label->setText(newIP);
}

void Widget::onProcessStarted()
{
    //qDebug() << "Process started!";
}

void Widget::on_pushButton_clicked()
{
    QStringList args;
    args << "ipconfig";
    process->start(prog, args);

    loadIP();
}
