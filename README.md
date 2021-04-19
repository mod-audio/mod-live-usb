# mod-live-usb

Work in progress, please come back later.

Super early instructions:

```sh
# build mod-os image (through docker and mod-plugin-builder)
# NOTE: takes a very long time!
make

# build plugins
# NOTE: takes a very long time!
make -C plugins

# run the container with USB souncard (requires root)
./mod-os/start.sh Rubix22
```

There are also releases for Live-CD/USB images.
