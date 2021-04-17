######################################
#
# mod-midi-merger
#
######################################

MOD_MIDI_MERGER_VERSION = f9ef13ae287b0b284a1799c5b65dbb8e0a3062dd
MOD_MIDI_MERGER_SITE = $(call github,moddevices,mod-midi-merger,$(MOD_MIDI_MERGER_VERSION))
MOD_MIDI_MERGER_DEPENDENCIES = jack2mod

$(eval $(cmake-package))
