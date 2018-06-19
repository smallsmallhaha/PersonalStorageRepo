1. 购买云服务器，建议选择 digitalocean SF2 区域的 ubuntu 18.04 版本的服务器
   使用 adduser 添加用户
   使用 visudo 将给新用户添加 sudo 权限

2. 安装配置shadowsocks服务器端
```
# 安装shadowsocks
pip install git+https://github.com/shadowsocks/shadowsocks.git@master
# 安装chacha20 加密方式
wget https://github.com/jedisct1/libsodium/releases/download/1.0.16/libsodium-1.0.16.tar.gz
tar zxvf libsodium-1.0.16.tar.gz
cd libsodium-1.0.16
./configure
make
make install
# 配置
cat << EOF > /etc/shadowsocks.json
{
  "server" : "::",
  "server_port" : 8388,
  "local_address" : "127.0.0.1",
  "local_port" : 1080,
  "password" : "your-password",
  "timeout" : 300,
  "method" : "chacha20",
  "fast_open" : false
}
EOF
# 启动
ssserver -c /etc/shadowsocks.json -d start
```
3. 安装配置shadowsocks客户端
* Linux
```
Linux客户端推荐shadowsocks官方软件或者ss-qt5，安装和配置方式如下：
* shadowsocks官方软件
apt-get install shadowsocks
cat << EOF > /etc/shadowsocks/client.json
{
  "server" : "your-server-ip",
  "server_port" : 8388,
  "local_address" : "127.0.0.1",
  "local_port" : 1080,
  "password" : "your-password",
  "timeout" : 600,
  "method" : "chacha20"
}
EOF
sslocal -c /etc/shadowsocks/client.json -d start
* ss-qt5
add-apt-repository ppa:hzwhuang/ss-qt5 
apt-get update 
apt-get install shadowsocks-qt5

开启带PAC的全局代理:
genpac --pac-proxy "SOCKS5 127.0.0.1:1080" -o autoproxy.pac
将生成的 autoproxy.pac 加入系统的自动代理路径中
```

4. 开启bbr加速
```
cat << EOF > /etc/sysctl.d/60-bbr-speedup.conf
net.core.default_qdisc=fq
net.ipv4.tcp_congestion_control=bbr
EOF
sysctl --system
```
