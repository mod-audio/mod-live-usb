#!/bin/bash

set -e

cd $(dirname ${0})

SOUNDCARD=${1}

# verify CLI arguments
if [ -z "${SOUNDCARD}" ]; then
    echo "usage: ${0} <soundcard>"
    exit 1
fi

# allow soundcard id instead of index
if [ -e /proc/asound/${SOUNDCARD} ]; then
    SOUNDCARD=$(readlink /proc/asound/${SOUNDCARD} | awk 'sub("card","")')
fi

# verify soundcard is valid
if [ ! -e /proc/asound/card${SOUNDCARD} ]; then
    echo "error: can't find soundcard ${SOUNDCARD}"
    exit 1
fi

# if [ ! -e /proc/asound/card${SOUNDCARD}/stream0 ]; then
#     echo "error: soundcard ${SOUNDCARD} has no streams"
#     exit 1
# fi

# find some info for soundcard
# SAMPLERATES=",$(cat /proc/asound/card${SOUNDCARD}/stream0 | awk 'sub("Rates:","")' | head -n 1 | tr -d "[:space:]"),"

# if echo ${SAMPLERATES} | grep -e ",48000,"; then
    SAMPLERATE=48000
# elif echo ${SAMPLERATES} | grep -e ",44100,"; then
#     SAMPLERATE=44100
# elif echo ${SAMPLERATES} | grep -e ",96000,"; then
#     SAMPLERATE=96000
# else
#     SAMPLERATE=$(echo ${SAMPLERATES} | sed 's|,|\n|g' | head -n 2 | tail -n 1)
# fi

if [ -e /proc/asound/card${SOUNDCARD}/usbid ]; then
    NPERIODS=3
else
    NPERIODS=2
fi

# pass soundcard setup into container
echo "# mod-live-usb soundcard setup
SOUNDCARD=${SOUNDCARD}
SAMPLERATE=${SAMPLERATE}
BUFFERSIZE=128
NPERIODS=${NPERIODS}
CAPTUREARGS=
PLAYBACKARGS=
EXTRAARGS=
" > $(pwd)/config/soundcard.sh

# no security, yay?
export SYSTEMD_SECCOMP=0

# ready!
sudo systemd-nspawn \
--boot \
--capability=all \
--private-users=false \
--resolv-conf=bind-host \
--machine="mod-live-usb" \
--directory=$(pwd)/rootfs.mount \
--bind=/dev/snd/controlC${SOUNDCARD} \
--bind=/dev/snd/pcmC${SOUNDCARD}D0c \
--bind=/dev/snd/pcmC${SOUNDCARD}D0p \
--bind=/dev/snd/seq \
--bind=/dev/snd/timer \
--bind-ro=/etc/hostname \
--bind-ro=/etc/hosts \
--bind-ro=$(pwd)/config:/mnt/config \
--bind-ro=$(pwd)/overlay-files/etc/group:/etc/group \
--bind-ro=$(pwd)/overlay-files/etc/passwd:/etc/passwd \
--bind-ro=$(pwd)/overlay-files/etc/shadow:/etc/shadow \
--bind-ro=$(pwd)/overlay-files/system:/etc/systemd/system \
--tmpfs=/run \
--tmpfs=/tmp \
--tmpfs=/var
