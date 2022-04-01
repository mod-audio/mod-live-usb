
# ---------------------------------------------------------------------------------------------------------------------
# build tools

CC  ?= gcc
CXX ?= g++

ifneq ($(wildcard /usr/lib/qt5/bin/moc),)
MOC ?= /usr/lib/qt5/bin/moc
else
MOC ?= moc
endif

# ---------------------------------------------------------------------------------------------------------------------
# build flags

# CXXFLAGS  = -fvisibility=hidden
CXXFLAGS += -O2
CXXFLAGS += -g
CXXFLAGS += -I.
CXXFLAGS += -pipe -MD -MP
CXXFLAGS += -fPIC

KF5_CFLAGS += -I/usr/include/KF5
KF5_CFLAGS += -I/usr/include/KF5/KCoreAddons
KF5_CFLAGS += -I/usr/include/KF5/KParts
KF5_CFLAGS += -I/usr/include/KF5/KService
KF5_CFLAGS += -I/usr/include/KF5/KXmlGui
KF5_CFLAGS += -I/usr/lib/qt/mkspecs/linux-g++
KF5_CFLAGS += -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++
KF5_LIBS   += -lKF5Parts -lKF5Service -lKF5CoreAddons

ALSA_CFLAGS = $(shell pkg-config --cflags alsa)
ALSA_LIBS   = $(shell pkg-config --libs alsa)

QT5_CFLAGS = $(shell pkg-config --cflags Qt5Widgets Qt5WebEngineWidgets Qt5X11Extras)
QT5_LIBS   = $(shell pkg-config --libs Qt5Widgets Qt5WebEngineWidgets Qt5X11Extras)

X11_CFLAGS = $(shell pkg-config --cflags x11)
X11_LIBS   = $(shell pkg-config --libs x11)

# ---------------------------------------------------------------------------------------------------------------------
# objects to build

OBJECTS  = welcome.cpp.o
OBJECTS += src/AudioContainerComm.cpp.o
OBJECTS += src/AudioDiscovery.cpp.o
OBJECTS += src/KioskForeignWidget.cpp.o
OBJECTS += src/KioskTabs.cpp.o
OBJECTS += src/PeakMeterThread.cpp.o
OBJECTS += src/Utils.cpp.o
OBJECTS += src/moc_KioskSettingsPopup.cpp.o
OBJECTS += src/moc_KioskWindow.cpp.o

# OBJECTS += moc_welcome.cpp.o
# OBJECTS += theme/CarlaStyle.cpp.o
# OBJECTS += theme/moc_CarlaStyle.cpp.o
# OBJECTS += theme/moc_CarlaStyleAnimations.cpp.o
# OBJECTS += theme/moc_CarlaStylePrivate.cpp.o

# ---------------------------------------------------------------------------------------------------------------------
# targets

TARGETS  = mod-live-usb-welcome

all: $(TARGETS)

mod-live-usb-welcome: $(OBJECTS)
	$(CXX) $^ $(X11_LIBS) $(KF5_LIBS) $(QT5_LIBS) $(ALSA_LIBS) -ldl -lrt $(LDFLAGS) -o $@
# 
# welcome.cpp.o: welcome.cpp
# 	$(CXX) $< -c $(KF5_CFLAGS) $(QT5_CFLAGS) $(CXXFLAGS) -o $@
# 
# src/AudioDiscovery.cpp.o: src/AudioDiscovery.cpp
# 	$(CXX) $< -c $(ALSA_CFLAGS) $(CXXFLAGS) -o $@
# 

src/KioskTabs.cpp.o: resources/watermark.txt

# 	$(CXX) $< -c $(KF5_CFLAGS) $(QT5_CFLAGS) $(CXXFLAGS) -o $@
# 
# src/Utils.cpp.o: src/Utils.cpp src/Utils.hpp
# 	$(CXX) $< -c $(KF5_CFLAGS) $(QT5_CFLAGS) $(CXXFLAGS) -o $@

# src/moc_%.cpp.o: src/moc_%.cpp
# 	$(CXX) $< -c $(KF5_CFLAGS) $(QT5_CFLAGS) $(CXXFLAGS) -o $@

%.cpp.o: %.cpp
	$(CXX) $< -c $(X11_CFLAGS) $(KF5_CFLAGS) $(QT5_CFLAGS) $(ALSA_CFLAGS) $(CXXFLAGS) -o $@

src/moc_%.cpp: src/%.hpp
	$(MOC) $< > $@

resources/watermark.txt: resources/watermark.png
	$(shell cat $< | base64 | tr -d '\n' | sed 's/\(.*\)/"\1"/' > $@)

# theme/moc_%.cpp: %.hpp
# 	$(MOC) $< > $@

# ---------------------------------------------------------------------------------------------------------------------
# cleanup

clean:
	rm -f $(TARGETS) *.o */*.o resources/*.txt

# ---------------------------------------------------------------------------------------------------------------------
# helpers

-include $(OBJECTS:%.o=%.d)

# ---------------------------------------------------------------------------------------------------------------------
