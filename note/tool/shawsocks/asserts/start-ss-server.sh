#!/bin/bash

if [[ -a /var/run/shadowsocks.pid ]]; then
  sudo ssserver -c /etc/shadowsocks.json -d restart
else
  sudo ssserver -c /etc/shadowsocks.json -d start
fi
