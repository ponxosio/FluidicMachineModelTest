#-------------------------------------------------
#
# Project created by QtCreator 2017-06-21T11:52:39
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_pythonpluginstest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += tst_pythonpluginstest.cpp \
    stringtranslationstack.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

debug {
    INCLUDEPATH += X:\fluidicMachineModel\dll_debug\include
    LIBS += -L$$quote(X:\fluidicMachineModel\dll_debug\bin) -lFluidicMachineModel

    INCLUDEPATH += X:\utils\dll_debug\include
    LIBS += -L$$quote(X:\utils\dll_debug\bin) -lutils

    INCLUDEPATH += X:\commomModel\dll_debug\include
    LIBS += -L$$quote(X:\commomModel\dll_debug\bin) -lcommonModel

    INCLUDEPATH += X:\protocolGraph\dll_debug\include
    LIBS += -L$$quote(X:\protocolGraph\dll_debug\bin) -lprotocolGraph

    INCLUDEPATH += X:\pythonPlugins\dll_debug\include
    LIBS += -L$$quote(X:\pythonPlugins\dll_debug\bin) -lpythonPlugins

    INCLUDEPATH +=X:\constraintsEngine\dll_debug\include
    LIBS += -L$$quote(X:\constraintsEngine\dll_debug\bin) -lconstraintsEngineLibrary

    LIBS += -L$$quote(X:\libraries\boost_1_63_0\stage\lib) -lboost_python-vc140-mt-gd-1_63
}

!debug {
    INCLUDEPATH += X:\fluidicMachineModel\dll_release\include
    LIBS += -L$$quote(X:\fluidicMachineModel\dll_release\bin) -lFluidicMachineModel

    INCLUDEPATH += X:\utils\dll_release\include
    LIBS += -L$$quote(X:\utils\dll_release\bin) -lutils

    INCLUDEPATH += X:\commomModel\dll_release\include
    LIBS += -L$$quote(X:\commomModel\dll_release\bin) -lcommonModel

    INCLUDEPATH += X:\protocolGraph\dll_release\include
    LIBS += -L$$quote(X:\protocolGraph\dll_release\bin) -lprotocolGraph

    INCLUDEPATH += X:\pythonPlugins\dll_release\include
    LIBS += -L$$quote(X:\pythonPlugins\dll_release\bin) -lpythonPlugins

    INCLUDEPATH +=X:\constraintsEngine\dll_release\include
    LIBS += -L$$quote(X:\constraintsEngine\dll_release\bin) -lconstraintsEngineLibrary

    LIBS += -L$$quote(X:\libraries\boost_1_63_0\stage\lib) -lboost_python-vc140-mt-1_63
}

INCLUDEPATH += X:\libraries\cereal-1.2.2\include
INCLUDEPATH += X:\libraries\boost_1_63_0

#python environment
INCLUDEPATH += C:\Python27\include
LIBS += -L$$quote(C:\Python27\libs)

#prolog environment
INCLUDEPATH += X:\swipl\include
LIBS += -L$$quote(X:\swipl\bin) -llibswipl
LIBS += -L$$quote(X:\swipl\lib) -llibswipl

HEADERS += \
    debugcommandsender.h \
    stringtranslationstack.h
