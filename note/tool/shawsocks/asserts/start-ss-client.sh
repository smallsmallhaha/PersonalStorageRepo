#!/bin/bash

if [[ -a /var/run/shadowsocks.pid ]]; then
  sudo sslocal -c /etc/shadowsocks/client.json -d restart
else
  sudo sslocal -c /etc/shadowsocks/client.json -d start
fi
