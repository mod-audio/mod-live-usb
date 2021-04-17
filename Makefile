
TARGETS = mod-os/rootfs.tar

all: $(TARGETS)

mod-os/rootfs.tar: mod-os/Dockerfile toolchain/.stamp_built
	./mod-os/build.sh

toolchain/.stamp_built: toolchain/Dockerfile
	./toolchain/build.sh
	touch $@
