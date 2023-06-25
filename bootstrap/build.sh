#!/bin/bash

#######################################################################################################################
# exit if any command fails

set -e

#######################################################################################################################
# common setup

cd $(dirname ${0})
source ../.common.env

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
# build full boostrap

docker run -v ${WORKDIR}:/home/builder/mod-workdir --rm mpb-toolchain:latest ./bootstrap.sh ${PLAT}

#######################################################################################################################
# mark as done

touch .stamp_built

#######################################################################################################################
