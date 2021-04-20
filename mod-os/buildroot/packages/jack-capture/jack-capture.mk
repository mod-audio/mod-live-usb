################################################################################
#
# jack-capture
#
################################################################################

# JACK_CAPTURE_VERSION = 0.9.71
# JACK_CAPTURE_SITE = http://archive.notam02.no/arkiv/src
# JACK_CAPTURE_SOURCE = jack_capture-$(JACK_CAPTURE_VERSION).tar.gz
JACK_CAPTURE_VERSION = e2c0c3d25fce0a13f3f18c622079cd29178af7f8
JACK_CAPTURE_SITE = $(call github,kmatheussen,jack_capture,$(JACK_CAPTURE_VERSION))

JACK_CAPTURE_DEPENDENCIES = jack2mod libsndfile

JACK_CAPTURE_TARGET_MAKE = \
	$(TARGET_MAKE_ENV) \
	$(TARGET_CONFIGURE_OPTS) \
	CFLAGS="$(TARGET_CFLAGS) -ffast-math $(JACK_CAPTURE_EXTRA_FLAGS)" \
	CXXFLAGS="$(TARGET_CXXFLAGS) -ffast-math $(JACK_CAPTURE_EXTRA_FLAGS)" \
	$(MAKE) -C $(@D)

define JACK_CAPTURE_CONFIGURE_CMDS
	$(JACK_CAPTURE_TARGET_MAKE) jack_capture
endef

define JACK_CAPTURE_INSTALL_TARGET_CMDS
	$(JACK_CAPTURE_TARGET_MAKE) install DESTDIR=$(TARGET_DIR) PREFIX=/usr
endef

$(eval $(generic-package))
