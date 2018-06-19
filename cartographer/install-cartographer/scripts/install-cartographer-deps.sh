_CURRENT_DIR="$( cd "$( dirname "$0" )" && pwd )"
sudo rosdep init
# update ros deps
rosdep update
# install deps for cartographer
rosdep install --from-paths ${_CURRENT_DIR}/../SLAM --ignore-src --rosdistro=${ROS_DISTRO} -y
# install ninja
sudo apt-get install ninja-build -y