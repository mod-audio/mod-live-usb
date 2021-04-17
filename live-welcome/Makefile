
SOURCES = welcome.cpp
OBJECTS = welcome.cpp.o moc_welcome.cpp.o
TARGETS = mod-live-usb-welcome

# CXXFLAGS  = -fvisibility=hidden
CXXFLAGS := -O0 -g

KF5_CFLAGS  = -I/usr/include/KF5
KF5_CFLAGS += -I/usr/include/KF5/KCoreAddons
KF5_CFLAGS += -I/usr/include/KF5/KParts
KF5_CFLAGS += -I/usr/include/KF5/KService
KF5_CFLAGS += -I/usr/include/KF5/KXmlGui
KF5_CFLAGS += -I/usr/lib/qt/mkspecs/linux-g++
KF5_CFLAGS += -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++
KF5_LIBS    = -lKF5Parts -lKF5Service -lKF5CoreAddons

QT5_CFLAGS = $(shell pkg-config --cflags Qt5Widgets Qt5WebEngineWidgets) -fPIC
QT5_LIBS   = $(shell pkg-config --libs Qt5Widgets Qt5WebEngineWidgets)

CC  ?= gcc
CXX ?= g++
MOC ?= moc
# /usr/lib/qt5/bin/moc

all: $(TARGETS)

mod-live-usb-welcome: $(OBJECTS)
	$(CXX) $^ $(KF5_LIBS) $(QT5_LIBS) $(LDFLAGS) -o $@

%.cpp.o: %.cpp
	$(CXX) $< -c $(KF5_CFLAGS) $(QT5_CFLAGS) $(CXXFLAGS) -o $@

# moc_%.cpp.o: moc_%.cpp
# 	$(CXX) $< -c $(KF5_CFLAGS) $(QT5_CFLAGS) $(CXXFLAGS) -o $@

moc_%.cpp: %.hpp
	$(MOC) $< > $@

