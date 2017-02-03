#-------------------------------------------------
#
# Project created by QtCreator 2016-12-01T14:14:05
#
#-------------------------------------------------

QT       += core gui network serialport multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = SIRE
TEMPLATE = subdirs

SUBDIRS +=   \
    src/common \
    src/Neutrino \
    src/Sylph \

common.subdir = src/common
Neutrino.subdir = src/Neutrino
Sylph.subdir = src/Sylph

Sylph.depends = common
Neutrino.depends = common

# The application version
VERSION = 1.0.1

# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

FORMS +=

DISTFILES +=
