################################################################################
#
# sndfile-tools
#
################################################################################

SNDFILE_TOOLS_VERSION = 5545b7c381016ca881bfc100656edfa31d90337c
SNDFILE_TOOLS_SITE = $(call github,erikd,sndfile-tools,$(SNDFILE_TOOLS_VERSION))
SNDFILE_TOOLS_DEPENDENCIES = cairo fftw-double jack2mod libglib2 libsamplerate libsndfile
SNDFILE_TOOLS_AUTORECONF = YES

ifeq ($(BR2_cortex_a7),y)
SNDFILE_TOOLS_EXTRA_FLAGS = -fprefetch-loop-arrays -funroll-loops -funsafe-loop-optimizations
endif

SNDFILE_TOOLS_CONF_OPTS  = CFLAGS="$(TARGET_CFLAGS) -ffast-math $(SNDFILE_TOOLS_EXTRA_FLAGS)"
SNDFILE_TOOLS_CONF_OPTS += CXXFLAGS="$(TARGET_CXXFLAGS) -ffast-math $(SNDFILE_TOOLS_EXTRA_FLAGS)"

define SNDFILE_TOOLS_POST_INSTALL_TARGET_INTCLIENT
	mkdir -p $(TARGET_DIR)/usr/lib/jack
	mv $(TARGET_DIR)/usr/lib/sndfile-jackplay.so $(TARGET_DIR)/usr/lib/jack/
endef

SNDFILE_TOOLS_POST_INSTALL_TARGET_HOOKS += SNDFILE_TOOLS_POST_INSTALL_TARGET_INTCLIENT

$(eval $(autotools-package))
