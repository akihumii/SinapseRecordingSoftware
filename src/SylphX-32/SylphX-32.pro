#-------------------------------------------------
#
# Project created by QtCreator 2018-10-04T14:54:33
#
#-------------------------------------------------

QT       += core gui network serialport multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = SylphX-32
TEMPLATE = app

# The application version
VERSION = 2.0.0

DEFINES += QT_DEPRECATED_WARNINGS

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../common/release/ -lcommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../common/debug/ -lcommon
else:unix: LIBS += -L$$OUT_PWD/../common/ -lcommon

SOURCES += main.cpp\
        mainwindow.cpp \
    serialchannel.cpp

HEADERS  += mainwindow.h \
    ringbuffer.h \
    serialchannel.h
