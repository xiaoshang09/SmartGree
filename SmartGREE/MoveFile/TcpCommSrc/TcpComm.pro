#-------------------------------------------------
#
# Project created by QtCreator 2017-10-12T17:47:56
#
#-------------------------------------------------

QT       += core network

CONFIG += c++11
#QMAKE_CXXFLAGS +=-O2 -march=i686 -mtune=i686 -std=c++0x

TARGET = TcpComm
TEMPLATE = lib
DEFINES += TCPCOMM_LIBRARY

SOURCES +=  \
    $$PWD/tcpserver.cpp \
    $$PWD/tcpsocket.cpp \
    $$PWD/tcpthread.cpp \
    $$PWD/customprotocol.cpp \
    $$PWD/crc32.cpp \
    $$PWD/crc16.cpp \
    $$PWD/tcpconfig.cpp \
    $$PWD/tcpserverpool.cpp

HEADERS += \
    $$PWD/tcpcomm_global.h \
    $$PWD/tcpserver.h \
    $$PWD/tcpsocket.h \
    $$PWD/tcpthread.h \
    $$PWD/customprotocol.h \
    $$PWD/crc32.h \
    $$PWD/crc16.h \
    $$PWD/tcpconfig.h \
    $$PWD/tcpserverpool.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
