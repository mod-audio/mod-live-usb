######################################
#
# mod-ui
#
######################################

# The mod-ui revision must necessarily be from branch `plugin-store`
MOD_UI_VERSION = ac0c5d285fc84fdd2e6a9020706c9bb62dba05a1
MOD_UI_SITE = $(call github,moddevices,mod-ui,$(MOD_UI_VERSION))
MOD_UI_DEPENDENCIES = python3 python-aggdraw python-pillow python-pycrypto python-pystache python-setuptools python-serial python-tornado host-python3 jack2mod lilv
MOD_UI_SETUP_TYPE = distutils
MOD_UI_ENV = CXX=$(TARGET_CXX)

$(eval $(python-package))
