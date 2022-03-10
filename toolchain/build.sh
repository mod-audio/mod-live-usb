#!/bin/bash

#######################################################################################################################
# exit if any command fails

set -e

cd $(dirname ${0})

#######################################################################################################################
# create docker image

docker build --build-arg=GROUP_ID=$(id -g) --build-arg=USER_ID=$(id -u) -t mpb-toolchain-x86_64 .

#######################################################################################################################
# environment variables

WORKDIR=${WORKDIR:=$(pwd)/mod-workdir}

#######################################################################################################################
# make sure workdir exists before we try to map it

mkdir -p ${WORKDIR}

#######################################################################################################################
# build the toolchain

docker run -v ${WORKDIR}:/home/builder/mod-workdir --rm mpb-toolchain-x86_64:latest ./bootstrap.sh x86_64 toolchain

#######################################################################################################################
# cleanup crosstool-ng files, which can get quite big

rm -f ${WORKDIR}/download/crosstool-ng-1.24.0.tar.bz2
rm -rf ${WORKDIR}/x86_64/build/crosstool-ng-1.24.0
mkdir ${WORKDIR}/x86_64/build/crosstool-ng-1.24.0
touch ${WORKDIR}/x86_64/build/crosstool-ng-1.24.0/.stamp_configured
touch ${WORKDIR}/x86_64/build/crosstool-ng-1.24.0/.stamp_built1
touch ${WORKDIR}/x86_64/build/crosstool-ng-1.24.0/.stamp_built2
touch ${WORKDIR}/x86_64/build/crosstool-ng-1.24.0/configure

#######################################################################################################################
# mark as done

touch .stamp_built

#######################################################################################################################
