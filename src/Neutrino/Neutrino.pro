#-------------------------------------------------
#
# Project created by QtCreator 2016-12-01T14:14:05
#
#-------------------------------------------------

QT       += core gui network serialport multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Neutrino
TEMPLATE = app

# The application version
VERSION = 1.0.1

# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

HEADERS += \
    channel.h \
    command.h \
    commanddialog.h \
    connectiondialog.h \
    dataprocessor.h \
    mainwindow.h \
    serialchannel.h \
    serialportdialog.h \
    signalaudio.h \
    signalchannel.h \
    socketedison.h

SOURCES += \
    channel.cpp \
    command.cpp \
    commanddialog.cpp \
    connectiondialog.cpp \
    dataprocessor.cpp \
    main.cpp \
    mainwindow.cpp \
    serialchannel.cpp \
    serialportdialog.cpp \
    signalaudio.cpp \
    signalchannel.cpp \
    socketedison.cpp
