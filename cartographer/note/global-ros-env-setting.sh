cat << EOF | sudo tee /etc/ld.so.conf.d/ros.conf
/opt/AlphaStar/buildANDinstall/install/lib
/opt/ros/kinetic/lib
/opt/ros/kinetic/lib/x86_64-linux-gnu
EOF

sudo ldconfig


cat << EOF | sudo tee /etc/udev/rules.d/90-SLAM-trolley-sensors.rules
# set xsens imu
SUBSYSTEMS=="usb-serial", KERNEL=="ttyUSB[0-9]*", DRIVERS=="xsens_mt", ACTION=="add", MODE="0666", SYMLINK+="trolley_sensors/imu"
# set hokuyo laserscan
SUBSYSTEMS=="usb", KERNEL=="ttyACM[0-9]*", ACTION=="add", ATTRS{idVendor}=="15d1", ATTRS{idProduct}=="0000", MODE="0666", PROGRAM="/opt/AlphaStar/buildANDinstall/install/lib/hokuyo_node/getID /dev/%k q", , SYMLINK+="trolley_sensors/hokuyo_%c"
EOF
