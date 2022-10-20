######################################
#
# mod-ui
#
######################################

MOD_UI_VERSION = d329b5117ef4ddb7c1205cac22a5ebd8c4c725b6
MOD_UI_SITE = $(call github,moddevices,mod-ui,$(MOD_UI_VERSION))
MOD_UI_DEPENDENCIES = python3 python-aggdraw python-pillow python-pycrypto python-pystache python-setuptools python-serial python-tornado host-python3 jack-capture jack2mod lilv mod-host
MOD_UI_SETUP_TYPE = distutils
MOD_UI_ENV = CXX=$(TARGET_CXX)

$(eval $(python-package))
