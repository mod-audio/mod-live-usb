#!/usr/bin/env bash
# shellcheck disable=SC2034

iso_name="mod-live-usb"
iso_label="v5"
iso_publisher="MOD Devices <https://moddevices.com>"
iso_application="MOD Live USB"
iso_version="v5"
install_dir="arch"
bootmodes=('bios.syslinux.mbr' 'bios.syslinux.eltorito' 'uefi-x64.systemd-boot.esp' 'uefi-x64.systemd-boot.eltorito')
arch="x86_64"
pacman_conf="pacman.conf"
airootfs_image_type="squashfs"
airootfs_image_tool_options=('-comp' 'xz' '-Xbcj' 'x86' '-b' '1M' '-Xdict-size' '1M')
file_permissions=(
  ["/etc/shadow"]="0:0:400"
  ["/root"]="0:0:750"
  ["/root/.mod-live/mod-live-usb-welcome"]="0:0:755"
  ["/root/.mod-live/mod-live-usb-welcome.run"]="0:0:755"
  ["/root/.mod-live/start.sh"]="0:0:755"
  ["/usr/local/bin/livecd-sound"]="0:0:755"
)
