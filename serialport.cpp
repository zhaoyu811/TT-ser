#include "serialport.h"
#include <QList>
#include <QSerialPortInfo>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>

QVector<double>x_1,y_1;
QVector<double>x_2,y_2;

serialport::serialport(QWidget *parent) : QMainWindow(parent)
{
    /**************************************************************
     * 添加参数窗口
     * 1.新建QGridLayout，在QGridLayout上添加QComboBox，QLabel等
     * 2.新建QWidget，把QGridLayout放入QWidget
     * 3.新建QDockWidget，通过setWidget函数把QWidget放入QDockWidget
     *************************************************************/
    gridLayout[0] = new QGridLayout;
    //在参数窗口添加串口的一些属性
    serial_attr_label[0] = new QLabel;
    serial_attr_label[0]->setText(tr("端口号"));
    gridLayout[0]->addWidget(serial_attr_label[0],0,0,1,1);
    serial_attr_combox[0] = new QComboBox;
    gridLayout[0]->addWidget(serial_attr_combox[0],0,1,1,1);

    serial_attr_label[1] = new QLabel;
    serial_attr_label[1]->setText(tr("波特率"));
    gridLayout[0]->addWidget(serial_attr_label[1],1,0,1,1);
    serial_attr_combox[1] = new QComboBox;
    gridLayout[0]->addWidget(serial_attr_combox[1],1,1,1,1);
    serial_attr_combox[1]->addItem(tr("4800"));
    serial_attr_combox[1]->addItem(tr("9600"));
    serial_attr_combox[1]->addItem(tr("115200"));
    serial_attr_combox[1]->setCurrentIndex(1);

    serial_attr_label[2] = new QLabel;
    serial_attr_label[2]->setText(tr("数据位"));
    gridLayout[0]->addWidget(serial_attr_label[2],2,0,1,1);
    serial_attr_combox[2] = new QComboBox;
    gridLayout[0]->addWidget(serial_attr_combox[2],2,1,1,1);
    serial_attr_combox[2]->addItem(tr("5"));
    serial_attr_combox[2]->addItem(tr("6"));
    serial_attr_combox[2]->addItem(tr("7"));
    serial_attr_combox[2]->addItem(tr("8"));
    serial_attr_combox[2]->setCurrentIndex(3);

    serial_attr_label[3] = new QLabel;
    serial_attr_label[3]->setText(tr("停止位"));
    gridLayout[0]->addWidget(serial_attr_label[3],3,0,1,1);
    serial_attr_combox[3] = new QComboBox;
    gridLayout[0]->addWidget(serial_attr_combox[3],3,1,1,1);
    serial_attr_combox[3]->addItem(tr("1"));
    serial_attr_combox[3]->addItem(tr("1.5"));
    serial_attr_combox[3]->addItem(tr("2"));
    serial_attr_combox[3]->setCurrentIndex(0);

    serial_attr_label[4] = new QLabel;
    serial_attr_label[4]->setText(tr("校验位"));
    gridLayout[0]->addWidget(serial_attr_label[4],4,0,1,1);
    serial_attr_combox[4] = new QComboBox;
    gridLayout[0]->addWidget(serial_attr_combox[4],4,1,1,1);
    serial_attr_combox[4]->addItem(tr("none"));
    serial_attr_combox[4]->addItem(tr("odd"));
    serial_attr_combox[4]->addItem(tr("even"));
    serial_attr_combox[4]->setCurrentIndex(0);

    open_serial_button = new QPushButton();
    open_serial_button->setText("打开串口");
    gridLayout[0]->addWidget(open_serial_button,5,1,1,1);
    connect(open_serial_button,&QPushButton::clicked,this,\
            &serialport::open_serial_button_clicked);

    update_serial_button = new QPushButton();
    update_serial_button->setText("更新串口");
    gridLayout[0]->addWidget(update_serial_button,5,0,1,1);
    connect(update_serial_button,&QPushButton::clicked,this,\
            &serialport::update_serial_button_clicked);

    widget[0] = new QWidget;
    widget[0]->setLayout(gridLayout[0]);

    dock[0] = new QDockWidget(tr("参数"), this);
    dock[0]->setWidget(widget[0]);
    dock[0]->setFeatures(QDockWidget::DockWidgetMovable);
    dock[0]->setAllowedAreas(Qt::LeftDockWidgetArea);

    addDockWidget(Qt::LeftDockWidgetArea, dock[0]);

    /*****************************添加折线图**************************/

    widget[1] = new QWidget;
    gridLayout[1] = new QGridLayout(widget[1]);
    plot = new QwtPlot(widget[1]);
    gridLayout[1]->addWidget(plot,0,0,1,1);
    widget[1]->setLayout(gridLayout[1]);
    setCentralWidget(widget[1]);

    plot->setTitle(tr("温度"));
    plot->setAxisScale( QwtPlot::yLeft, 0.0, 200 );
    plot->setAxisScale( QwtPlot::xBottom, 0.0, 5 );

    grid[0] = new QwtPlotGrid();
    grid[1] = new QwtPlotGrid();
    curve[0] = new QwtPlotCurve();
    curve[1] = new QwtPlotCurve();

    QwtSymbol *symbol = new QwtSymbol( QwtSymbol::Ellipse,
        QBrush( Qt::yellow ), QPen( Qt::red, 2 ), QSize( 2, 2 ) );
    curve[0]->setSymbol( symbol );
/*
    QVector<double>x_1,y_1;
    for(int i = 0;i<1000;i++){
        x_1.push_back(i);
        double tt =x_1.at(i)/2.0;
        y_1.push_back(tt);
    }
    curve[0]->setSamples( x_1,y_1 );
    curve[0]->attach( plot );
*/
    /**************************数据库窗口***********************************/
    save_as_button = new QPushButton;
    save_as_button->setText("另存为");
    show_all_button = new QPushButton;
    show_all_button->setText("显示全部");
    connect(show_all_button,&QPushButton::clicked,this,\
            &serialport::show_all_button_clicked);
    connect(save_as_button,&QPushButton::clicked,this,\
            &serialport::save_as_button_clicked);
    gridLayout[2] = new QGridLayout;
    gridLayout[2]->addWidget(save_as_button,2,1,1,1);
    gridLayout[2]->addWidget(show_all_button,2,2,1,1);
    widget[2] = new QWidget;
    widget[2]->setLayout(gridLayout[2]);

    dock[1] = new QDockWidget(tr("数据库"),this);
    dock[1]->setWidget(widget[2]);
    dock[1]->setFeatures(QDockWidget::DockWidgetMovable);
    dock[1]->setAllowedAreas(Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, dock[1]);

    //建立数据库
    mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("mydb.db");
    if (mydb.open())
        qDebug("mydb.db 创建成功");
    else
        qDebug("mydb.db 创建失败");
    query = new QSqlQuery(mydb);
    query->exec("create table temperature (id int primary key, position_1 double, position_2 double, time varchar)");
    //建立tableview
    tableview = new QTableView;
    model = new QStandardItemModel();
    //model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("id")));
    model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("position_1")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("position_2")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("time")));
    tableview->setModel(model);
    gridLayout[2]->addWidget(tableview,1,1,1,2);

    query->exec("select * from temperature");
    while (query->next())
    {
        model->setItem(i,0,new QStandardItem(query->value(1).toString()));
        model->setItem(i,1,new QStandardItem(query->value(2).toString()));
        model->setItem(i,2,new QStandardItem(query->value(3).toString()));
        i++;
    }
    /**************************mqtt**************************/
    QHostAddress host = QHostAddress("192.168.1.132");
    mqttclient = new QMQTT::Client();
    mqttclient->setHost(host);
    mqttclient->setPort(1883);
    mqttclient->username() = "book";
    mqttclient->password() = "123";
    mqttclient->setClientId("client_id_1");
    connect(mqttclient,&QMQTT::Client::connected,this,&serialport::mqtt_connected);
    mqttclient->connectToHost();
}

