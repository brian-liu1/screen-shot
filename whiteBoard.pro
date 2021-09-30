#-------------------------------------------------
#
# Project created by QtCreator 2021-09-28T08:57:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = whiteBoard
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11
INCLUDEPATH += $$PWD/window \
               $$PWD/graphicview \
               $$PWD/command

SOURCES += main.cpp \
        window/widget.cpp \
        window/screenwidget.cpp \
        graphicview/graphicview.cpp \
        graphicview/graphicscene.cpp \
        graphicview/myline.cpp \
        graphicview/myrect.cpp \
        graphicview/textedit.cpp \
        command/mycommand.cpp \
        mybutton.cpp \
    graphicview/colorbar.cpp

HEADERS += window/widget.h \
           window/screenwidget.h \
           graphicview/graphicview.h \
           graphicview/graphicscene.h \
           graphicview/myline.h \
           graphicview/myrect.h \
           graphicview/textedit.h \
           command/mycommand.h \
           mybutton.h \
    graphicview/colorbar.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

RC_FILE = logo.rc

