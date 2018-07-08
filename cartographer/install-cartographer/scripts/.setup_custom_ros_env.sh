# you can modify CUSTOM_ROS_BUILD_DIR
export CUSTOM_ROS_BUILD_DIR=/opt/AlphaStar/buildANDinstall

#--------------------------------------------------------------------

if [ -d ${CUSTOM_ROS_BUILD_DIR}/data ]; then
  export CUSTOM_ROS_DATA_DIR=${CUSTOM_ROS_BUILD_DIR}/data
else
  export CUSTOM_ROS_DATA_DIR=${HOME}
fi

_SHELL_COMMAND=`ps -p $$ -oargs=`
_SHELL=

if [[ "${_SHELL_COMMAND}" =~ zsh ]]; then
  _SHELL=zsh
elif [[ "${_SHELL_COMMAND}" =~ bash ]]; then
  _SHELL=bash
else
  echo -e "\e[31mOnly bash and zsh supported."
fi

# autoload ros environment
if [ "${_SHELL}" ]; then
  if [[ -a ${CUSTOM_ROS_BUILD_DIR}/install/setup.${_SHELL} ]]; then
    echo -e "\e[32m""Custom ROS environment succesfully loaded."
    source ${CUSTOM_ROS_BUILD_DIR}/install/setup.${_SHELL} --extend
  elif [[ -a /opt/ros/melodic/setup.${_SHELL} ]]; then
    echo -e "\e[32m""Default ROS environment succesfully loaded."
    source /opt/ros/melodic/setup.${_SHELL} --extend
  elif [[ -a /opt/ros/kinetic/setup.${_SHELL} ]]; then
    echo -e "\e[32m""Default ROS environment succesfully loaded."
    source /opt/ros/kinetic/setup.${_SHELL} --extend
  else
    echo -e "\e[31mNo ROS environment found."
  fi
fi