void serialport::open_serial_button_clicked()
{
    qDebug("open_serial_button_clicked");
    if (flag == 0)
    {
        flag = 1;
        open_serial_button->setText("关闭");
        serial = new QSerialPort;
        //设置串口号
        serial->setPortName(serial_attr_combox[0]->currentText());
        //设置串口波特率
        serial->setBaudRate(serial_attr_combox[1]->currentText().toInt());
        //设置串口数据位
        if (serial_attr_combox[2]->currentText() == "5")
            serial->setDataBits(QSerialPort::Data5);
        else if (serial_attr_combox[2]->currentText() == "6")
            serial->setDataBits(QSerialPort::Data6);
        else if (serial_attr_combox[2]->currentText() == "7")
            serial->setDataBits(QSerialPort::Data7);
        else if (serial_attr_combox[2]->currentText() == "8")
            serial->setDataBits(QSerialPort::Data8);
        else
            serial->setDataBits(QSerialPort::UnknownDataBits);
        //设置串口校验位
        if (serial_attr_combox[4]->currentText() == "none")
            serial->setParity(QSerialPort::NoParity);
        else if (serial_attr_combox[4]->currentText() == "odd")
            serial->setParity(QSerialPort::OddParity);
        else if (serial_attr_combox[4]->currentText() == "even")
            serial->setParity(QSerialPort::EvenParity);
        else
            serial->setParity(QSerialPort::UnknownParity);
        //设置串口停止位
        if (serial_attr_combox[3]->currentText() == "1")
            serial->setStopBits(QSerialPort::OneStop);
        else if (serial_attr_combox[3]->currentText() == "1.5")
            serial->setStopBits(QSerialPort::OneAndHalfStop);
        else if (serial_attr_combox[3]->currentText() == "2")
            serial->setStopBits(QSerialPort::TwoStop);
        else
            serial->setStopBits(QSerialPort::UnknownStopBits);
        //建立信号槽，当有数据读时，调用readMyCom函数
        if (serial->open(QSerialPort::ReadWrite))
            //qDebug("打开成功");
            QMessageBox::information(this,tr("打开成功"),"打开成功");
        else
            //qDebug("打开失败");
            QMessageBox::critical(this,tr("打开失败"),"打开失败");
        connect(serial,&QSerialPort::readyRead,this,&serialport::readMyCom);
        //将各个选项关闭，不让设置发生变化
        serial_attr_combox[0]->setEnabled(false);
        serial_attr_combox[1]->setEnabled(false);
        serial_attr_combox[2]->setEnabled(false);
        serial_attr_combox[3]->setEnabled(false);
        serial_attr_combox[4]->setEnabled(false);
        open_serial_button->setEnabled(true);
    }
    else if(flag == 1)
    {
        flag = 0;
        //关闭串口
        serial->close();
        //打开各个选项，这时可以改变设置
        open_serial_button->setText("打开");
        serial_attr_combox[0]->setEnabled(true);
        serial_attr_combox[1]->setEnabled(true);
        serial_attr_combox[2]->setEnabled(true);
        serial_attr_combox[3]->setEnabled(true);
        serial_attr_combox[4]->setEnabled(true);
        open_serial_button->setEnabled(true);
    }
}

