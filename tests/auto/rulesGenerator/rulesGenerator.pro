#-------------------------------------------------
#
# Project created by QtCreator 2017-02-03T17:33:03
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_rulesgeneratortest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_rulesgeneratortest.cpp \
    stringtranslationstack.cpp \
    prologtranslationstack.cpp
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
    stringtranslationstack.h \
    prologtranslationstack.h

RESOURCES += \
    additionaltestfiles.qrc
