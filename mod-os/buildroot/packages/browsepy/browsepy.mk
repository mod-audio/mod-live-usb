################################################################################
#
# browsepy
#
################################################################################

BROWSEPY_VERSION = c2245873f9432b2839f09be7eeb2992fd3bbc5ff
BROWSEPY_SITE = $(call github,moddevices,browsepy,$(BROWSEPY_VERSION))
BROWSEPY_DEPENDENCIES = python3 python-flask python-scandir host-python-setuptools
BROWSEPY_SETUP_TYPE = setuptools

$(eval $(python-package))
