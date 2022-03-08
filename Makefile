
all: iso

# step 1: build toolchain

toolchain: toolchain/.stamp_built

toolchain/.stamp_built: toolchain/build.sh toolchain/Dockerfile
	./$<

# step 2: build common bootstrap packages

bootstrap: bootstrap/.stamp_built

bootstrap/.stamp_built: toolchain bootstrap/build.sh
	./$<

# step 3: build mod-os image

mod-os: mod-os/.stamp_built

mod-os/.stamp_built: bootstrap mod-os/build.sh mod-os/Dockerfile
	./$<

# step 4: build plugins bootstrap

plugins-bootstrap: plugins/.stamp_built

plugins/.stamp_built: toolchain plugins/build.sh plugins/Dockerfile
	./$<

# step 5: build plugins

plugins: plugins-bootstrap
	$(MAKE) -C plugins

# .PHONY: plugins

# step 6: build ISO

iso: archiso/output/mod-live-usb-v6-x86_64.iso

archiso/output/%.iso: mod-os plugins archiso/build.sh archiso/Dockerfile archiso/liveusb/packages.x86_64
	./$<

run:
	./archiso/run_archiso.sh -i ./archiso/output/mod-live-usb-v6-x86_64.iso
