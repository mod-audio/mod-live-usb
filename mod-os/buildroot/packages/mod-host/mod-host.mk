######################################
#
# mod-host
#
######################################

MOD_HOST_VERSION = a8a0a2eed4b11d46118367006f4ba1f429404c67
MOD_HOST_SITE = $(call github,moddevices,mod-host,$(MOD_HOST_VERSION))
MOD_HOST_DEPENDENCIES = jack2mod lilv readline fftw-single fftw-double

MOD_HOST_TARGET_CFLAGS = $(TARGET_CFLAGS) -ffast-math
MOD_HOST_TARGET_MAKE = $(TARGET_MAKE_ENV) $(TARGET_CONFIGURE_OPTS) CFLAGS="$(MOD_HOST_TARGET_CFLAGS)" $(MAKE) -C $(@D)

define MOD_HOST_BUILD_CMDS
	$(MOD_HOST_TARGET_MAKE)
endef

define MOD_HOST_INSTALL_TARGET_CMDS
	$(MOD_HOST_TARGET_MAKE) install DESTDIR=$(TARGET_DIR) PREFIX=/usr
endef

$(eval $(generic-package))
