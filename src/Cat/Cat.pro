#-------------------------------------------------
#
# Project created by QtCreator 2019-06-04T09:49:35
#
#-------------------------------------------------

QT       += core gui network serialport multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Cat
TEMPLATE = app

# The application version
VERSION = 1.0.0

# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../common/release/ -lcommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../common/debug/ -lcommon
else:unix: LIBS += -L$$OUT_PWD/../common/ -lcommon

INCLUDEPATH += $$PWD../common

DEPENDPATH += $$PWD../common

#PRE_TARGETDEPS += -L$$OUT_PWD/../common/release/common.a

#win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../common/release/common.lib
#else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../common/debug/common.lib
#else:unix: PRE_TARGETDEPS += $$OUT_PWD/../common/common.a

SOURCES += main.cpp \
    catwindow.cpp \
    commandcat.cpp \
    socketfilename.cpp \
    filenamedialog.cpp

HEADERS  += \
    catwindow.h \
    commandcat.h \
    socketfilename.h \
    filenamedialog.h

FORMS    +=
