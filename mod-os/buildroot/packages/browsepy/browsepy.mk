################################################################################
#
# browsepy
#
################################################################################

BROWSEPY_VERSION = 368e6918f10175111956c89907f7baec4efe99b3
BROWSEPY_SITE = $(call github,moddevices,browsepy,$(BROWSEPY_VERSION))
BROWSEPY_DEPENDENCIES = python3 python-flask python-scandir host-python-setuptools
BROWSEPY_SETUP_TYPE = setuptools

$(eval $(python-package))
