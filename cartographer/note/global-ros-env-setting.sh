# Load runtime environment of ROS and cartographer for all executables
cat << EOF | sudo tee /etc/ld.so.conf.d/ros.conf
/opt/AlphaStar/buildANDinstall/install/lib
/opt/ros/kinetic/lib
/opt/ros/kinetic/lib/x86_64-linux-gnu
EOF

sudo ldconfig


# Configure trolley's sensors: 1 imu, 3 lidars, 1 camera control board
cat << EOF | sudo tee /etc/udev/rules.d/90-SLAM-trolley-sensors.rules
# set xsens imu
SUBSYSTEMS=="usb-serial", KERNEL=="ttyUSB[0-9]*", DRIVERS=="xsens_mt", ACTION=="add", MODE="0666", SYMLINK+="trolley_sensors/imu"
# set hokuyo laserscan
SUBSYSTEMS=="usb", KERNEL=="ttyACM[0-9]*", ACTION=="add", ATTRS{idVendor}=="15d1", ATTRS{idProduct}=="0000", MODE="0666", PROGRAM="/opt/AlphaStar/buildANDinstall/install/lib/hokuyo_node/getID /dev/%k q", , SYMLINK+="trolley_sensors/hokuyo_%c"
# camera controller
KERNEL=="ttyACM[0-9]*", SUBSYSTEMS=="usb", ATTRS{idVendor}=="0451", ATTRS{idProduct}=="f432", ACTION=="add", MODE="0666", SYMLINK+="trolley_sensors/camera_controller"
EOF


# Add software support for Panasonic DMC-GX8 Storage Card's exfat filesystem
sudo apt-get install exfat-utils exfat-fuse

# Auto mount sdcard when camera storage mode on
<<COMMENT
Append the following to /etc/fstab:
UUID=3382-675E /media/alphasttar/camera0 exfat-fuse defaults,nofail 0 0
UUID=3382-675E /media/alphasttar/camera1 exfat-fuse defaults,nofail 0 0
UUID=3382-675E /media/alphasttar/camera2 exfat-fuse defaults,nofail 0 0
UUID=3382-675E /media/alphasttar/camera3 exfat-fuse defaults,nofail 0 0
UUID=3382-675E /media/alphasttar/camera4 exfat-fuse defaults,nofail 0 0
UUID=3382-675E /media/alphasttar/camera5 exfat-fuse defaults,nofail 0 0
COMMENT
