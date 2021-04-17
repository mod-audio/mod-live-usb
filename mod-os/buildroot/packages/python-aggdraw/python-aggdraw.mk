################################################################################
#
# python-aggdraw
#
################################################################################

PYTHON_AGGDRAW_VERSION = 183299329e8bbfcc5bbc280da492cc0a2c242f55
PYTHON_AGGDRAW_SITE = $(call github,pytroll,aggdraw,$(PYTHON_AGGDRAW_VERSION))
PYTHON_AGGDRAW_DEPENDENCIES = python3 host-python-setuptools
PYTHON_AGGDRAW_SETUP_TYPE = setuptools
PYTHON_AGGDRAW_ENV = CC=$(TARGET_CC)

$(eval $(python-package))
