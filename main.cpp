#include <serialport.h>
#include <QApplication>
#include <QSerialPortInfo>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    serialport Serialport;

    Serialport.show();
    Serialport.serial_attr_combox[0]->clear();
    QList<QSerialPortInfo> list = QSerialPortInfo::availablePorts();
    foreach(const QSerialPortInfo &serialportinfo, list)
    {
        Serialport.serial_attr_combox[0]->addItem(serialportinfo.portName());
    }
    Serialport.show();

    return a.exec();
}
