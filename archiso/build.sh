#!/bin/bash

#######################################################################################################################
# exit if any command fails

set -e

cd $(dirname ${0})

#######################################################################################################################
# create docker image

if [ ! -e workdir/.stamp_built ]; then
    sudo rm -rf workdir
    mkdir workdir
    docker build -t mod-live-usb_iso .
    touch workdir/.stamp_built
fi

#######################################################################################################################
# setup directories

CHROOT_DIR=${PWD}/liveusb/airootfs
MOD_LIVE_DIR=${CHROOT_DIR}/root/.mod-live

mkdir -p cache workdir
mkdir -p ${CHROOT_DIR}/mnt/mod-os
mkdir -p ${CHROOT_DIR}/mnt/pedalboards
mkdir -p ${CHROOT_DIR}/mnt/plugins
mkdir -p ${CHROOT_DIR}/root/rwdata/root
mkdir -p ${CHROOT_DIR}/root/rwdata/user-files
mkdir -p ${MOD_LIVE_DIR}

#######################################################################################################################
# mount pedalboards and plugins dir (so we dont have to copy the whole thing)

if [ ! -e ${CHROOT_DIR}/mnt/pedalboards/INST_FM_Synth.pedalboard ]; then
    sudo mount --bind ../pedalboards ${CHROOT_DIR}/mnt/pedalboards
fi

if [ ! -e ${CHROOT_DIR}/mnt/plugins/abGate.lv2 ]; then
    sudo mount --bind ../plugins/bundles ${CHROOT_DIR}/mnt/plugins
fi

#######################################################################################################################
# copy files needed for container

cp -r ../documentation ../mod-os/config ../mod-os/overlay-files ${MOD_LIVE_DIR}/
cp ../mod-os/start.sh ${MOD_LIVE_DIR}/start.sh
cp ../mod-os/rootfs.ext2 ${MOD_LIVE_DIR}/rootfs.ext2

#######################################################################################################################
# delete pre-generated script

rm -f ${MOD_LIVE_DIR}/config/soundcard.sh

#######################################################################################################################
# generate live-welcome binary

make clean -C ${PWD}/../live-welcome
docker run \
  -v ${PWD}/../live-welcome:/opt/mount/live-welcome \
  --rm mod-live-usb_iso:latest \
  /usr/bin/make -C /opt/mount/live-welcome
cp ${PWD}/../live-welcome/mod-live-usb-welcome     ${MOD_LIVE_DIR}/mod-live-usb-welcome
cp ${PWD}/../live-welcome/mod-live-usb-welcome.run ${MOD_LIVE_DIR}/mod-live-usb-welcome.run
cp ${PWD}/../mod-os/buildroot/jack2-live-usb/libjack.so ${MOD_LIVE_DIR}/libjack.so

# make sure to regen things
rm -f output/*.iso
# sudo rm -rf workdir

# bootstrap
rm -f workdir/base._*
docker run --privileged \
  -v ${PWD}/cache:/var/cache/pacman/pkg \
  -v ${PWD}/liveusb:/opt/mount/liveusb \
  -v ${PWD}/output:/opt/mount/output \
  -v ${PWD}/workdir:/opt/mount/workdir \
  --rm mod-live-usb_iso:latest \
  /usr/bin/mkarchiso -v -m base -o /opt/mount/output -w /opt/mount/workdir /opt/mount/liveusb

# clean unwanted files
sudo rm -rf ${PWD}/workdir/x86_64/airootfs/usr/include
sudo rm -rf ${PWD}/workdir/x86_64/airootfs/usr/share/doc
sudo rm -rf ${PWD}/workdir/x86_64/airootfs/usr/share/gtk-doc
sudo rm -rf ${PWD}/workdir/x86_64/airootfs/usr/share/info
sudo rm -rf ${PWD}/workdir/x86_64/airootfs/usr/share/man
sudo rm -rf ${PWD}/workdir/x86_64/airootfs/usr/share/sounds
sudo rm -rf ${PWD}/workdir/x86_64/airootfs/usr/share/wallpapers

mkdir -p tmp
sudo mv ${PWD}/workdir/x86_64/airootfs/usr/share/locale/en* tmp/
sudo rm -rf ${PWD}/workdir/x86_64/airootfs/usr/share/locale/*
sudo mv tmp/* ${PWD}/workdir/x86_64/airootfs/usr/share/locale/

# generate iso
rm -f workdir/base._*
rm -f workdir/build._*
rm -f workdir/iso._*
docker run --privileged \
  -v ${PWD}/cache:/var/cache/pacman/pkg \
  -v ${PWD}/liveusb:/opt/mount/liveusb \
  -v ${PWD}/output:/opt/mount/output \
  -v ${PWD}/workdir:/opt/mount/workdir \
  --rm mod-live-usb_iso:latest \
  /usr/bin/mkarchiso -v -m iso -o /opt/mount/output -w /opt/mount/workdir /opt/mount/liveusb
