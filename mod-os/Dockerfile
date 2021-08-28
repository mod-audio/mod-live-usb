FROM mpb-toolchain-x86_64
LABEL maintainer="Filipe Coelho <falktx@moddevices.com>"
ENV DEBIAN_FRONTEND noninteractive
ENV USER builder
ENV HOME /home/$USER

# NOTE you can edit this as needed
ENV MPB_COMMIT_HASH_FOR_MOD_OS 581c17fd0173b2e22b4297911dbe54ed1c058574

# update to requested commit
RUN git checkout . && git checkout master && git pull && git checkout $MPB_COMMIT_HASH_FOR_MOD_OS && git submodule update

# create dummy files and dirs for rootfs
RUN touch $HOME/mod-workdir/x86_64/target/etc/group && \
    touch $HOME/mod-workdir/x86_64/target/etc/hostname && \
    touch $HOME/mod-workdir/x86_64/target/etc/hosts && \
    touch $HOME/mod-workdir/x86_64/target/etc/localtime && \
    touch $HOME/mod-workdir/x86_64/target/etc/passwd && \
    touch $HOME/mod-workdir/x86_64/target/etc/resolv.conf && \
    touch $HOME/mod-workdir/x86_64/target/etc/shadow && \
    mkdir $HOME/mod-workdir/x86_64/target/dev && \
    mkdir $HOME/mod-workdir/x86_64/target/home && \
    mkdir $HOME/mod-workdir/x86_64/target/mnt && \
    mkdir $HOME/mod-workdir/x86_64/target/proc && \
    mkdir $HOME/mod-workdir/x86_64/target/root && \
    mkdir $HOME/mod-workdir/x86_64/target/run && \
    mkdir $HOME/mod-workdir/x86_64/target/srv && \
    mkdir $HOME/mod-workdir/x86_64/target/sys && \
    mkdir $HOME/mod-workdir/x86_64/target/tmp

# create extra dirs for custom mounting points
RUN mkdir $HOME/mod-workdir/x86_64/target/data && \
    mkdir $HOME/mod-workdir/x86_64/target/data/user-files && \
    mkdir $HOME/mod-workdir/x86_64/target/mnt/config && \
    mkdir $HOME/mod-workdir/x86_64/target/mnt/lv2 && \
    mkdir $HOME/mod-workdir/x86_64/target/mnt/pedalboards && \
    mkdir $HOME/mod-workdir/x86_64/target/mnt/plugins && \
    mkdir $HOME/mod-workdir/x86_64/target/root/data && \
    mkdir -p $HOME/mod-workdir/x86_64/target/usr/share/mod/html && \
    touch $HOME/mod-workdir/x86_64/target/usr/share/mod/html/mod-ui.css && \
    touch $HOME/mod-workdir/x86_64/target/usr/share/mod/html/mod-ui.js

# patch source for build
COPY buildroot/01_mod-os-config.patch $HOME/01_mod-os-config.patch
COPY buildroot/busybox.config $HOME/busybox.config
RUN patch -p1 -i $HOME/01_mod-os-config.patch

# copy and activate packages
COPY buildroot/packages $HOME/packages
COPY buildroot/activate-packages.sh $HOME/activate-packages.sh
RUN $HOME/activate-packages.sh

# clean target
RUN rm -rf $HOME/mod-workdir/x86_64/target/
RUN rm -f $HOME/mod-workdir/x86_64/*/.stamp_target_installed

# generate mod-os content
RUN ./bootstrap.sh x86_64

# WIP
RUN cp $HOME/mod-workdir/x86_64/toolchain/x86_64-mod-linux-gnu/sysroot/lib/libmvec.so.1 $HOME/mod-workdir/x86_64/target/lib/

# final bootstrap, generates ext2 image
COPY buildroot/02_ext2-image.patch $HOME/02_ext2-image.patch
RUN patch -p1 -i $HOME/02_ext2-image.patch
RUN ./bootstrap.sh x86_64

# CMD
CMD ["bash"]
