######################################
#
# mod-ui
#
######################################

MOD_UI_VERSION = 9e67eb4aab39e9d63ca8fe7c33bb52fc404987c8
MOD_UI_SITE = $(call github,moddevices,mod-ui,$(MOD_UI_VERSION))
MOD_UI_DEPENDENCIES = python3 python-aggdraw python-pillow python-pycrypto python-pystache python-setuptools python-serial python-tornado host-python3 jack-capture jack2mod lilv mod-host
MOD_UI_SETUP_TYPE = distutils
MOD_UI_ENV = CXX=$(TARGET_CXX)

$(eval $(python-package))
