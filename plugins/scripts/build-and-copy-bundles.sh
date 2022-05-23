#!/bin/bash

set -e

plat=${1}
buildroot_pkg=${2}
lv2_bundles=${3}

./build ${plat} ${buildroot_pkg}
for b in $(echo ${lv2_bundles} | sed 's/,/ /g'); do
    cp -rv /home/builder/mod-workdir/${plat}/plugins/${b} /opt/mount/
done
