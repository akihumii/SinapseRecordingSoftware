#-------------------------------------------------
#
# Project created by QtCreator 2017-04-19T15:06:15
#
#-------------------------------------------------

QT       += core gui network serialport multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Odin
TEMPLATE = app

# The application version
VERSION = 1.0.4

# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../common/release/ -lcommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../common/debug/ -lcommon
else:unix: LIBS += -L$$OUT_PWD/../common/ -lcommon

INCLUDEPATH += $$PWD../common

DEPENDPATH += $$PWD../common

SOURCES += main.cpp\
        mainwindow.cpp \
    serialodin.cpp \
    commandodin.cpp \
    socketodin.cpp \
    pulseplot.cpp

HEADERS  += mainwindow.h \
    serialodin.h \
    commandodin.h \
    socketodin.h \
    loopingthread.h \
    pulseplot.h

FORMS    +=
