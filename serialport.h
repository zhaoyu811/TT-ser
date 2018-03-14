#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QGridLayout>
#include <QDockWidget>
#include <QLabel>
#include <QSerialPort>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTableView>
#include <QStandardItemModel>
#include <QTimer>
#include <QTextLine>
#include <QDateTime>
#include <QPrinter>

#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_plot_grid.h>
#include <qwt/qwt_symbol.h>
#include <qmqtt/qmqtt.h>

class serialport : public QMainWindow
{
    Q_OBJECT
public:
    explicit serialport(QWidget *parent = 0);
    QComboBox *serial_attr_combox[5];

private:

    QLabel *serial_attr_label[5];
    QPushButton *open_serial_button;
    QPushButton *update_serial_button;

    QWidget *widget[3];
    QGridLayout *gridLayout[3];

    QDockWidget *dock[2];
    QTextLine *textline;

    int flag = 0;
    QSerialPort *serial;

    QPushButton *save_as_button;
    QSqlDatabase mydb;
    QSqlQuery *query;
    QTableView *tableview;
    QStandardItemModel *model;
    int i = 0;

    QPushButton *show_all_button;
    QMQTT::Client *mqttclient;

    QwtPlot *plot;
    QwtPlotGrid *grid[2];
    QwtPlotCurve *curve[2];

signals:

public slots:
    void open_serial_button_clicked();
    void update_serial_button_clicked();
    void readMyCom();
    void save_as_button_clicked();
    void show_all_button_clicked();
    void mqtt_connected();
    void mqtt_subscribed();
    void mqtt_received(const QMQTT::Message &message);
};

#endif // SERIALPORT_H
