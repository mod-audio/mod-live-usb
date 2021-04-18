#!/bin/bash

set -e

cd $(dirname ${0})

# verify soundcard is valid
if [ ! -e /proc/asound/Dummy ]; then
    echo "error: can't find Loopback soundcard"
    exit 1
fi

# get soundcard index
SOUNDCARD=$(readlink /proc/asound/Dummy | awk 'sub("card","")')

# pass soundcard setup into container
echo "# mod-live-usb soundcard setup
SOUNDCARD=${SOUNDCARD}
SAMPLERATE=48000
BUFFERSIZE=256
NPERIODS=2
CAPTUREARGS=
PLAYBACKARGS=-P
EXTRAARGS=-o 2
" > $(pwd)/config/soundcard.sh

# no security, yay?
export SYSTEMD_SECCOMP=0

# optional nspawn options (everything must be valid)
NSPAWN_OPTS=""
if [ -e /dev/snd/pcmC${SOUNDCARD}D0c ]; then
NSPAWN_OPTS+=" --bind=/dev/snd/pcmC${SOUNDCARD}D0c"
fi
if [ -e /dev/snd/pcmC${SOUNDCARD}D0p ]; then
NSPAWN_OPTS+=" --bind=/dev/snd/pcmC${SOUNDCARD}D0p"
fi
if [ -e /mnt/pedalboards ]; then
NSPAWN_OPTS+=" --bind-ro=/mnt/pedalboards"
fi
if [ -e /mnt/plugins ]; then
NSPAWN_OPTS+=" --bind-ro=/mnt/plugins"
fi

# ready!
sudo systemd-nspawn \
--boot \
--read-only \
--capability=all \
--private-users=false \
--resolv-conf=bind-host \
--machine="mod-live-usb" \
--image=$(pwd)/rootfs.ext2 \
--bind=/dev/snd/controlC${SOUNDCARD} \
--bind=/dev/snd/seq \
--bind=/dev/snd/timer \
--bind=$(pwd)/../rwdata/data:/root/data \
--bind=$(pwd)/../rwdata/user-files:/data/user-files \
--bind-ro=/etc/hostname \
--bind-ro=/etc/hosts \
--bind-ro=$(pwd)/config:/mnt/config \
--bind-ro=$(pwd)/overlay-files/etc/group:/etc/group \
--bind-ro=$(pwd)/overlay-files/etc/passwd:/etc/passwd \
--bind-ro=$(pwd)/overlay-files/etc/shadow:/etc/shadow \
--bind-ro=$(pwd)/overlay-files/system:/etc/systemd/system \
--tmpfs=/tmp \
--tmpfs=/var ${NSPAWN_OPTS}

# --tmpfs=/run \