void serialport::update_serial_button_clicked()
{
    qDebug("update_serial_button_clicked");
    serial_attr_combox[0]->clear();
    QList<QSerialPortInfo> list = QSerialPortInfo::availablePorts();
    foreach(const QSerialPortInfo &serialportinfo, list)
    {
        serial_attr_combox[0]->addItem(serialportinfo.portName());
    }
}

//当串口有数据可读时，此函数被调用
double temperature1 = 0, temperature2 = 0;
void serialport::readMyCom()
{
    static QByteArray temp;
    unsigned short temp1 = 0, temp2 = 0;

    temp.append(serial->readAll());
    temp = temp.mid(temp.indexOf("{{"));

    while (temp.contains("{{") && temp.contains("}}"))
    {
        //数据处理
        temp1 = (temp1|temp[3]) << 8;
        temp1 = temp1|(0x00ff&temp[4]);
        temperature1 = 3.3*temp1/4096*2*150/5;
        temp2 = (temp2|temp[6]) << 8;
        temp2 = temp2|(0x00ff&temp[7]);
        temperature2 = 3.3*temp2/4096*2*150/5;
        temp.clear();
        QDateTime time = QDateTime::currentDateTime();
        QString datatime = time.toString("yyyy-MM-dd HH:mm:ss");
        //数据保存到数据库
        query->exec(QString("insert into temperature values(%1,%2,%3,'%4')").arg(i).arg(temperature1).arg(temperature2).arg(datatime));
        //数据加载到表格里
        model->setItem(i,0,new QStandardItem(QString("%1").arg(temperature1)));
        model->setItem(i,1,new QStandardItem(QString("%1").arg(temperature2)));
        model->setItem(i,2,new QStandardItem(datatime));
        tableview->updatesEnabled();
        //数据实时曲线
        static QPolygonF points;
        points << QPointF( i, temperature1 );
        curve[0]->setSamples( points );
        curve[0]->attach( plot );
        //更新数据数量
        i++;
    }
}


