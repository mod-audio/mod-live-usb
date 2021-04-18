#!/bin/bash

set -e

cd $(dirname ${0})

docker build -t mod-live-usb_iso .

mkdir -p output workdir
mkdir -p liveusb/airootfs/mnt/mod-os
mkdir -p liveusb/airootfs/mnt/plugins
mkdir -p liveusb/airootfs/root

# mount plugin dir (so we dont have to copy stuff)
if [ ! -e liveusb/airootfs/mnt/plugins/abGate.lv2 ]; then
    sudo mount --bind ../plugins/bundles liveusb/airootfs/mnt/plugins
fi

# extract container rootfs (FIXME non-exec binaries because of this?)
if [ ! -e liveusb/airootfs/mnt/mod-os/bin ]; then
    tar xf ../mod-os/rootfs.tar -C liveusb/airootfs/mnt/mod-os
fi

# copy small files needed for container
cp -r ../mod-os/config ../mod-os/overlay-files liveusb/airootfs/root/
cp ../mod-os/start.sh liveusb/airootfs/root/start.sh

# generate live-welcome binary
if [ ! -e liveusb/airootfs/root/mod-live-usb-welcome ]; then
    docker run \
        -v ${PWD}/../live-welcome:/opt/mount/live-welcome \
        --rm mod-live-usb_iso:latest \
        /usr/bin/make -C /opt/mount/live-welcome
    cp ${PWD}/../live-welcome/mod-live-usb-welcome liveusb/airootfs/root/mod-live-usb-welcome
fi

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

# ... -platform linuxfb -style fusion --no-sandbox
