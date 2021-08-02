######################################
#
# mod-midi-merger
#
######################################

MOD_MIDI_MERGER_VERSION = c3b9432af9a1249c1a4ef77904913825cf1a50a5
MOD_MIDI_MERGER_SITE = $(call github,moddevices,mod-midi-merger,$(MOD_MIDI_MERGER_VERSION))
MOD_MIDI_MERGER_DEPENDENCIES = jack2mod

$(eval $(cmake-package))
