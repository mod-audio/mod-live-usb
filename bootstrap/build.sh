#!/bin/bash

#######################################################################################################################
# exit if any command fails

set -e

cd $(dirname ${0})

#######################################################################################################################
# build full boostrap

docker start mpb-container-x86_64

docker exec -i mpb-container-x86_64 /bin/bash <<EOF
# reset any changes from toolchain image
git checkout .

# clean target
rm -rf ~/mod-workdir/x86_64/target/
rm -f ~/mod-workdir/x86_64/*/.stamp_target_installed

# run bootstrap
./bootstrap.sh x86_64

# done
exit 0
EOF

docker stop mpb-container-x86_64

#######################################################################################################################
# mark as done

touch .stamp_built

#######################################################################################################################
