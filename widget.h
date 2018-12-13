#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtCore>
#include <QtDebug>
#include <QtGui/QtGui>
#include <QtNetwork>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
    QProcess *process;
    QString prog;
    void loadIP();
    QString ip;
    int timerid;
    bool connected;
    int counter;
    QHash<int, QString> ips;

protected:
    void closeEvent(QCloseEvent *event);
    void timerEvent(QTimerEvent *event);

signals:
    void newIP(QString &newIP);

private slots:
    void onNewIP(QString &newIP);

public slots:
    void onProcessStarted();
private slots:
    void on_pushButton_clicked();
};

#endif // WIDGET_H
