QT += core widgets
CONFIG += release
TEMPLATE = app
TARGET = ordered-autostart
SOURCES += src/main.cpp
QMAKE_LFLAGS_RELEASE += -s
