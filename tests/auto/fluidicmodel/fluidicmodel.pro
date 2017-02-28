#-------------------------------------------------
#
# Project created by QtCreator 2017-02-28T18:51:50
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_fluidicmodeltest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_fluidicmodeltest.cpp \
    prologexecutor.cpp \
    prologtranslationstack.cpp \
    stringpluginfactory.cpp \
    stringvalveproduct.cpp \
    stringpumpproduct.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    prologexecutor.h \
    prologtranslationstack.h \
    stringpluginfactory.h \
    stringvalveproduct.h \
    stringpumpproduct.h

debug {
    INCLUDEPATH += X:\fluidicMachineModel\dll_debug\include
    LIBS += -L$$quote(X:\fluidicMachineModel\dll_debug\bin) -lFluidicMachineModel
}

!debug {
    INCLUDEPATH += X:\fluidicMachineModel\dll_release\include
    LIBS += -L$$quote(X:\fluidicMachineModel\dll_release\bin) -lFluidicMachineModel
}

INCLUDEPATH += X:\fluidicMachineModel\includes

INCLUDEPATH += X:\swipl\include
LIBS += -L$$quote(X:\swipl\bin) -llibswipl
LIBS += -L$$quote(X:\swipl\lib) -llibswipl
