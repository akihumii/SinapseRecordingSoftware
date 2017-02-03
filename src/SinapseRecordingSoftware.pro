#-------------------------------------------------
#
# Project created by QtCreator 2016-12-01T14:14:05
#
#-------------------------------------------------

QT       += core gui network serialport multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = SIRE
TEMPLATE = subdirs

SUBDIRS = DataAnalyzer/DataAnalyzer.pro \
    Sylph

# The application version
VERSION = 1.0.1

# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

DEFINES += NEUTRINO_II
# DEFINES += SYLPH

SOURCES += main.cpp\
    data.cpp \
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
    dataprocessor.cpp \
    dataanalyzer.cpp \
    common/data.cpp \
    common/filter.cpp \
    common/filterdialog.cpp \
    common/qcustomplot.cpp \
    Neutrino/channel.cpp \
    Neutrino/command.cpp \
    Neutrino/commanddialog.cpp \
    Neutrino/connectiondialog.cpp \
    Neutrino/dataprocessor.cpp \
    Neutrino/main.cpp \
    Neutrino/mainwindow.cpp \
    Neutrino/serialchannel.cpp \
    Neutrino/serialportdialog.cpp \
    Neutrino/signalaudio.cpp \
    Neutrino/signalchannel.cpp \
    Neutrino/socketedison.cpp \
    Sylph/dataprocessor.cpp \
    Sylph/main.cpp \
    Sylph/mainwindow.cpp \
    Sylph/serialchannel.cpp \
    Sylph/serialportdialog.cpp \
    Sylph/signalaudio.cpp


HEADERS  += \
    data.h \
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
    dataprocessor.h \
    dataanalyzer.h \
    common/data.h \
    common/filter.h \
    common/filterdialog.h \
    common/qcustomplot.h \
    common/qtincludes.h \
    Neutrino/channel.h \
    Neutrino/command.h \
    Neutrino/commanddialog.h \
    Neutrino/connectiondialog.h \
    Neutrino/dataprocessor.h \
    Neutrino/mainwindow.h \
    Neutrino/serialchannel.h \
    Neutrino/serialportdialog.h \
    Neutrino/signalaudio.h \
    Neutrino/signalchannel.h \
    Neutrino/socketedison.h \
    Sylph/dataprocessor.h \
    Sylph/mainwindow.h \
    Sylph/serialchannel.h \
    Sylph/serialportdialog.h \
    Sylph/signalaudio.h

FORMS +=

DISTFILES +=
