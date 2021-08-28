#!/bin/bash

#######################################################################################################################
# exit if any command fails

set -e

cd $(dirname ${0})

#######################################################################################################################
# environment variables

WORKDIR=${WORKDIR:=$(realpath $(pwd)/../toolchain/mod-workdir)}

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
touch ${WORKDIR}/x86_64/target/etc/group
touch ${WORKDIR}/x86_64/target/etc/hostname
touch ${WORKDIR}/x86_64/target/etc/hosts
touch ${WORKDIR}/x86_64/target/etc/localtime
touch ${WORKDIR}/x86_64/target/etc/passwd
touch ${WORKDIR}/x86_64/target/etc/resolv.conf
touch ${WORKDIR}/x86_64/target/etc/shadow

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

#######################################################################################################################
# cleanup

rm -f rootfs.ext2
rm -f ~/mod-workdir/x86_64/*/.stamp_target_installed
rm -rf ~/mod-workdir/x86_64/target/

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

# WIP
# RUN cp ${WORKDIR}/x86_64/toolchain/x86_64-mod-linux-gnu/sysroot/lib/libmvec.so.1 ${WORKDIR}/x86_64/target/lib/

#######################################################################################################################
# build mod-os

docker start mpb-mod-os-container-x86_64

docker exec -i mpb-mod-os-container-x86_64 /bin/bash <<EOF
./bootstrap.sh x86_64
EOF

docker stop mpb-mod-os-container-x86_64

#######################################################################################################################
# fetch rootfs.ext2 from image

docker run -v ${PWD}:/opt/mount --rm --entrypoint \
    cp mpb-mod-os-container-x86_64:latest /home/builder/mod-workdir/x86_64/images/rootfs.ext2 /opt/mount/rootfs.ext2

#######################################################################################################################
# mark as done

touch .stamp_built

#######################################################################################################################
