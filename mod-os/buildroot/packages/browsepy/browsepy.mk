################################################################################
#
# browsepy
#
################################################################################

BROWSEPY_VERSION = c1040717bf5b910fd9791464c414daa209180270
BROWSEPY_SITE = $(call github,moddevices,browsepy,$(BROWSEPY_VERSION))
BROWSEPY_DEPENDENCIES = python3 python-flask python-scandir host-python-setuptools
BROWSEPY_SETUP_TYPE = setuptools

$(eval $(python-package))
