
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

QT5_CFLAGS = $(shell pkg-config --cflags Qt5Network Qt5Widgets Qt5WebEngineWidgets Qt5X11Extras)
QT5_LIBS   = $(shell pkg-config --libs Qt5Network Qt5Widgets Qt5WebEngineWidgets Qt5X11Extras)

X11_CFLAGS = $(shell pkg-config --cflags x11)
X11_LIBS   = $(shell pkg-config --libs x11)

# ---------------------------------------------------------------------------------------------------------------------
# objects to build

FILES  = welcome.cpp
FILES += src/AudioContainerComm.cpp
FILES += src/AudioDiscovery.cpp
FILES += src/KioskForeignWidget.cpp
FILES += src/KioskTabs.cpp
FILES += src/PeakMeterThread.cpp
FILES += src/Utils.cpp
FILES += src/moc_KioskSettingsPopup.cpp
FILES += src/moc_KioskWindow.cpp
FILES += widgets/moc_collapsiblebox.cpp

OBJECTS = $(FILES:%=%.o)

# ---------------------------------------------------------------------------------------------------------------------
# targets

TARGETS  = mod-live-usb-welcome

all: $(TARGETS)

mod-live-usb-welcome: $(OBJECTS)
	$(CXX) $^ $(X11_LIBS) $(KF5_LIBS) $(QT5_LIBS) $(ALSA_LIBS) -ldl -lrt $(LDFLAGS) -o $@

src/KioskTabs.cpp.o: resources/watermark.txt

%.cpp.o: %.cpp
	$(CXX) $< -c $(X11_CFLAGS) $(KF5_CFLAGS) $(QT5_CFLAGS) $(ALSA_CFLAGS) $(CXXFLAGS) -o $@

src/moc_%.cpp: src/%.hpp
	$(MOC) $< > $@

widgets/moc_%.cpp: widgets/%.hpp
	$(MOC) $< > $@

resources/watermark.txt: resources/watermark.png
	$(shell cat $< | base64 | tr -d '\n' | sed 's/\(.*\)/"\1"/' > $@)

# ---------------------------------------------------------------------------------------------------------------------
# cleanup

clean:
	rm -f $(TARGETS) $(OBJECTS) $(OBJECTS:%.o=%.d) resources/*.txt

# ---------------------------------------------------------------------------------------------------------------------
# helpers

-include $(OBJECTS:%.o=%.d)

# ---------------------------------------------------------------------------------------------------------------------
