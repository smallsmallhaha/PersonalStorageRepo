_CURRENT_DIR="$( cd "$( dirname "$0" )" && pwd )"
rm -rf ${CUSTOM_ROS_BUILD_DIR}/build/protobuf
mkdir -p ${CUSTOM_ROS_BUILD_DIR}/build/protobuf
cd ${CUSTOM_ROS_BUILD_DIR}/build/protobuf
cmake -G Ninja \
  -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
  -DCMAKE_BUILD_TYPE=Release \
  -Dprotobuf_BUILD_TESTS=OFF \
  -DCMAKE_INSTALL_PREFIX=${CUSTOM_ROS_BUILD_DIR}/install \
  ${_CURRENT_DIR}/../third-party/protobuf/cmake
ninja install