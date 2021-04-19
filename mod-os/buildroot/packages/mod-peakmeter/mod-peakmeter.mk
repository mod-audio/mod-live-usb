######################################
#
# mod-peakmeter
#
######################################

MOD_PEAKMETER_VERSION = 5bebb31946c9f5d4560f1a129958d55195ccd18f
MOD_PEAKMETER_SITE = $(call github,moddevices,mod-peakmeter,$(MOD_PEAKMETER_VERSION))
MOD_PEAKMETER_DEPENDENCIES = jack2mod

MOD_PEAKMETER_TARGET_MAKE = \
	$(TARGET_MAKE_ENV) \
	$(TARGET_CONFIGURE_OPTS) \
	CFLAGS="$(TARGET_CFLAGS) -ffast-math $(MOD_PEAKMETER_EXTRA_FLAGS)" \
	CXXFLAGS="$(TARGET_CXXFLAGS) -ffast-math $(MOD_PEAKMETER_EXTRA_FLAGS)" \
	$(MAKE) -C $(@D)

define MOD_PEAKMETER_BUILD_CMDS
	$(MOD_PEAKMETER_TARGET_MAKE)
endef

define MOD_PEAKMETER_INSTALL_TARGET_CMDS
	install -d $(TARGET_DIR)/usr/lib/jack/
	install -m 644 $(@D)/mod-peakmeter.so $(TARGET_DIR)/usr/lib/jack/
endef

$(eval $(generic-package))
