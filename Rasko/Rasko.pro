#-------------------------------------------------
#
# Project created by QtCreator 2016-10-30T14:28:03
#
#-------------------------------------------------

QT       += core gui network


#INCLUDEPATH += "C:/crpytopp/include"
#LIBS += -L"C:\crpytopp\cryptopp-CRYPTOPP_5_6_5\Win32\Output\Debug" -lcryptlib




greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
include(./QHotkey/qhotkey.pri)


TARGET = Rasko
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp  \
        startup_window.cpp \
    passwordprompt.cpp \
    accountnew_edit.cpp \
    simplecrypt.cpp \
    setting.cpp \
    pass_gen.cpp

HEADERS  += mainwindow.h  \
    startup_window.h \
    passwordprompt.h \
    accountnew_edit.h \
    simplecrypt.h \
    setting.h \
    pass_gen.h

FORMS    +=  mainwindow.ui \
   startup_window.ui \
    passwordprompt.ui \
    accountnew_edit.ui \
    setting.ui \
    pass_gen.ui

win32: RC_FILE = application.rc


RESOURCES += \
    resouces.qrc
