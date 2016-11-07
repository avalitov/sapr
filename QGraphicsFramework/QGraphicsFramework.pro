#-------------------------------------------------
#
# Project created by QtCreator 2015-10-28T14:30:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QGraphicsFramework1
TEMPLATE = app

CONFIG += c++11

#LIBS += -LC:/libs/SFML-2.3.2-build/lib
#CONFIG(release, debug|release): LIBS += -lsfml-audio -lsfml-graphics -lsfml-main -lsfml-network -lsfml-window -lsfml-system
##CONFIG(debug, debug|release): LIBS += -lsfml-audio-d -lsfml-graphics-d -lsfml-main-d -lsfml-network-d -lsfml-window-d -lsfml-system-d
#CONFIG(debug, debug|release): LIBS += -lsfml-audio -lsfml-graphics -lsfml-main -lsfml-network -lsfml-window -lsfml-system

#INCLUDEPATH += C:/libs/SFML-2.3.2/include
#DEPENDPATH += C:/libs/SFML-2.3.2/include


SOURCES += main.cpp\
        mainwindow.cpp \
    init.cpp \
#    graphics.cpp \
    error1.cpp \
    processor.cpp \
    my_matrix.cpp \
    graphics1.cpp

HEADERS  += mainwindow.h \
    my_matrix.h

FORMS    += mainwindow.ui

RESOURCES +=
