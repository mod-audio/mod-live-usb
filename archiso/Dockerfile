FROM archlinux:base
LABEL maintainer="Filipe Coelho <falktx@moddevices.com>"

# fix glibc breakage, great way to start! :(
ENV patched_glibc glibc-linux4-2.33-4-x86_64.pkg.tar.zst
RUN curl -LO https://repo.archlinuxcn.org/x86_64/$patched_glibc && bsdtar -C / -xvf $patched_glibc

# install packages for archiso and live-welcome
RUN pacman -Syu --noconfirm arch-install-scripts dosfstools e2fsprogs libisoburn mtools squashfs-tools && \
    gcc git kparts make pkg-config qt5-webengine && \
    pacman -Scc

# misc setup
RUN locale-gen en_US.UTF-8

# archiso settings
ENV ARCHISO_GIT_URL https://gitlab.archlinux.org/archlinux/archiso.git

# checkout archiso
RUN git clone $ARCHISO_GIT_URL
WORKDIR archiso
RUN git pull && git checkout 1a9710963996e476790d7a351b57b090866c9b71
RUN make install

# CMD
CMD ["bash"]