void serialport::save_as_button_clicked()
{
    qDebug("save_as_button_clicked");
    QFileDialog::Options options;
    options |= QFileDialog::DontUseNativeDialog;

    QString selectedFilter;
    QString filename = QFileDialog::getSaveFileName(this,\
                                 tr("QFileDialog::getSaveFilename()"),\
                                 "text.txt",\
                                 tr("ALL Files(*);; Text Files(*.txt);; PDF Files(*.pdf);;xlsx Files(*.xlsx)"),\
                                 &selectedFilter, options);
    QFileInfo file;
    file.setFile(filename);
    if (file.suffix() == "txt")
    {
        QFile data(filename);
        if (data.open(QFile::WriteOnly | QFile::Truncate))
        {
            QTextStream out(&data);
            query->exec("select * from distance");
            while (query->next())
            {
                out << QString("时间：%1   位置点1温度：%2  位置点2温度：%3").arg(query->value(3).toString())\
                                .arg(query->value(1).toString()).arg(query->value(2).toString()) << endl;
            }
        }
    }
    else if (file.suffix() == "pdf")
    {
        QPrinter printer;
        printer.setOutputFormat(QPrinter::PdfFormat);//设置输出格式为pdf
        printer.setPageSize(QPrinter::A4);//设置纸张大小为A4
        printer.setOutputFileName(filename);//设置输出路径

        QPainter painter;
        painter.begin(&printer);
        query->exec("select * from distance");
        int i = 0;
        while (query->next())
        {
           // painter.drawText(20,20*i,QString("时间：%1   位置点1温度：%2  位置点2温度：%3").arg(query->value(3).toString()).arg(query->value(1).toString()).arg(query->value(2).toString()) << endl);
            if ((i*20) > 1000)
            {
                i = 0;
                printer.newPage();
            }
            i++;
        }
        painter.end();
    }
   /* else if (file.suffix() == "xlsx")
    {
        QXlsx::Document xlsx;
        query->exec("select * from distance");
        int i = 2;
        xlsx.write(1,1,"time");
        xlsx.write(1,2,"distance");
        while (query->next())
        {
            //xlsx.write(i,1, query->value(0));
            xlsx.write(i,2, query->value(1));
            xlsx.write(i,1, query->value(2));
            i++;
        }
        QXlsx::Chart *lineChart = xlsx.insertChart(0, 3, QSize(1000, 300));
        lineChart->setChartType(QXlsx::Chart::CT_Line);
        lineChart->addSeries(QXlsx::CellRange(QString("B2:B%1").arg(i)));

        xlsx.saveAs(filename);
    }*/
}

void serialport::show_all_button_clicked()
{
    qDebug("show_all_button_clicked");
    QVector<double> x(i), y(i);
    int j = 0;

    query->exec("select * from temperature");

    while (query->next())
    {
        x[j] = j+1;
        y[j] = query->value(1).toDouble();
        j++;
    }
}

void serialport::mqtt_connected()
{
    qDebug() << "mqtt_connected";
    connect(mqttclient,&QMQTT::Client::subscribed,this,&serialport::mqtt_subscribed);
    mqttclient->subscribe(QString(tr("cmd")), 2);
}

void serialport::mqtt_subscribed()
{
    qDebug() << "mqtt_subscribed";
    connect(mqttclient,&QMQTT::Client::received,this,&serialport::mqtt_received);
}

void serialport::mqtt_received(const QMQTT::Message& message)
{
    qDebug() << "mqtt_received";
    qDebug() << "publish received: \"" << QString::fromUtf8(message.payload());
    if ((QString::fromUtf8(message.payload())) == QString(tr("off")))
    {
        serial->write("off");
    }
    else if ((QString::fromUtf8(message.payload())) == QString(tr("on")))
    {
        serial->write("on");
    }
    else if ((QString::fromUtf8(message.payload())) == QString(tr("temperature")))
    {
        QMQTT::Message message1;
        message1.setTopic(QString(tr("temperature")));
        message1.setQos(2);
        temperature1 = 1111.1111;
        temperature2 = 2222.2222;
        message1.setPayload((QString("temperature1:%1,temperature2:%2").arg(temperature1).arg(temperature2)).toLatin1());
        mqttclient->publish(message1);
    }
}
