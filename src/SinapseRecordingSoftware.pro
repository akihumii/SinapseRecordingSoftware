#-------------------------------------------------
#
# Project created by QtCreator 2016-12-01T14:14:05
#
#-------------------------------------------------

QT       += core gui network serialport multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = SINAPSE
TEMPLATE = app

# The application version
VERSION = 1.0

# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

DEFINES += NEUTRINO_II
# DEFINES += SYLPH

SOURCES += main.cpp\
    data.cpp \
    filenamedialog.cpp \
    qcustomplot.cpp \
    serialchannel.cpp \
    serialportdialog.cpp \
    signalchannel.cpp \
    channel.cpp \
    command.cpp \
    commanddialog.cpp \
    connectiondialog.cpp \
    socketedison.cpp \
    filter.cpp \
    filterdialog.cpp \
    signalaudio.cpp \
    mainwindow.cpp \
    dataprocessor.cpp

HEADERS  += \
    data.h \
    filenamedialog.h \
    qcustomplot.h \
    qtincludes.h \
    serialchannel.h \
    serialportdialog.h \
    signalchannel.h \
    channel.h \
    command.h \
    commanddialog.h \
    connectiondialog.h \
    socketedison.h \
    filter.h \
    filterdialog.h \
    signalaudio.h \
    mainwindow.h \
    dataprocessor.h

FORMS +=

DISTFILES +=
