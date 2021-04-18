#!/bin/bash

set -e

cd $(dirname ${0})

if [ ! -e workdir/.stamp_built ]; then
    sudo rm -rf workdir
    mkdir workdir
    docker build -t mod-live-usb_iso .
    touch workdir/.stamp_built
fi

CHROOT_DIR=liveusb/airootfs
MOD_LIVE_DIR=${CHROOT_DIR}/root/.mod-live

mkdir -p output workdir
mkdir -p ${CHROOT_DIR}/mnt/pedalboards
mkdir -p ${CHROOT_DIR}/mnt/plugins
mkdir -p ${MOD_LIVE_DIR}

# mount plugin dir (so we dont have to copy the whole thing)
if [ ! -e liveusb/airootfs/mnt/plugins/abGate.lv2 ]; then
    sudo mount --bind ../plugins/bundles liveusb/airootfs/mnt/plugins
fi

# copy files needed for container
cp -r ../mod-os/config ../mod-os/overlay-files ${MOD_LIVE_DIR}/
cp ../mod-os/start.sh ${MOD_LIVE_DIR}/start.sh
cp ../mod-os/rootfs.ext2 ${MOD_LIVE_DIR}/rootfs.ext2

# generate live-welcome binary
docker run \
  -v ${PWD}/../live-welcome:/opt/mount/live-welcome \
  --rm mod-live-usb_iso:latest \
  /usr/bin/make -C /opt/mount/live-welcome
cp ${PWD}/../live-welcome/mod-live-usb-welcome ${MOD_LIVE_DIR}/mod-live-usb-welcome

# delete pre-generated script
rm -f ${MOD_LIVE_DIR}/config/soundcard.sh

# make sure to regen things
rm -f output/*.iso
rm -f workdir/build._*

# let's go!
docker run --privileged \
  -v ${PWD}/liveusb:/opt/mount/liveusb \
  -v ${PWD}/output:/opt/mount/output \
  -v ${PWD}/workdir:/opt/mount/workdir \
  --rm mod-live-usb_iso:latest \
  /usr/bin/mkarchiso -L "Live-USB" -o /opt/mount/output -w /opt/mount/workdir -v /opt/mount/liveusb
