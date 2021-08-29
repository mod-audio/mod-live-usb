#!/bin/bash

#######################################################################################################################
# exit if any command fails

set -e

cd $(dirname ${0})

#######################################################################################################################
# environment variables

WORKDIR=${WORKDIR:=$(pwd)/mod-workdir}

#######################################################################################################################
# setup directories for CI

mkdir -p ${WORKDIR}

if [ -n "${GITHUB_ACTIONS}" ]; then
    sudo chown -R 1000:1000 ${WORKDIR}
fi

#######################################################################################################################
# create docker image

docker build -t mpb-toolchain-x86_64 .

#######################################################################################################################
# build the toolchain

docker run -v ${WORKDIR}:/home/builder/mod-workdir --rm mpb-toolchain-x86_64:latest ./bootstrap.sh x86_64 toolchain

#######################################################################################################################
# cleanup for CI

if [ -n "${GITHUB_ACTIONS}" ]; then
    sudo chown -R runner ${WORKDIR}
fi

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
