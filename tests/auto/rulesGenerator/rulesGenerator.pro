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
    prologtranslationstack.cpp \
    dummycalculator.cpp \
    prologexecutor.cpp
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

INCLUDEPATH += X:\swipl\include
LIBS += -L$$quote(X:\swipl\bin) -llibswipl
LIBS += -L$$quote(X:\swipl\lib) -llibswipl

HEADERS += \
    stringtranslationstack.h \
    prologtranslationstack.h \
    dummycalculator.h \
    prologexecutor.h

RESOURCES += \
    additionaltestfiles.qrc
