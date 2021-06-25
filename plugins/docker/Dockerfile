FROM mod-live-usb_toolchain
LABEL maintainer="Filipe Coelho <falktx@moddevices.com>"
ENV DEBIAN_FRONTEND noninteractive
ENV USER builder
ENV HOME /home/$USER

# checkout mod-plugin-builder
WORKDIR $HOME/mod-plugin-builder
RUN git checkout . && git checkout master && git pull && git submodule init && git submodule update

# NOTE you can edit this as needed
ENV MPB_COMMIT_HASH_FOR_PLUGINS 98538d9d82038f745fa37a48aa2e231bdc52f43a

# update to requested commit
RUN git checkout master && git pull && git checkout $MPB_COMMIT_HASH_FOR_PLUGINS && git submodule update

# re-run bootstrap in case dependencies need update
RUN ./bootstrap.sh x86_64 && ./.clean-install.sh x86_64

# our simple to build and copy resulting bundles
COPY scripts/build-and-copy-bundles.sh $HOME/build-and-copy-bundles.sh

# CMD
CMD ["bash"]
