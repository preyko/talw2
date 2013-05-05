#-------------------------------------------------
#
# Project created by QtCreator 2013-05-04T08:40:22
#
#-------------------------------------------------

QT       += core gui

TARGET = RLInterpreterGUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

symbian: LIBS += -lrlprecomp
else:unix|win32: LIBS += -L$$PWD/../../RLPrecompiler/build/ -lrlprecomp

INCLUDEPATH += $$PWD/../../RLPrecompiler
DEPENDPATH += $$PWD/../../RLPrecompiler

win32: PRE_TARGETDEPS += $$PWD/../../RLPrecompiler/build/rlprecomp.lib
else:unix:!symbian: PRE_TARGETDEPS += $$PWD/../../RLPrecompiler/build/librlprecomp.a

symbian: LIBS += -lrltype
else:unix|win32: LIBS += -L$$PWD/../../rlinterpreter/ -lrltype

INCLUDEPATH += $$PWD/../../rlinterpreter
DEPENDPATH += $$PWD/../../rlinterpreter

win32: PRE_TARGETDEPS += $$PWD/../../rlinterpreter/rltype.lib
else:unix:!symbian: PRE_TARGETDEPS += $$PWD/../../rlinterpreter/librltype.a
