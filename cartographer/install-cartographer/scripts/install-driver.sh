_CURRENT_DIR="$( cd "$( dirname "$0" )" && pwd )"

if [ -z "$ROS_DISTRO" ]; then
  echo -e '\e[33mNo ROS environment detected, please load a ROS environment first.'
  exit
fi

catkin_make_isolated \
    --install \
    --use-ninja \
    --merge \
    --source ${_CURRENT_DIR}/../driver \
    --install-space ${CUSTOM_ROS_BUILD_DIR}/install \
    --build ${CUSTOM_ROS_BUILD_DIR}/build \
    --devel ${CUSTOM_ROS_BUILD_DIR}/devel