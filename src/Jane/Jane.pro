#-------------------------------------------------
#
# Project created by QtCreator 2017-06-07T10:49:43
#
#-------------------------------------------------

QT       += core gui network serialport multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Mjolnir
TEMPLATE = app

# The application version
VERSION = 0.0.1

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VERSION\\\" QT_DEPRECATED_WARNINGS

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../common/release/ -lcommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../common/debug/ -lcommon
else:unix: LIBS += -L$$OUT_PWD/../common/ -lcommon

INCLUDEPATH += $$PWD../common

DEPENDPATH += $$PWD../common

SOURCES += main.cpp\
        mainwindow.cpp \
    amwflash_mainwindow.cpp \
    console.cpp \
    settingdialog.cpp \
    command.cpp \
    stimulator.cpp\
    socketthor.cpp\
    dataprocessor.cpp\
    channel.cpp\
    serialchannel.cpp \
    measurementdialog.cpp

HEADERS  += mainwindow.h \
    amwflash_mainwindow.h \
    console.h \
    settingdialog.h \
    command.h \
    stimulator.h\
    socketthor.h\
    dataprocessor.h\
    channel.h\
    serialchannel.h \
    measurementdialog.h

