FROM mpb-toolchain-x86_64
LABEL maintainer="Filipe Coelho <falktx@moddevices.com>"
ENV DEBIAN_FRONTEND noninteractive
ENV USER builder
ENV HOME /home/$USER

ARG PLAT=x86_64

# run bootstrap (contained within docker)
RUN ./bootstrap.sh $PLAT && ./.clean-install.sh $PLAT

# fetch submodules, needed for building plugins
RUN git submodule init && git submodule update

# NOTE you can edit this as needed
ARG MPB_COMMIT_HASH_FOR_PLUGINS=0df8b34546109712dfbba01aeb06344db89d0176

# update to requested commit
RUN git checkout . && git checkout master && git pull && git checkout $MPB_COMMIT_HASH_FOR_PLUGINS && git submodule update

# re-run bootstrap in case dependencies need update
RUN ./bootstrap.sh $PLAT && ./.clean-install.sh $PLAT

# our simple script to build and copy resulting bundles
COPY scripts/build-and-copy-bundles.sh $HOME/build-and-copy-bundles.sh

# CMD
CMD ["bash"]
