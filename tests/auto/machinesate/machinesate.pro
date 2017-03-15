#-------------------------------------------------
#
# Project created by QtCreator 2017-03-01T16:59:02
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_machinesatetest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_machinesatetest.cpp
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
