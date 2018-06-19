#!/bin/bash

_CURRENT_DIR="$( cd "$( dirname "$0" )" && pwd )"

_REPO_PATH_AND_BRANCH=(\
../driver/xsens_node master \
../SLAM/cartographer master \
../SLAM/cartographer_ros master \
../SLAM/lidar_slam master \
)

_NUM=$((${#_REPO_PATH_AND_BRANCH[*]} / 2))

for ((i=0; i<${_NUM}; i ++))
do
  _i1=$(($i*2))
  _i2=$(($i*2+1))
  _REPO=${_REPO_PATH_AND_BRANCH[$_i1]}
  _BRANCH=${_REPO_PATH_AND_BRANCH[$_i2]}
  cd ${_CURRENT_DIR}/${_REPO}
  git fetch --all
  git reset --hard origin/${_BRANCH}
done
