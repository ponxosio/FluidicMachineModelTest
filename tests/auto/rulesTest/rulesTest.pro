#-------------------------------------------------
#
# Project created by QtCreator 2017-01-30T11:21:50
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_rulestesttest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_rulestesttest.cpp \
        stringtranslationstack.cpp

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

HEADERS += \
    stringtranslationstack.h

