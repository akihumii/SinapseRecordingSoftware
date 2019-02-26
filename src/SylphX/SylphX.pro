#-------------------------------------------------
#
# Project created by QtCreator 2016-12-01T14:14:05
#
#-------------------------------------------------

QT       += core gui network serialport multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = SylphX
TEMPLATE = app

# The application version
VERSION = 1.4.0

# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../common/release/ -lcommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../common/debug/ -lcommon
else:unix: LIBS += -L$$OUT_PWD/../common/ -lcommon

INCLUDEPATH += $$PWD../common

DEPENDPATH += $$PWD../common

HEADERS += \
    dataprocessor.h \
    mainwindow.h \
    serialchannel.h \
    serialportdialog.h \
    socketsylph.h \
    ../Odin/serialodin.h \
    ../Odin/commandodin.h \
    ../Odin/socketodin.h \
    ../Odin/loopingthread.h \
    ../Odin/pulseplot.h \
    ../Odin/odinwindow.h \
    dynomometer.h


SOURCES += \
    dataprocessor.cpp \
    main.cpp \
    mainwindow.cpp \
    serialchannel.cpp \
    serialportdialog.cpp \
    socketsylph.cpp \
    ../Odin/serialodin.cpp \
    ../Odin/commandodin.cpp \
    ../Odin/socketodin.cpp \
    ../Odin/pulseplot.cpp \
    ../Odin/odinwindow.cpp \
    dynomometer.cpp
