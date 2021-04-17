#!/bin/bash

set -e
set -u

PLATFORM="x86_64"
source .common
export DOWNLOAD_PATH=${DOWNLOAD_DIR}
export TOOLCHAIN_PATH=${TOOLCHAIN_DIR}

for p in $(ls "${HOME}/packages" | sort); do
    if [ -e "global-packages/{p}" ]; then
        echo "ERROR: package ${p} already exists as MPB global-package"
        exit 1
    fi
    rm -rf "${BUILD_DIR}/${BUILDROOT_VERSION}/package/${p}"
    rm -rf "${SOURCE_DIR}/plugins-dep/package/${p}"
    cp -r "${HOME}/packages/${p}" "${SOURCE_DIR}/plugins-dep/package/"
    echo "source \"\$BR2_EXTERNAL/package/${p}/Config.in\"" >> "${SOURCE_DIR}/plugins-dep/Config.in"
done
