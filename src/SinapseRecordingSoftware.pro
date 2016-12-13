#-------------------------------------------------
#
# Project created by QtCreator 2016-12-01T14:14:05
#
#-------------------------------------------------

QT       += core gui network serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = SinapseRecordingSoftware
TEMPLATE = app

# The application version
VERSION = 0.1.1

# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

SOURCES += main.cpp\
        mainwindow_ma.cpp \
    data.cpp \
    filenamedialog.cpp \
    qcustomplot.cpp \
    serialchannel.cpp \
    serialportdialog.cpp \
    signalchannel.cpp \
    mainwindow_ka.cpp \
    channel.cpp \
    command.cpp \
    commanddialog.cpp \
    connectiondialog.cpp \
    socketedison.cpp \
    dataprocessor_ma.cpp \
    dataprocessor_ka.cpp \
    serialneutrino.cpp \
    filter.cpp \
    filterdialog.cpp

HEADERS  += mainwindow_ma.h \
    data.h \
    filenamedialog.h \
    qcustomplot.h \
    qtincludes.h \
    serialchannel.h \
    serialportdialog.h \
    signalchannel.h \
    mainwindow_ka.h \
    channel.h \
    command.h \
    commanddialog.h \
    connectiondialog.h \
    socketedison.h \
    dataprocessor_ma.h \
    dataprocessor_ka.h \
    serialneutrino.h \
    filter.h \
    filterdialog.h

FORMS +=

DISTFILES +=
