#-------------------------------------------------
#
# Project created by QtCreator 2016-12-01T14:14:05
#
#-------------------------------------------------

QT       += core gui network serialport multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Jane
TEMPLATE = app

# The application version
VERSION = 1.0.8

# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../common/release/ -lcommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../common/debug/ -lcommon
else:unix: LIBS += -L$$OUT_PWD/../common/ -lcommon

INCLUDEPATH += $$PWD../common

DEPENDPATH += $$PWD../common

HEADERS += \
    channel.h \
    command.h \
#    commanddialog.h \
    dataprocessor.h \
    serialchannel.h \
#    serialportdialog.h \
    signalchannel.h \
    qtincludes.h \
    socketneutrino.h \
#    measurementdialog.h\
    import/cmddialog.h\
    import/amwflash_mainwindow.h\
    import/console.h\
    import/settingdialog.h\
    import/stimulator.h\
    import/commandj.h \
    graphwindow.h

SOURCES += \
    channel.cpp \
    command.cpp \
#    commanddialog.cpp \
    dataprocessor.cpp \
    main.cpp \
    serialchannel.cpp \
#    serialportdialog.cpp \
    signalchannel.cpp \
    socketneutrino.cpp \
#    measurementdialog.cpp\
    import/cmddialog.cpp\
    import/amwflash_mainwindow.cpp\
    import/console.cpp\
    import/settingdialog.cpp\
    import/stimulator.cpp\
    import/commandj.cpp \
    graphwindow.cpp


FORMS +=

DISTFILES += \
