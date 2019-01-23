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
    src/SylphII \
    src/SylphX \
    src/Odin \
    src/Jane \
#    src/AMWFlash \
#    src/NeuroModulation

common.subdir = src/common
Neutrino.subdir = src/Neutrino
SylphII.subdir = src/SylphII
SylphX.subdir = src/SylphX
Odin.subdir = src/Odin
Jane.subdir = src/Jane

Neutrino.depends = common
SylphII.depends = common
SylphX.depends = common
Odin.depends = common
Jane.depends = common

FORMS +=

DISTFILES +=

