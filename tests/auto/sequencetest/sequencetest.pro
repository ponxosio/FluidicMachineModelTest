#-------------------------------------------------
#
# Project created by QtCreator 2017-01-31T09:55:58
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_sequencetesttest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_sequencetesttest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

debug {
    INCLUDEPATH += X:\fluidicMachineModel\dll_debug\include
    LIBS += -L$$quote(X:\fluidicMachineModel\dll_debug\bin) -lFluidicMachineModel

    INCLUDEPATH += X:\utils\dll_debug\include
    LIBS += -L$$quote(X:\utils\dll_debug\bin) -lutils
}

!debug {
    INCLUDEPATH += X:\fluidicMachineModel\dll_release\include
    LIBS += -L$$quote(X:\fluidicMachineModel\dll_release\bin) -lFluidicMachineModel

    INCLUDEPATH += X:\utils\dll_release\include
    LIBS += -L$$quote(X:\utils\dll_release\bin) -lutils
}

INCLUDEPATH += X:\fluidicMachineModel\includes
