FROM mpb-toolchain
LABEL maintainer="Filipe Coelho <falktx@moddevices.com>"
ENV DEBIAN_FRONTEND noninteractive
ENV USER builder
ENV HOME /home/$USER

# NOTE you can edit this as needed
ENV MPB_COMMIT_HASH_FOR_MOD_OS 61853bfa88467011ffbfbc73f08e3040d8451dfd

# update to requested commit
RUN git checkout . && git checkout master && git pull && git checkout $MPB_COMMIT_HASH_FOR_MOD_OS && git submodule update

# patch source for build
COPY buildroot/busybox.config $HOME/busybox.config
COPY buildroot/mod-os-config.patch $HOME/mod-os-config.patch
RUN patch -p1 -i $HOME/mod-os-config.patch

# copy and activate packages
COPY buildroot/packages $HOME/packages
COPY buildroot/activate-packages.sh $HOME/activate-packages.sh
RUN $HOME/activate-packages.sh

# CMD
CMD ["bash"]
