#!/bin/bash

#######################################################################################################################
# exit if any command fails

set -e

cd $(dirname ${0})

#######################################################################################################################
# environment variables

WORKDIR=${WORKDIR:=$(realpath $(pwd)/../toolchain/mod-workdir)}

#######################################################################################################################
# cleanup

rm -f rootfs.ext2 ${WORKDIR}/x86_64/images/rootfs.ext2

if [ -z "${NOCLEAN}" ]; then
    rm -f ${WORKDIR}/x86_64/build/*/.stamp_target_installed
    rm -rf ${WORKDIR}/x86_64/target/

    # FIXME this seems needed, not sure why yet
    rm -rf ${WORKDIR}/x86_64/build/lv2-*
    rm -rf ${WORKDIR}/x86_64/build/serd-*
    rm -rf ${WORKDIR}/x86_64/build/sord-*
    rm -rf ${WORKDIR}/x86_64/build/sratom-*
    rm -rf ${WORKDIR}/x86_64/build/lilv-*
fi

#######################################################################################################################
# create dummy files and dirs for rootfs

mkdir -p ${WORKDIR}/x86_64/target/etc
mkdir -p ${WORKDIR}/x86_64/target/dev
mkdir -p ${WORKDIR}/x86_64/target/home
mkdir -p ${WORKDIR}/x86_64/target/mnt
mkdir -p ${WORKDIR}/x86_64/target/proc
mkdir -p ${WORKDIR}/x86_64/target/root
mkdir -p ${WORKDIR}/x86_64/target/run
mkdir -p ${WORKDIR}/x86_64/target/srv
mkdir -p ${WORKDIR}/x86_64/target/sys
mkdir -p ${WORKDIR}/x86_64/target/tmp
touch ${WORKDIR}/x86_64/target/etc/hostname
touch ${WORKDIR}/x86_64/target/etc/localtime
cp overlay-files/etc/group  ${WORKDIR}/x86_64/target/etc/group
cp overlay-files/etc/passwd ${WORKDIR}/x86_64/target/etc/passwd
cp overlay-files/etc/shadow ${WORKDIR}/x86_64/target/etc/shadow

#######################################################################################################################
# merged usr mode

mkdir -p ${WORKDIR}/x86_64/target/usr/bin
mkdir -p ${WORKDIR}/x86_64/target/usr/lib
mkdir -p ${WORKDIR}/x86_64/target/usr/sbin

if [ ! -e ${WORKDIR}/x86_64/target/bin ]; then
    ln -s usr/bin ${WORKDIR}/x86_64/target/bin
fi
if [ ! -e ${WORKDIR}/x86_64/target/lib ]; then
    ln -s usr/lib ${WORKDIR}/x86_64/target/lib
fi
if [ ! -e ${WORKDIR}/x86_64/target/sbin ]; then
    ln -s usr/sbin ${WORKDIR}/x86_64/target/sbin
fi

#######################################################################################################################
# create extra dirs for custom mounting points

mkdir -p ${WORKDIR}/x86_64/target/data/user-files
mkdir -p ${WORKDIR}/x86_64/target/mnt/config
mkdir -p ${WORKDIR}/x86_64/target/mnt/lv2
mkdir -p ${WORKDIR}/x86_64/target/mnt/pedalboards
mkdir -p ${WORKDIR}/x86_64/target/mnt/plugins
mkdir -p ${WORKDIR}/x86_64/target/root/data
mkdir -p ${WORKDIR}/x86_64/target/usr/share/mod/html
touch ${WORKDIR}/x86_64/target/usr/share/mod/html/mod-ui.css
touch ${WORKDIR}/x86_64/target/usr/share/mod/html/mod-ui.js

# this is needed somehow
if [ ! -e ${WORKDIR}/x86_64/target/usr/lib/libmvec.so.1 ]; then
    cp ${WORKDIR}/x86_64/toolchain/x86_64-mod-linux-gnu/sysroot/lib/libmvec.so.1 \
       ${WORKDIR}/x86_64/target/usr/lib/libmvec.so.1
fi

#######################################################################################################################
# create docker image

docker build -t mpb-mod-os-x86_64 .

#######################################################################################################################
# build mod-os

docker run -v ${WORKDIR}:/home/builder/mod-workdir --rm mpb-mod-os-x86_64:latest ./bootstrap.sh x86_64

#######################################################################################################################
# fetch rootfs.ext2 from image

ln -sf ${WORKDIR}/x86_64/images/rootfs.ext2 rootfs.ext2

#######################################################################################################################
# mark as done

touch .stamp_built

#######################################################################################################################
