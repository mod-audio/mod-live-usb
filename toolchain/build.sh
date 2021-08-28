#!/bin/bash

#######################################################################################################################
# exit if any command fails

set -e

cd $(dirname ${0})

#######################################################################################################################
# environment variables

WORKDIR=${WORKDIR:=$(pwd)/mod-workdir}

#######################################################################################################################
# setup directories

mkdir -p ${WORKDIR}/x86_64/build
mkdir -p ${WORKDIR}/x86_64/target

if [ -n "${GITHUB_ACTIONS}" ]; then
    chmod 777 ${WORKDIR}
    chmod 777 ${WORKDIR}/x86_64
    chmod 777 ${WORKDIR}/x86_64/build
    chmod 777 ${WORKDIR}/x86_64/target

    # TESTING
    chown -R 1000:1000 ${WORKDIR}/x86_64/build
fi

#######################################################################################################################
# create docker image

docker build -t mpb-toolchain-x86_64 .

#######################################################################################################################
# create docker container

if ! docker ps -a | grep -q mpb-container-x86_64; then
    docker create \
        --name mpb-container-x86_64 \
        -ti \
        -v ${WORKDIR}:/home/builder/mod-workdir \
        mpb-toolchain-x86_64:latest
fi

#######################################################################################################################
# build the toolchain

docker start mpb-container-x86_64

docker exec -i mpb-container-x86_64 /bin/bash <<EOF
./bootstrap.sh x86_64 toolchain
EOF

docker stop mpb-container-x86_64

#######################################################################################################################
# cleanup

if [ -n "${GITHUB_ACTIONS}" ]; then
    GITHUB_ACTIONS_HELPER=sudo
fi

${GITHUB_ACTIONS_HELPER} rm -f ${WORKDIR}/download/crosstool-ng-1.24.0.tar.bz2
${GITHUB_ACTIONS_HELPER} rm -rf ${WORKDIR}/x86_64/build/crosstool-ng-1.24.0
mkdir ${WORKDIR}/x86_64/build/crosstool-ng-1.24.0
touch ${WORKDIR}/x86_64/build/crosstool-ng-1.24.0/.stamp_configured
touch ${WORKDIR}/x86_64/build/crosstool-ng-1.24.0/.stamp_built1
touch ${WORKDIR}/x86_64/build/crosstool-ng-1.24.0/.stamp_built2
touch ${WORKDIR}/x86_64/build/crosstool-ng-1.24.0/configure

#######################################################################################################################
# mark as done

touch .stamp_built

#######################################################################################################################
