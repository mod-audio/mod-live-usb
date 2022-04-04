FROM archlinux:base
LABEL maintainer="Filipe Coelho <falktx@moddevices.com>"

# force new keyring, required for installing anything
RUN pacman -Sy --noconfirm archlinux-keyring && \
    pacman -Scc && \
    rm -rf /etc/pacman.d/gnupg && \
    pacman-key --init && \
    pacman-key --populate archlinux

# update packages
RUN pacman pacman -Syuu --noconfirm && pacman -Scc

# install packages for archiso and live-welcome
RUN pacman -Syu --noconfirm arch-install-scripts dosfstools e2fsprogs libisoburn mtools squashfs-tools && \
    pacman -Syu --noconfirm gcc git jack2 kparts make patch pkg-config qt5-webengine && \
    pacman -Scc

# misc setup
RUN locale-gen en_US.UTF-8

# archiso settings
ENV ARCHISO_GIT_URL https://gitlab.archlinux.org/archlinux/archiso.git

# checkout archiso
RUN git clone $ARCHISO_GIT_URL
WORKDIR archiso
RUN git pull && git checkout 0b64536292a5947374fbd2b73b5240082f12d26c
COPY archiso-base-mode.patch .
RUN patch -p1 -i archiso-base-mode.patch
RUN make install PREFIX=/usr

# CMD
CMD ["bash"]
