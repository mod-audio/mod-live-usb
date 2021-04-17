#!/bin/bash

set -e

cd $(dirname $0)

docker build -t mod-live-usb_toolchain .
