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
# touch ${WORKDIR}/x86_64/target/etc/group
touch ${WORKDIR}/x86_64/target/etc/hostname
# touch ${WORKDIR}/x86_64/target/etc/hosts
touch ${WORKDIR}/x86_64/target/etc/localtime
# touch ${WORKDIR}/x86_64/target/etc/passwd
# touch ${WORKDIR}/x86_64/target/etc/resolv.conf
# touch ${WORKDIR}/x86_64/target/etc/shadow

#######################################################################################################################
# merged usr mode

mkdir -p ${WORKDIR}/x86_64/target/usr/bin
mkdir -p ${WORKDIR}/x86_64/target/usr/lib
mkdir -p ${WORKDIR}/x86_64/target/usr/sbin

ln -s usr/bin  ${WORKDIR}/x86_64/target/bin
ln -s usr/lib  ${WORKDIR}/x86_64/target/lib
ln -s usr/sbin ${WORKDIR}/x86_64/target/sbin

#######################################################################################################################
# create extra dirs for custom mounting points

mkdir -p ${WORKDIR}/x86_64/target/data
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
cp ${WORKDIR}/x86_64/toolchain/x86_64-mod-linux-gnu/sysroot/lib/libmvec.so.1 ${WORKDIR}/x86_64/target/lib/

#######################################################################################################################
# create docker image

docker build -t mpb-mod-os-x86_64 .

#######################################################################################################################
# create docker container

if ! docker ps -a | grep -q mpb-mod-os-container-x86_64; then
    docker create \
        --name mpb-mod-os-container-x86_64 \
        -ti \
        -v ${WORKDIR}:/home/builder/mod-workdir \
        mpb-mod-os-x86_64:latest
fi

#######################################################################################################################
# build mod-os

docker start mpb-mod-os-container-x86_64

docker exec -i mpb-mod-os-container-x86_64 /bin/bash <<EOF
./bootstrap.sh x86_64
EOF

docker stop mpb-mod-os-container-x86_64

#######################################################################################################################
# fetch rootfs.ext2 from image

ln -sf ${WORKDIR}/x86_64/images/rootfs.ext2 rootfs.ext2

#######################################################################################################################
# mark as done

touch .stamp_built

#######################################################################################################################
