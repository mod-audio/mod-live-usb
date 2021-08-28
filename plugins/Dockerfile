FROM mpb-toolchain-x86_64
LABEL maintainer="Filipe Coelho <falktx@moddevices.com>"
ENV DEBIAN_FRONTEND noninteractive
ENV USER builder
ENV HOME /home/$USER

# run bootstrap (contained within docker)
RUN ./bootstrap.sh x86_64 && ./.clean-install.sh x86_64

# NOTE you can edit this as needed
ARG MPB_COMMIT_HASH_FOR_PLUGINS=9a234bfcc2a3b51620704274dedefc4e8a16a647

# update to requested commit
RUN git checkout master && git pull && git checkout $MPB_COMMIT_HASH_FOR_PLUGINS && git submodule update

# re-run bootstrap in case dependencies need update
RUN ./bootstrap.sh x86_64 && ./.clean-install.sh x86_64

# our simple script to build and copy resulting bundles
COPY scripts/build-and-copy-bundles.sh $HOME/build-and-copy-bundles.sh

# CMD
CMD ["bash"]
