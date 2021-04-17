
# list of known good projects
PROJECTS += abgate
PROJECTS += amsynth
PROJECTS += artyfx
PROJECTS += bolliedelay
# PROJECTS += calf
PROJECTS += caps-lv2
PROJECTS += carla-plugins
# PROJECTS += die-plugins
PROJECTS += dpf-plugins

# list of known good bundles
BUNDLES += abGate.lv2
BUNDLES += amsynth.lv2
BUNDLES += artyfx.lv2
BUNDLES += bolliedelay.lv2
BUNDLES += calf.lv2
BUNDLES += mod-caps-AmpVTS.lv2 mod-caps-AutoFilter.lv2 mod-caps-CabinetIV.lv2 mod-caps-CEO.lv2 mod-caps-Click.lv2 mod-caps-PhaserII.lv2 mod-caps-Plate.lv2 mod-caps-PlateX2.lv2 mod-caps-Scape.lv2 mod-caps-Spice.lv2 mod-caps-SpiceX2.lv2 mod-caps-White.lv2 mod-caps-Wider.lv2
BUNDLES += carla-files.lv2
BUNDLES += distrho-a-fluidsynth.lv2
BUNDLES += Kars.lv2 MVerb.lv2 Nekobi.lv2 PingPongPan.lv2

# include plugin projects for version and bundle list
# TODO only import known-good projects
MAKEFILES = $(wildcard mod-plugin-builder/plugins/package/*/*.mk)
include $(MAKEFILES)

# function to convert project name into buildroot var name
BUILDROOT_VAR = $(shell echo $(1) | tr a-z A-Z | tr - _)

# function to convert project name into stamp filename
PROJECT_STAMP = .stamps/$(1)/$($(call BUILDROOT_VAR,$(1))_VERSION)

# utilities
COMMA  = ,
SPACE  =
SPACE +=

all: $(foreach PROJECT,$(PROJECTS),$(call PROJECT_STAMP,$(PROJECT)))

define BUILD_PLUGIN
	docker run -v $(CURDIR)/bundles:/opt/mount --rm mod-live-usb_plugins:latest \
		/home/builder/build-and-copy-bundles.sh $(1) \
		$(subst $(SPACE),$(COMMA),$(filter $(BUNDLES),$($(call BUILDROOT_VAR,$(1))_BUNDLES)))
endef

.stamps/%: docker/.stamp_built
	$(shell mkdir -p $(shell dirname $@))
	$(call BUILD_PLUGIN,$(firstword $(subst /, ,$*)))
	touch $@

docker/.stamp_built: docker/Dockerfile
	./docker/build.sh
	touch $@
