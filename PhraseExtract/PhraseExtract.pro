#-------------------------------------------------
#
# Project created by QtCreator 2016-05-04T11:15:21
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = ../out/bin/PhraseExtract
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

LIBS += -lboost_iostreams -lz

INCLUDEPATH += /usr/include/boost

SOURCES += main.cpp \
    SentenceAlignment.cpp \
    TablesCore.cpp \
    InputFileStream.cpp \
    OutputFileStream.cpp \
    clsInput.cpp

HEADERS += \
    SentenceAlignment.h \
    PhraseExtractionOptions.h \
    TablesCore.h \
    InputFileStream.h \
    gzfilebuf.h \
    OutputFileStream.h \
    clsInput.h
