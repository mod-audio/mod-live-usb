#!/bin/bash

set -e

buildroot_pkg=${1}
lv2_bundles=${2}

./build x86_64 ${1}
for b in $(echo ${lv2_bundles} | sed 's/,/ /g'); do
    cp -rv /home/builder/mod-workdir/x86_64/plugins/${b} /opt/mount/
done
