#-------------------------------------------------
#
# Project created by QtCreator 2017-03-14T13:35:10
#
#-------------------------------------------------

QT       += core gui serialport printsupport sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TT-ser
TEMPLATE = app


SOURCES += main.cpp\
    serialport.cpp

HEADERS  += \
    serialport.h

LIBS += -LG:/Qt/Qt5.9.1/5.9.1/mingw53_32/bin -lqmqtt
LIBS += -LG:/Qt/Qt5.9.1/5.9.1/mingw53_32/bin -lqmqttd
LIBS += -LG:/Qt/Qt5.9.1/5.9.1/mingw53_32/bin -lqwt
LIBS += -LG:/Qt/Qt5.9.1/5.9.1/mingw53_32/bin -lqwtd
