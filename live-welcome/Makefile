
OBJECTS  = welcome.cpp.o
OBJECTS += src/AudioDiscovery.cpp.o
OBJECTS += src/KioskTabs.cpp.o
OBJECTS += src/moc_KioskSettingsPopup.cpp.o
OBJECTS += src/moc_KioskWindow.cpp.o
# OBJECTS += moc_welcome.cpp.o
# OBJECTS += theme/CarlaStyle.cpp.o
# OBJECTS += theme/moc_CarlaStyle.cpp.o
# OBJECTS += theme/moc_CarlaStyleAnimations.cpp.o
# OBJECTS += theme/moc_CarlaStylePrivate.cpp.o

TARGETS  = mod-live-usb-welcome

# CXXFLAGS  = -fvisibility=hidden
CXXFLAGS := -O0 -g

KF5_CFLAGS  = -I.
KF5_CFLAGS += -I/usr/include/KF5
KF5_CFLAGS += -I/usr/include/KF5/KCoreAddons
KF5_CFLAGS += -I/usr/include/KF5/KParts
KF5_CFLAGS += -I/usr/include/KF5/KService
KF5_CFLAGS += -I/usr/include/KF5/KXmlGui
KF5_CFLAGS += -I/usr/lib/qt/mkspecs/linux-g++
KF5_CFLAGS += -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++
KF5_LIBS    = -lKF5Parts -lKF5Service -lKF5CoreAddons

ALSA_CFLAGS = $(shell pkg-config --cflags alsa)
ALSA_LIBS   = $(shell pkg-config --libs alsa)

QT5_CFLAGS = $(shell pkg-config --cflags Qt5Widgets Qt5WebEngineWidgets) -fPIC
QT5_LIBS   = $(shell pkg-config --libs Qt5Widgets Qt5WebEngineWidgets)

CC  ?= gcc
CXX ?= g++

ifneq ($(wildcard /usr/lib/qt5/bin/moc),)
MOC ?= /usr/lib/qt5/bin/moc
else
MOC ?= moc
endif

all: $(TARGETS)

mod-live-usb-welcome: $(OBJECTS)
	$(CXX) $^ $(KF5_LIBS) $(QT5_LIBS) $(ALSA_LIBS) -ldl $(LDFLAGS) -o $@

welcome.cpp.o: welcome.cpp src/KioskWindow.hpp src/KioskTabs.hpp src/KioskSettingsPopup.hpp src/AudioDiscovery.hpp
	$(CXX) $< -c $(KF5_CFLAGS) $(QT5_CFLAGS) $(CXXFLAGS) -o $@

src/AudioDiscovery.cpp.o: src/AudioDiscovery.cpp src/AudioDiscovery.hpp
	$(CXX) $< -c $(ALSA_CFLAGS) $(CXXFLAGS) -o $@

src/KioskTabs.cpp.o: src/KioskTabs.cpp src/KioskTabs.hpp src/KioskAbout.hpp
	$(CXX) $< -c $(KF5_CFLAGS) $(QT5_CFLAGS) $(CXXFLAGS) -o $@

src/moc_%.cpp.o: src/moc_%.cpp
	$(CXX) $< -c $(KF5_CFLAGS) $(QT5_CFLAGS) $(CXXFLAGS) -o $@

src/moc_%.cpp: src/%.hpp
	$(MOC) $< > $@

# theme/moc_%.cpp: %.hpp
# 	$(MOC) $< > $@

clean:
	rm -f $(TARGETS) *.o theme/*.o
