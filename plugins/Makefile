
PLAT = $(shell bash -c "source ../.common.env && echo \$$PLAT")

# list of known good projects
PROJECTS += abgate
PROJECTS += aidadsp-lv2
PROJECTS += amsynth
PROJECTS += artyfx
PROJECTS += bolliedelay
PROJECTS += caps-lv2
PROJECTS += carla-plugins
PROJECTS += chow-centaur
PROJECTS += collisiondrive
PROJECTS += die-plugins
# PROJECTS += distrho-ports
PROJECTS += dpf-plugins
PROJECTS += dragonfly-reverb
PROJECTS += fluidplug
PROJECTS += fomp
PROJECTS += freaked
PROJECTS += guitarix
PROJECTS += guitarix-studiopre
PROJECTS += gxbajatubedriver
PROJECTS += gxbottlerocket
PROJECTS += gxguvnor
PROJECTS += gxhotbox
PROJECTS += gxhyperion
PROJECTS += gxknightfuzz
PROJECTS += gxquack
PROJECTS += gxsaturate
PROJECTS += gxsd1
PROJECTS += gxsd2lead
PROJECTS += gxsfp
PROJECTS += gxslowgear
PROJECTS += gxsupersonic
PROJECTS += gxsvt
PROJECTS += gxswitchlesswah
PROJECTS += gxtimray
PROJECTS += gxtonemachine
PROJECTS += gxuvox
PROJECTS += gxvbass
PROJECTS += gxvintagefuzzmaster
PROJECTS += gxvmk2d
PROJECTS += gxvoodoo
PROJECTS += gxvoxtb
PROJECTS += gxvstb
PROJECTS += infamous-plugins
PROJECTS += invada-lv2
PROJECTS += mod-cv-plugins
# PROJECTS += lsp-plugins-impulsantworten
PROJECTS += lv2-examples
PROJECTS += mod-ams-lv2
PROJECTS += mod-arpeggiator
PROJECTS += mod-audio-mixers
PROJECTS += mod-cabsim-ir-loader
PROJECTS += mod-convolution-loader
PROJECTS += mod-cv-plugins
PROJECTS += mod-distortion
PROJECTS += mod-mda-lv2
PROJECTS += mod-midi-utilities
PROJECTS += mod-pitchshifter
PROJECTS += mod-tal-noisemaker
PROJECTS += mod-utilities
PROJECTS += mod-volume
PROJECTS += modmeter
PROJECTS += modspectre
PROJECTS += notes-lv2
PROJECTS += neural-amp-modeler-lv2
PROJECTS += remaincalm-plugins
PROJECTS += rkrlv2
# PROJECTS += samplv1
PROJECTS += schrammel-ojd
PROJECTS += screcord
PROJECTS += setbfree-mod
PROJECTS += setbfree
PROJECTS += shiro-plugins
PROJECTS += sooperlooper-lv2
PROJECTS += tal-reverb
PROJECTS += tap-lv2
PROJECTS += x42-fat1
PROJECTS += x42-fil4
PROJECTS += x42-mclk
PROJECTS += x42-midifilter
PROJECTS += x42-midigen
PROJECTS += x42-mtc
PROJECTS += x42-stepseq
PROJECTS += x42-tinygain
PROJECTS += x42-xfade
PROJECTS += x42-zconvo
# not in stable/prod, but still useful for many reasons
PROJECTS += crypt
PROJECTS += bolliedelayxt
PROJECTS += dexed-labs
PROJECTS += dm-graindelay
PROJECTS += portal-lv2
PROJECTS += x42-tuna-lv2-labs
# not in stable/prod, not ready no modgui, but I still want it
# PROJECTS += chow-kick
# PROJECTS += distrho-ports-v6

# FIXME use qemu tricks
ifeq ($(PLAT),modduo-new)
XPLA_COMPAT = true
else ifeq ($(PLAT),modduox-new)
XPLA_COMPAT = true
endif

ifeq ($(XPLA_COMPAT),true)
PROJECTS += calf
PROJECTS += pitchtracking-series
PROJECTS += zam-plugins
PROJECTS += zynaddsubfx-labs
endif

# function to convert project name into buildroot var name
BUILDROOT_VAR = $(shell echo $(1) | tr a-z A-Z | tr - _)

# function to convert project name into stamp filename
PROJECT_STAMP = .stamps/$(PLAT)/$(1)/$($(call BUILDROOT_VAR,$(1))_VERSION)

# utilities
BLANK =
COMMA = ,
SPACE = $(BLANK) $(BLANK)

# include plugin projects for version and bundle list
include $(foreach PROJECT,$(PROJECTS),mod-plugin-builder/plugins/package/$(PROJECT)/$(PROJECT).mk)

# list of known good bundles
BUNDLES = $(foreach PROJECT,$(PROJECTS),$($(call BUILDROOT_VAR,$(PROJECT))_BUNDLES))

all: $(foreach PROJECT,$(PROJECTS),$(call PROJECT_STAMP,$(PROJECT)))

define BUILD_PLUGIN
	./docker-run.sh -v $(CURDIR)/bundles/$(PLAT):/opt/mount --rm mpb-plugins-$(PLAT):latest \
		/home/builder/build-and-copy-bundles.sh $(PLAT) $(1) \
		$(subst $(SPACE),$(COMMA),$(filter $(BUNDLES),$($(call BUILDROOT_VAR,$(1))_BUNDLES)))
endef

.stamps/$(PLAT)/%: # .stamp_built
	$(shell mkdir -p $(shell dirname $@) $(CURDIR)/bundles/$(PLAT))
	$(call BUILD_PLUGIN,$(firstword $(subst /, ,$*)))
	touch $@
