#!/bin/bash

set -e

cd $(dirname ${0})

PLAT=${PLAT:=x86_64}

# regular usage, no systemd yet
if [ -z "${AUDIO_USING_SYSTEMD}" ]; then

    SOUNDCARD=${1}
    SAMPLERATE=${2}
    BUFFERSIZE=${3}
    EXEC=sudo

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

    # if this is systemd, stop now and activate through it
    if [ -n "${USING_SYSTEMD}" ]; then
        exec systemctl start mod-live-audio
        exit 1
    fi

    # not systemd, tell container to bypass security
    export SYSTEMD_SECCOMP=0

# using systemd for audio startup, triggered by ourselves
else

    # setup rwdata persistance if possible
    RWDATA_DEV=$(mount | grep iso9660 | cut -d ' ' -f 1 | cut -c 1-8)
    RWDATA_MNT=$(realpath $(pwd)/../rwdata)
    if [[ "${RWDATA_DEV}" == "/dev/sd"* ]]; then
        if [ ! -e /dev/loop3 ]; then
            losetup -o 2G /dev/loop3 ${RWDATA_DEV}
        fi
        if [ ! -e ${RWDATA_MNT}/lost+found ]; then
            if ! mount /dev/loop3 ${RWDATA_MNT}; then
                mkfs.ext4 /dev/loop3
            fi
            mount /dev/loop3 ${RWDATA_MNT} && true # ignore error
        fi
    fi

    EXEC=exec

fi

# create dedicated shared memory location
rm -rf /dev/shm/live-usb
mkdir /dev/shm/live-usb
chmod 777 /dev/shm/live-usb

# optional nspawn options (everything must be valid)
NSPAWN_OPTS=""

# container shared memory
NSPAWN_OPTS+=" --bind=/dev/shm/live-usb:/dev/shm"

# audio control IPC
if [ -e /dev/shm/ac ]; then
    NSPAWN_OPTS+=" --bind=/dev/shm/ac"
fi

# system messages IPC
if [ -e /dev/shm/sys_msgs ]; then
    NSPAWN_OPTS+=" --bind=/dev/shm/sys_msgs"
fi

# soundcard (capture)
if [ -e /dev/snd/pcmC${SOUNDCARD_ID}D0c ]; then
    NSPAWN_OPTS+=" --bind=/dev/snd/pcmC${SOUNDCARD_ID}D0c"
fi

# soundcard (playback)
if [ -e /dev/snd/pcmC${SOUNDCARD_ID}D0p ]; then
    NSPAWN_OPTS+=" --bind=/dev/snd/pcmC${SOUNDCARD_ID}D0p"
fi

# pedalboards
if [ -e /mnt/pedalboards ]; then
    NSPAWN_OPTS+=" --bind-ro=/mnt/pedalboards"
elif [ -e ../pedalboards/INST_FM_Synth.pedalboard ]; then
    NSPAWN_OPTS+=" --bind=$(realpath $(pwd)/../pedalboards):/mnt/pedalboards"
fi

# plugins
if [ -e /mnt/plugins/${PLAT} ]; then
    NSPAWN_OPTS+=" --bind-ro=/mnt/plugins/${PLAT}:/mnt/plugins"
elif [ -e ../plugins/bundles/${PLAT}/abGate.lv2 ]; then
    NSPAWN_OPTS+=" --bind-ro=$(realpath $(pwd)/../plugins/bundles/${PLAT}):/mnt/plugins"
fi

# mod-os (starting point)
if [ -e /mnt/mod-os/etc/fstab ]; then
    NSPAWN_OPTS+=" --directory=/mnt/mod-os"
else
    NSPAWN_OPTS+=" --image=$(realpath $(pwd)/rootfs.ext2)"
fi

echo "starting up, pwd is $(pwd)"

# ready!
${EXEC} systemd-nspawn \
--boot \
--read-only \
--capability=all \
--private-users=false \
--resolv-conf=bind-host \
--machine="mod-live-audio" \
--bind=/dev/snd/controlC${SOUNDCARD_ID} \
--bind=/dev/snd/seq \
--bind=/dev/snd/timer \
--bind=$(realpath $(pwd)/../rwdata/root):/root \
--bind=$(realpath $(pwd)/../rwdata/user-files):/data/user-files \
--bind-ro=/etc/hostname \
--bind-ro=$(pwd)/config:/mnt/config \
--bind-ro=$(pwd)/overlay-files/system:/etc/systemd/system \
--bind-ro=$(pwd)/overlay-files/tmpfiles.d:/usr/lib/tmpfiles.d \
--bind-ro=$(pwd)/overlay-files/mod-ui.css:/usr/share/mod/html/mod-ui.css \
--bind-ro=$(pwd)/overlay-files/mod-ui.js:/usr/share/mod/html/mod-ui.js \
--tmpfs=/tmp \
--tmpfs=/var ${NSPAWN_OPTS}

# --tmpfs=/run \
