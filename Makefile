
all: iso

# step 1: build toolchain

toolchain: toolchain/.stamp_built

toolchain/.stamp_built: toolchain/build.sh toolchain/Dockerfile
	./$<

# step 2: build common bootstrap packages

bootstrap: toolchain bootstrap/.stamp_built

bootstrap/.stamp_built: bootstrap/build.sh
	./$<

# step 3: build mod-os image

mod-os: bootstrap mod-os/rootfs.ext2

mod-os/rootfs.ext2: mod-os/build.sh mod-os/Dockerfile
	./$<

# step 4: build plugins

plugins: plugins-bootstrap
	$(MAKE) -C plugins

plugins-bootstrap: toolchain plugins/.stamp_built

plugins/.stamp_built: plugins/build.sh plugins/Dockerfile
	./$<

# .PHONY: plugins

# step 5: build ISO

iso: mod-os plugins archiso/output/mod-live-usb-v5-x86_64.iso

archiso/output/%.iso: archiso/build.sh archiso/Dockerfile archiso/liveusb/packages.x86_64
	./$<

run:
	./archiso/run_archiso.sh -i ./archiso/output/mod-live-usb-v5-x86_64.iso
