#-------------------------------------------------
#
# Project created by QtCreator 2016-12-01T14:14:05
#
#-------------------------------------------------

QT       += core gui network serialport multimedia svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = common
TEMPLATE = lib

CONFIG += staticlib

# The application version
VERSION = 1.0.1

# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

HEADERS += \
    data.h \
    filter.h \
    filterdialog.h \
    qcustomplot.h \
    qtincludes.h \
    signalaudio.h \
    socketabstract.h \
    connectiondialog.h \
    led.h

SOURCES += \
    data.cpp \
    filter.cpp \
    filterdialog.cpp \
    qcustomplot.cpp \
    signalaudio.cpp \
    socketabstract.cpp \
    connectiondialog.cpp

FORMS +=
