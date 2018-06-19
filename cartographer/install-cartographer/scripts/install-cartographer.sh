_CURRENT_DIR="$( cd "$( dirname "$0" )" && pwd )"
catkin_make_isolated \
    --install \
    --use-ninja \
    --merge \
    --source ${_CURRENT_DIR}/../SLAM \
    --install-space ${CUSTOM_ROS_BUILD_DIR}/install \
    --build ${CUSTOM_ROS_BUILD_DIR}/build \
    --devel ${CUSTOM_ROS_BUILD_DIR}/devel