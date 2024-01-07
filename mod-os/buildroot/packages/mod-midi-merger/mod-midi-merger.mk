######################################
#
# mod-midi-merger
#
######################################

MOD_MIDI_MERGER_VERSION = c10f5bb9151c654afdfdec6fd395539bb03eabe1
MOD_MIDI_MERGER_SITE = $(call github,moddevices,mod-midi-merger,$(MOD_MIDI_MERGER_VERSION))
MOD_MIDI_MERGER_DEPENDENCIES = jack2mod

$(eval $(cmake-package))
