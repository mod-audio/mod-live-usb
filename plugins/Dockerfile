FROM mpb-toolchain-x86_64
LABEL maintainer="Filipe Coelho <falktx@moddevices.com>"
ENV DEBIAN_FRONTEND noninteractive
ENV USER builder
ENV HOME /home/$USER

# run bootstrap (contained within docker)
RUN ./bootstrap.sh x86_64 && ./.clean-install.sh x86_64

# fetch submodules, needed for building plugins
RUN git submodule init && git submodule update

# NOTE you can edit this as needed
ARG MPB_COMMIT_HASH_FOR_PLUGINS=3ce0848af280ba38748073b735d23b0921c6b426

# update to requested commit
RUN git checkout master && git pull && git checkout $MPB_COMMIT_HASH_FOR_PLUGINS && git submodule update

# re-run bootstrap in case dependencies need update
RUN ./bootstrap.sh x86_64 && ./.clean-install.sh x86_64

# our simple script to build and copy resulting bundles
COPY scripts/build-and-copy-bundles.sh $HOME/build-and-copy-bundles.sh

# CMD
CMD ["bash"]
