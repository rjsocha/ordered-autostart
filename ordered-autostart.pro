QT += core widgets
CONFIG += release c++17
TEMPLATE = app
TARGET = ordered-autostart
SOURCES += src/main.cpp
DEFINES += QT_NO_KEYWORDS
CONFIG += link_pkgconfig
PKGCONFIG += gio-2.0

