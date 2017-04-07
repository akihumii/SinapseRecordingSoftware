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

Neutrino.depends = common
Sylph.depends = common

FORMS +=

DISTFILES +=
