#!/bin/bash

set -e

cd $(dirname ${0})

# verify soundcard is valid
if [ ! -e /proc/asound/Dummy ]; then
    echo "error: can't find Dummy soundcard"
    exit 1
fi
# sudo modprobe snd-dummy hrtimer=1 pcm_devs=1 pcm_substreams=1

exec bash ./start.sh Dummy
