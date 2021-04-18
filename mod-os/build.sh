#!/bin/bash

set -e

cd $(dirname ${0})

docker build -t mod-live-usb_mod-os .
docker run -v ${PWD}:/opt/mount --rm --entrypoint \
  cp mod-live-usb_mod-os:latest /home/builder/rootfs.ext2 /opt/mount/rootfs.ext2
