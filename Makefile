
TARGETS = archiso/output/mod-live-usb-v5-x86_64.iso

all: $(TARGETS)

# archiso/output/%.iso: archiso/Dockerfile archiso/build.sh archiso/liveusb/packages.x86_64 mod-os/rootfs.ext2
# 	./archiso/build.sh
# 
# mod-os/rootfs.ext2: mod-os/Dockerfile toolchain/.stamp_built
# 	./mod-os/build.sh

toolchain: toolchain/.stamp_built

toolchain/.stamp_built: toolchain/build.sh toolchain/base/Dockerfile toolchain/bootstrap/Dockerfile
	./$<
