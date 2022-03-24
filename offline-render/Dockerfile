FROM scratch
LABEL maintainer="Filipe Coelho <falktx@moddevices.com>"

# environment setup
ENV LANG en_US.UTF-8
ENV LD_BIND_NOW 1
ENV LV2_PATH /root/.lv2:/usr/lib/lv2
ENV MOD_DATA_DIR=/root/data
ENV MOD_LOG 1
ENV MOD_MODEL_CPU x86_64
ENV MOD_MODEL_TYPE offline-render:x86_64

# base rootfs
ADD rootfs/ /

# plugins
RUN mkdir /root/.lv2
ADD plugins/ /root/.lv2/

# extra files
ADD files/jack-internal-session.conf /etc/
ADD files/run.sh /root/
RUN mkdir /root/output
RUN touch /data/using-256-frames

# setup user
USER root
WORKDIR /root

# CMD
CMD ["/root/run.sh"]
# CMD ["bash"]
