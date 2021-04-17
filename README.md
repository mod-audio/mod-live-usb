# mod-live-usb

Work in progress, please come back later.

Super early instructions:

```sh
# build mod-os image (through docker and mod-plugin builder)
# NOTE: takes a very long time!
make

# extract rootfs
mkdir mod-os/rootfs.mount
tar xf mod-os/rootfs.tar mod-os/rootfs.mount

# setup plugins, TODO
# cp/rsync plugins/* mod-os/rootfs.mount/mnt/plugins/

# run the container with USB souncard
./mod-os/start.sh Rubix22
```
