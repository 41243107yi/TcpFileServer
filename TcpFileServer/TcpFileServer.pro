QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TcpFileServer
TEMPLATE = app

SOURCES += main.cpp \
           tabwidge.cpp \
           tcpfilesender.cpp \
           tcpfileserver.cpp

HEADERS  += tabwidge.h \
            tcpfilesender.h \
            tcpfileserver.h
