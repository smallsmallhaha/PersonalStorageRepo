#!/bin/bash

# 用户ROS软件包安装位置，可自定义
CUSTOM_ROS_BUILD_DIR=/opt/AlphaStar/buildANDinstall

#--------------------------------------------------------------------

_CURRENT_DIR="$( cd "$( dirname "$0" )" && pwd )"

# print infomation with color
function error() { echo -e "\e[31m"$@"\e[0m"; }
function warning() { echo -e "\e[33m"$@"\e[0m"; }
function green() { echo -e "\e[32m"$@"\e[0m"; }


# 检查是否已加载ROS环境，若已加载，则发出警告并退出
#if [ $ROS_DISTRO ]; then
#  warning 'Please do not load any ROS environment before installation!!'
#  warning 'Suggestion: Delete "source <ros-dir>/setup.bash" in ~/.bashrc\e[0m'
#fi

function create_ros_install_dir() {
  _INSTALL_DIR="$1"
  green "Creating directory for new installation (${_INSTALL_DIR})..."
  sudo mkdir -p "${_INSTALL_DIR}"
  sudo chown $USER:$USER "${_INSTALL_DIR}"
}

# 删除原文件夹，新建同名文件夹
function recreate_ros_install_dir() {
  _INSTALL_DIR="$1"
  green "Removing old installation (${_INSTALL_DIR})..."
  sudo rm -rf "${_INSTALL_DIR}"
  create_ros_install_dir "${_INSTALL_DIR}"
}

# 将加载ROS环境的代码加入.bashrc或.zshrc
function add_source_setup_to_bashrc() {
  cp "${_CURRENT_DIR}/.setup_custom_ros_env.sh" ~
  if [ -z "$(grep '^source ~/.setup_custom_ros_env.sh' ~/.bashrc)" ]; then
    green "Adding 'source ~/.setup_custom_ros_env.sh' to ~/.bashrc"
    echo "source ~/.setup_custom_ros_env.sh" >> ~/.bashrc
  fi
  if [[ -a ~/.zshrc ]]; then
    if [ -z "$(grep '^source ~/.setup_custom_ros_env.sh' ~/.zshrc)" ]; then
      green "Adding 'source ~/.setup_custom_ros_env.sh' to ~/.zshrc"
      echo "source ~/.setup_custom_ros_env.sh" >> ~/.zshrc
    fi
  fi
  green "New environment initialized."
  warning "Please close current terminal and start a new one."
}

# 安装ROS环境
if [ -a "${CUSTOM_ROS_BUILD_DIR}" ]; then
  # if folder exists
  while true; do
    warning "Old installation in ${CUSTOM_ROS_BUILD_DIR} detected."
    echo "Options:"
    echo "  1. Delete old installation and create a new setup environment"
    echo "  2. Keep the old installation, just update .setup_custom_ros_env.sh"
    echo "  3. Exit"
    read -p "Select an option: " _option
    case $_option in
      1 )
        recreate_ros_install_dir "${CUSTOM_ROS_BUILD_DIR}"
        add_source_setup_to_bashrc
        break;;
      2 )
        add_source_setup_to_bashrc
        break;;
      3 )
        break;;
      * )
        echo "Please select a valid option."
        ;;
    esac
  done
else
  # folder does not exist
  echo "Options:"
  echo "  1. Create a new setup environment"
  echo "  2. Exit"
  read -p "Select an option: " _option
  case $_option in
    1 )
      create_ros_install_dir "${CUSTOM_ROS_BUILD_DIR}"
      add_source_setup_to_bashrc
      ;;
    2 )
      ;;
    * )
      echo "Please select a valid option."
      ;;
    esac
fi
