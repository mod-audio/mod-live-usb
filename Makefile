
TARGETS = archiso/output/mod-live-usb-v5-x86_64.iso

all: $(TARGETS)

# archiso/output/%.iso: archiso/Dockerfile archiso/build.sh archiso/liveusb/packages.x86_64 mod-os/rootfs.ext2
# 	./archiso/build.sh

# step 1: build toolchain

toolchain: toolchain/.stamp_built

toolchain/.stamp_built: toolchain/build.sh %/Dockerfile
	./$<

# step 2: build common bootstrap packages

bootstrap: bootstrap/.stamp_built

bootstrap/.stamp_built: bootstrap/build.sh
	./$<

# step 3: build mod-os image

mod-os: toolchain mod-os/rootfs.ext2

mod-os/rootfs.ext2: mod-os/build.sh mod-os/Dockerfile
	./$<

# common rules
%/.stamp_built: %/build.sh %/Dockerfile
	./$<
