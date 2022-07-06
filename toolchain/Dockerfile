FROM ubuntu:20.04
LABEL maintainer="Filipe Coelho <falktx@moddevices.com>"
ENV DEBIAN_FRONTEND noninteractive

# update and upgrade system
RUN apt-get update && apt-get upgrade -qy && apt-get clean

# install packages for buildroot
RUN apt-get install -qy locales acl bash bash-completion bc curl cvs git mercurial rsync subversion sudo wget dosfstools && \
    apt-get install -qy bison bundler bzip2 cpio flex gawk gperf gzip help2man nano perl patch python tar texinfo unzip python3-dev python3-setuptools && \
    apt-get install -qy automake binutils build-essential device-tree-compiler premake4 sunxi-tools libtool-bin ncurses-dev jq && \
    apt-get clean
RUN wget http://archive.ubuntu.com/ubuntu/pool/universe/p/premake/premake_3.7-1_amd64.deb && \
    dpkg -i premake_3.7-1_amd64.deb && \
    rm premake_3.7-1_amd64.deb

RUN locale-gen en_US.UTF-8
RUN echo "source /etc/bash_completion" >> $HOME/.bashrc

# user configurations
ENV USER builder
ENV HOME /home/$USER

# create user
ARG GROUP_ID=1001
ARG USER_ID=1001
RUN groupadd -g $GROUP_ID -o $USER
RUN useradd -d $HOME -m -G sudo -u $USER_ID -g $GROUP_ID -o $USER
# set user to sudoers (no password required)
RUN echo "$USER ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/99-$USER && chmod 0440 /etc/sudoers.d/99-$USER

# switch user
USER $USER
WORKDIR $HOME

# create directory for build
RUN mkdir $HOME/mod-workdir

# mod-plugin-builder settings
ENV MPB_GIT_URL https://github.com/moddevices/mod-plugin-builder
ENV MPB_GIT_HASH 61853bfa88467011ffbfbc73f08e3040d8451dfd

# checkout mod-plugin-builder
RUN git clone $MPB_GIT_URL
WORKDIR mod-plugin-builder
RUN git checkout $MPB_GIT_HASH

# turn off progress bar
RUN sed -i 's/CT_LOG_PROGRESS_BAR=y/CT_LOG_PROGRESS_BAR=n/' toolchain/*.config

# CMD
CMD ["bash"]
