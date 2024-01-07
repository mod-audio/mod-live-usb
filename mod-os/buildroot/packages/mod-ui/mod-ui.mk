######################################
#
# mod-ui
#
######################################

MOD_UI_VERSION = c5e4f5d10cfd5d696b99ee338598948527f7f11b
MOD_UI_SITE = $(call github,moddevices,mod-ui,$(MOD_UI_VERSION))
MOD_UI_DEPENDENCIES = python3 python-aggdraw python-pillow python-pycrypto python-pystache python-setuptools python-serial python-tornado host-python3 jack-capture jack2mod lilv mod-host
MOD_UI_SETUP_TYPE = distutils
MOD_UI_ENV = CXX=$(TARGET_CXX)

$(eval $(python-package))
