######################################
#
# mod-peakmeter
#
######################################

MOD_PEAKMETER_VERSION = 0694c717e9c25c7b14d81e34046ddfc271f26799
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
