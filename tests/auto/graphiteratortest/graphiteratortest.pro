#-------------------------------------------------
#
# Project created by QtCreator 2017-02-19T18:47:56
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_graphiteratortesttest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_graphiteratortesttest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

debug {
    INCLUDEPATH += X:\fluidicMachineModel\dll_debug\include
    LIBS += -L$$quote(X:\fluidicMachineModel\dll_debug\bin) -lFluidicMachineModel
}

!debug {
    INCLUDEPATH += X:\fluidicMachineModel\dll_release\include
    LIBS += -L$$quote(X:\fluidicMachineModel\dll_release\bin) -lFluidicMachineModel
}

INCLUDEPATH += X:\fluidicMachineModel\includes

