
QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_fluidicmachinetest.cpp

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
