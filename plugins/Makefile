
# list of known good projects
PROJECTS += abgate
PROJECTS += amsynth
PROJECTS += artyfx
PROJECTS += bolliedelay
PROJECTS += calf
PROJECTS += caps-lv2
PROJECTS += carla-plugins
PROJECTS += collisiondrive
PROJECTS += die-plugins
# PROJECTS += distrho-ports
PROJECTS += dpf-plugins
PROJECTS += fluidplug
PROJECTS += fomp
PROJECTS += freaked
PROJECTS += guitarix
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
# PROJECTS += mod-audio-to-cv-pitch
PROJECTS += mod-cabsim-ir-loader
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
PROJECTS += remaincalm-plugins
PROJECTS += rkrlv2
# PROJECTS += samplv1
PROJECTS += setbfree-mod
PROJECTS += setbfree
# PROJECTS += shiro-plugins # FIXME build broken, submodules using git:// github stuff
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
PROJECTS += zam-plugins
# not in stable/prod, but still useful for many reasons
# PROJECTS += cardinal
PROJECTS += dexed-labs
PROJECTS += screcord
PROJECTS += x42-tuna-lv2-labs
# not in stable/prod, not ready no modgui, but I still want it
# PROJECTS += chow-kick
# PROJECTS += distrho-ports-v6

# function to convert project name into buildroot var name
BUILDROOT_VAR = $(shell echo $(1) | tr a-z A-Z | tr - _)

# function to convert project name into stamp filename
PROJECT_STAMP = .stamps/$(1)/$($(call BUILDROOT_VAR,$(1))_VERSION)

# utilities
COMMA  = ,
SPACE  =
SPACE +=

# include plugin projects for version and bundle list
include $(foreach PROJECT,$(PROJECTS),mod-plugin-builder/plugins/package/$(PROJECT)/$(PROJECT).mk)

# list of known good bundles
BUNDLES = $(foreach PROJECT,$(PROJECTS),$($(call BUILDROOT_VAR,$(PROJECT))_BUNDLES))

all: $(foreach PROJECT,$(PROJECTS),$(call PROJECT_STAMP,$(PROJECT)))

define BUILD_PLUGIN
	docker run -v $(CURDIR)/bundles:/opt/mount --rm mpb-plugins-x86_64:latest \
		/home/builder/build-and-copy-bundles.sh $(1) \
		$(subst $(SPACE),$(COMMA),$(filter $(BUNDLES),$($(call BUILDROOT_VAR,$(1))_BUNDLES)))
endef

.stamps/%: # .stamp_built
	$(shell mkdir -p $(shell dirname $@) $(CURDIR)/bundles)
	$(call BUILD_PLUGIN,$(firstword $(subst /, ,$*)))
	touch $@
