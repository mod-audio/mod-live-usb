#!/bin/bash

#######################################################################################################################
# exit if any command fails

set -e

cd $(dirname ${0})

#######################################################################################################################
# environment variables

WORKDIR=${WORKDIR:=$(realpath $(pwd)/../toolchain/mod-workdir)}

#######################################################################################################################
# fetch rootfs.ext2 image

if [ ! -e rootfs.ext2 ]; then
    ln -s ${WORKDIR}/x86_64/images/rootfs.ext2 rootfs.ext2
fi

#######################################################################################################################
# fetch plugins

mkdir -p plugins
rsync --delete --links --recursive --update ../plugins/bundles/ plugins/

#######################################################################################################################
# mount rootfs

mkdir -p rootfs

if [ ! -e rootfs/linuxrc ]; then
    sudo mount rootfs.ext2 rootfs
    sudo chmod 755 rootfs/lost+found
    sudo chmod 644 rootfs/usr/lib/core
fi

#######################################################################################################################
# create docker image

docker build -t mpb-offline-render .

#######################################################################################################################
