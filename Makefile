
TARGETS = archiso/output/mod-live-usb-v2-x86_64.iso

all: $(TARGETS)

archiso/output/%.iso: archiso/Dockerfile archiso/build.sh archiso/liveusb/packages.x86_64 mod-os/rootfs.ext2
	./archiso/build.sh

mod-os/rootfs.ext2: mod-os/Dockerfile toolchain/.stamp_built
	./mod-os/build.sh

toolchain/.stamp_built: toolchain/Dockerfile
	./toolchain/build.sh
	touch $@
