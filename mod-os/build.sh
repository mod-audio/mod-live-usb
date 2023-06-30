#!/bin/bash

#######################################################################################################################
# exit if any command fails

set -e

#######################################################################################################################
# common setup

cd $(dirname ${0})
source ../.common.env

#######################################################################################################################
# cleanup

rm -f rootfs.ext2 ${WORKDIR}/${PLAT}/images/rootfs.ext2

if [ -z "${NOCLEAN}" ]; then
    rm -f ${WORKDIR}/${PLAT}/build/*/.stamp_target_installed
    rm -rf ${WORKDIR}/${PLAT}/target/

    # FIXME this seems needed, not sure why yet
    rm -rf ${WORKDIR}/${PLAT}/build/lv2-*
    rm -rf ${WORKDIR}/${PLAT}/build/serd-*
    rm -rf ${WORKDIR}/${PLAT}/build/sord-*
    rm -rf ${WORKDIR}/${PLAT}/build/sratom-*
    rm -rf ${WORKDIR}/${PLAT}/build/lilv-*
fi

#######################################################################################################################
# create dummy files and dirs for rootfs

mkdir -p ${WORKDIR}/${PLAT}/target/etc
mkdir -p ${WORKDIR}/${PLAT}/target/dev
mkdir -p ${WORKDIR}/${PLAT}/target/home
mkdir -p ${WORKDIR}/${PLAT}/target/mnt
mkdir -p ${WORKDIR}/${PLAT}/target/proc
mkdir -p ${WORKDIR}/${PLAT}/target/root
mkdir -p ${WORKDIR}/${PLAT}/target/run
mkdir -p ${WORKDIR}/${PLAT}/target/srv
mkdir -p ${WORKDIR}/${PLAT}/target/sys
mkdir -p ${WORKDIR}/${PLAT}/target/tmp
touch ${WORKDIR}/${PLAT}/target/etc/localtime
cp overlay-files/etc/group    ${WORKDIR}/${PLAT}/target/etc/group
cp overlay-files/etc/hostname ${WORKDIR}/${PLAT}/target/etc/hostname
cp overlay-files/etc/hosts    ${WORKDIR}/${PLAT}/target/etc/hosts
cp overlay-files/etc/passwd   ${WORKDIR}/${PLAT}/target/etc/passwd
cp overlay-files/etc/shadow   ${WORKDIR}/${PLAT}/target/etc/shadow

#######################################################################################################################
# merged usr mode

mkdir -p ${WORKDIR}/${PLAT}/target/usr/bin
mkdir -p ${WORKDIR}/${PLAT}/target/usr/lib
mkdir -p ${WORKDIR}/${PLAT}/target/usr/sbin

if [ ! -e ${WORKDIR}/${PLAT}/target/bin ]; then
    ln -s usr/bin ${WORKDIR}/${PLAT}/target/bin
fi
if [ ! -e ${WORKDIR}/${PLAT}/target/lib ]; then
    ln -s usr/lib ${WORKDIR}/${PLAT}/target/lib
fi
if [ ! -e ${WORKDIR}/${PLAT}/target/sbin ]; then
    ln -s usr/sbin ${WORKDIR}/${PLAT}/target/sbin
fi

#######################################################################################################################
# create extra dirs for custom mounting points

mkdir -p ${WORKDIR}/${PLAT}/target/data/user-files
mkdir -p ${WORKDIR}/${PLAT}/target/mnt/config
mkdir -p ${WORKDIR}/${PLAT}/target/mnt/lv2
mkdir -p ${WORKDIR}/${PLAT}/target/mnt/pedalboards
mkdir -p ${WORKDIR}/${PLAT}/target/mnt/plugins
mkdir -p ${WORKDIR}/${PLAT}/target/root/data
mkdir -p ${WORKDIR}/${PLAT}/target/usr/share/mod/html
touch ${WORKDIR}/${PLAT}/target/usr/share/mod/html/mod-ui.css
touch ${WORKDIR}/${PLAT}/target/usr/share/mod/html/mod-ui.js

# this is needed somehow
if [ "${PLAT}" = "x86_64" ] && [ ! -e ${WORKDIR}/x86_64/target/usr/lib/libmvec.so.1 ]; then
    cp ${WORKDIR}/x86_64/toolchain/x86_64-modaudio-linux-gnu/sysroot/lib/libmvec.so.1 \
       ${WORKDIR}/x86_64/target/usr/lib/libmvec.so.1
fi

#######################################################################################################################
# create docker image

docker build -t mpb-mod-os .

#######################################################################################################################
# build mod-os

docker run -v ${WORKDIR}:/home/builder/mod-workdir --rm mpb-mod-os:latest ./bootstrap.sh ${PLAT}

#######################################################################################################################
# fetch rootfs.ext2 from image

ln -sf ${WORKDIR}/${PLAT}/images/rootfs.ext2 rootfs.ext2

#######################################################################################################################
# mark as done

touch .stamp_built

#######################################################################################################################
