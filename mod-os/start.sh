#!/bin/bash

set -e

cd $(dirname ${0})

SOUNDCARD=${1}
SAMPLERATE=${2}
BUFFERSIZE=${3}

# verify CLI arguments
if [ -z "${SOUNDCARD}" ]; then
    echo "usage: ${0} <soundcard> [samplerate] [buffersize]"
    exit 1
fi

# using default
if [ ${SOUNDCARD} = "default" ]; then
    SOUNDCARD_ID=0
    SOUNDCARD_HW="default"
# using hw: prefix
elif echo ${SOUNDCARD} | grep -q "hw:"; then
    SOUNDCARD_ID=${SOUNDCARD##*hw:}
    SOUNDCARD_ID=${SOUNDCARD_ID%%,*}
    SOUNDCARD_HW=${SOUNDCARD}
# using card id/name
elif [ -e /proc/asound/${SOUNDCARD} ]; then
    SOUNDCARD_ID=$(readlink /proc/asound/${SOUNDCARD} | awk 'sub("card","")')
    SOUNDCARD_HW="hw:${SOUNDCARD_ID}"
# fallback, assuming to be index
else
    SOUNDCARD_ID="${SOUNDCARD_ID}"
    SOUNDCARD_HW="hw:${SOUNDCARD_ID}"
fi

# verify soundcard is valid
if [ "${SOUNDCARD_HW}" != "default" ] && [ ! -e /proc/asound/card${SOUNDCARD_ID} ]; then
    echo "error: can't find soundcard ${SOUNDCARD} (id: ${SOUNDCARD_ID}, hw: ${SOUNDCARD_HW}"
    exit 1
fi

# fallback soundcard values
if [ -z "${SAMPLERATE}" ]; then
    SAMPLERATE=48000
fi
if [ -z "${BUFFERSIZE}" ]; then
    BUFFERSIZE=128
fi

if [ -e /proc/asound/card${SOUNDCARD_ID}/usbid ]; then
    NPERIODS=3
else
    NPERIODS=2
fi

# pass soundcard setup into container
echo "# mod-live-usb soundcard setup
SOUNDCARD_ID=${SOUNDCARD_ID}
SOUNDCARD_HW=${SOUNDCARD_HW}
SAMPLERATE=${SAMPLERATE}
BUFFERSIZE=${BUFFERSIZE}
NPERIODS=${NPERIODS}
CAPTUREARGS=
PLAYBACKARGS=
EXTRAARGS=
" > $(pwd)/config/soundcard.sh

# no security, yay?
export SYSTEMD_SECCOMP=0

# optional nspawn options (everything must be valid)
NSPAWN_OPTS=""
if [ -e /dev/snd/pcmC${SOUNDCARD_ID}D0c ]; then
NSPAWN_OPTS+=" --bind=/dev/snd/pcmC${SOUNDCARD_ID}D0c"
fi
if [ -e /dev/snd/pcmC${SOUNDCARD_ID}D0p ]; then
NSPAWN_OPTS+=" --bind=/dev/snd/pcmC${SOUNDCARD_ID}D0p"
fi
if [ -e /mnt/pedalboards ]; then
NSPAWN_OPTS+=" --bind-ro=/mnt/pedalboards"
elif [ -e ../pedalboards/INST_FM_Synth.pedalboard ]; then
NSPAWN_OPTS+=" --bind=$(pwd)/../pedalboards:/mnt/pedalboards"
fi
if [ -e /mnt/plugins ]; then
NSPAWN_OPTS+=" --bind-ro=/mnt/plugins"
elif [ -e ../plugins/bundles/abGate.lv2 ]; then
NSPAWN_OPTS+=" --bind-ro=$(pwd)/../plugins/bundles:/mnt/plugins"
fi

echo "starting up, pwd is $(pwd)"

# ready!
sudo systemd-nspawn \
--boot \
--read-only \
--capability=all \
--private-users=false \
--resolv-conf=bind-host \
--machine="mod-live-usb" \
--image=$(pwd)/rootfs.ext2 \
--bind=/dev/snd/controlC${SOUNDCARD_ID} \
--bind=/dev/snd/seq \
--bind=/dev/snd/timer \
--bind=$(pwd)/../rwdata/root:/root \
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
