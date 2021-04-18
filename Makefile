
TARGETS = mod-os/rootfs.ext2

all: $(TARGETS)

archiso/output/mod-live-usb.iso: archiso/Dockerfile mod-os/rootfs.ext2
	./archiso/build.sh

mod-os/rootfs.ext2: mod-os/Dockerfile toolchain/.stamp_built
	./mod-os/build.sh

toolchain/.stamp_built: toolchain/Dockerfile
	./toolchain/build.sh
	touch $@
