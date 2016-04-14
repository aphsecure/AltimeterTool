#-------------------------------------------------
#
# Project created by QtCreator 2016-04-05T16:14:35
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AltimeterTool
TEMPLATE = app
RC_FILE = icon.rc
CONFIG += c++11

SOURCES += main.cpp\
        altimetertoolmainwindow.cpp \
    altimeterhandler.cpp \
    murutils.cpp \
    settingsdialog.cpp \
    informationwidget.cpp

HEADERS  += altimetertoolmainwindow.h \
    murmagics.h \
    altimeterhandler.h \
    murutils.h \
    settingsdialog.h \
    informationwidget.h

RESOURCES += \
    icons.qrc \
    style.qrc

FORMS += \
    settingsdialog.ui \
    informationwidget.ui
