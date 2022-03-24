FROM scratch
LABEL maintainer="Filipe Coelho <falktx@moddevices.com>"

# environment setup
ENV LANG en_US.UTF-8
ENV LD_BIND_NOW 1
ENV LV2_PATH /root/.lv2:/usr/lib/lv2
ENV MOD_DATA_DIR=/root/data
# ENV MOD_KEYS_PATH /root/keys/
ENV MOD_MODEL_CPU x86_64
ENV MOD_MODEL_TYPE offline-render:x86_64
# ENV MOD_HOST_SOCKET_PORT 5555
# ENV SNDFILE_JACKPLAY_AUTOCONNECT mod-host:monitor-in_%d
USER root

# base rootfs
ADD rootfs/ /

# plugins
RUN mkdir /root/.lv2
ADD plugins/ /root/.lv2/

# jack config
ADD config/jack-internal-session.conf /etc/
RUN touch /data/using-256-frames

# CMD
CMD ["bash"]

# docker run -ti --rm -p 8080:80 -p 5555:5555 -p 5556:5556 mpb-offline-render:latest
# jackd -r -C /etc/jack-internal-session.conf -d dummy -r 48000 -p 256
# &
# python3 /usr/bin/mod-ui
