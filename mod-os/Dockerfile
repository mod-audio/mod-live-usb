FROM mod-live-usb_toolchain
LABEL maintainer="Filipe Coelho <falktx@moddevices.com>"
ENV DEBIAN_FRONTEND noninteractive
ENV USER builder
ENV HOME /home/$USER

# NOTE you can edit this as needed
ENV MPB_COMMIT_HASH_FOR_MOD_OS 8048f2dcf416a774e64a98ffead8f75cb7bc861f

# checkout mod-plugin-builder
WORKDIR $HOME/mod-plugin-builder

# update to requested commit
RUN git checkout master && git pull && git checkout $MPB_COMMIT_HASH_FOR_MOD_OS && git submodule update

# patch source for build
COPY buildroot/01_mod-os-config.patch $HOME/01_mod-os-config.patch
COPY buildroot/busybox.config $HOME/busybox.config
RUN patch -p1 -i $HOME/01_mod-os-config.patch

# create dummy files and dirs for rootfs
RUN touch $HOME/mod-workdir/x86_64/target/etc/group && \
    touch $HOME/mod-workdir/x86_64/target/etc/hostname && \
    touch $HOME/mod-workdir/x86_64/target/etc/hosts && \
    touch $HOME/mod-workdir/x86_64/target/etc/localtime && \
    touch $HOME/mod-workdir/x86_64/target/etc/passwd && \
    touch $HOME/mod-workdir/x86_64/target/etc/resolv.conf && \
    touch $HOME/mod-workdir/x86_64/target/etc/shadow && \
    mkdir $HOME/mod-workdir/x86_64/target/dev && \
    mkdir $HOME/mod-workdir/x86_64/target/mnt && \
    mkdir $HOME/mod-workdir/x86_64/target/proc && \
    mkdir $HOME/mod-workdir/x86_64/target/root && \
    mkdir $HOME/mod-workdir/x86_64/target/run && \
    mkdir $HOME/mod-workdir/x86_64/target/sys && \
    mkdir $HOME/mod-workdir/x86_64/target/tmp

# run bootstrap, for the new dependencies and patched config
RUN ./bootstrap.sh x86_64 && ./.clean-install.sh x86_64

# copy and activate packages
COPY buildroot/packages $HOME/packages
COPY buildroot/packages.txt $HOME/packages.txt
COPY buildroot/activate-packages.sh $HOME/activate-packages.sh
RUN $HOME/activate-packages.sh

# create extra dirs for custom mounting points
RUN mkdir $HOME/mod-workdir/x86_64/target/data && \
    mkdir $HOME/mod-workdir/x86_64/target/data/user-files && \
    mkdir $HOME/mod-workdir/x86_64/target/mnt/config && \
    mkdir $HOME/mod-workdir/x86_64/target/mnt/lv2 && \
    mkdir $HOME/mod-workdir/x86_64/target/mnt/pedalboards && \
    mkdir $HOME/mod-workdir/x86_64/target/mnt/plugins

# generate mod-os content
RUN ./bootstrap.sh x86_64 && ./.clean-install.sh x86_64

# WIP
RUN cp $HOME/mod-workdir/x86_64/toolchain/x86_64-mod-linux-gnu/sysroot/lib/libmvec.so.1 $HOME/mod-workdir/x86_64/target/lib/

# final bootstrap, generates tar image
COPY buildroot/02_tar-image.patch $HOME/02_tar-image.patch
RUN patch -p1 -i $HOME/02_tar-image.patch
RUN ./bootstrap.sh x86_64 && mv $HOME/mod-workdir/x86_64/images/rootfs.tar $HOME/ && ./.clean-install.sh x86_64

# CMD
CMD ["bash"]
