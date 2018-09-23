# slam遇到的一些问题
## hokuyo激光雷达获取强度信息，则扫描角将会受限
[ WARN] [1510732704.063576793]: More than 95.000000 degree/cluster scan range requested 
on UTM-30LX firmware version 1.20.02(16/Jul./2013) in intensity mode with cluster=1. 
The max_ang was adjusted to limit the range. You may extend the scanner's angular range 
using the allow_unsafe_settings option, but this may result in incorrect data or laser 
crashes that will require a power cycle of the laser.

# 
# use_sim_time 影响ros::Time::now()的输出，当为true时，输出为主题/Clock publish的时间，且只能被/Clock所改变；当为false时，输出为系统当前时间
# assert只使用PointCloud2 MultiEchoLaserScan LaserScan的输入
# pbstream只含有pose_graph 。。。两种数据结构
# num_subdivisions_per_laser_scan 意思是将LaserScan的ranges数据分成多少份
trajectory_builder_->AddRangefinderData(
        sensor_id, time, sensor_to_tracking->translation().cast<float>(),
        carto::sensor::TransformPointCloud(ranges,
        sensor_to_tracking->cast<float>()));

# c11 智能指针详解
## auto_ptr
构造函数、拷贝构造函数、赋值运算符可以用于传递控制权
.release() 销毁旧的auto_ptr，将控制权传递给release的返回值
.reset() 彻底重置指针
## unique_ptr
控制权单一
只能使用std::move()传递控制权
## shared_ptr
控制权可共享
使用引用计数
## weak_ptr
不能决定所指对象的生命周期，引用所指对象时，需要lock()成shared_ptr才能使用


# cartographer 消息处理
--
 |-- cartographer_ros::Node
   |-- subscribers_
   |-- Handle**Message()
   |-- builduilder_bridge_
     |-- sensor_bridge[]
       |-- sensor_bridge
         |-- Handle**Message()
         |-- trajectory_builder_
           |-- Add**Data()
           |-- AddSensorData()

cartographer_node 运行命令
/home/whu/Documents/ros/develANDinstall/install/lib/cartographer_ros/cartographer_node -configuration_directory /home/whu/Documents/ros/develANDinstall/install/share/cartographer_ros/configuration_files -configuration_basename backpack_2d.lua echoes:=horizontal_laser_2d __name:=cartographer_node
        
/home/whu/Documents/ros/develANDinstall/install/lib/cartographer_ros/cartographer_offline_node -configuration_directory /home/whu/Documents/ros/develANDinstall/install/share/cartographer_ros/configuration_files -configuration_basename backpack_2d.lua -urdf_filename /home/whu/Documents/ros/develANDinstall/install/share/cartographer_ros/urdf/backpack_2d.urdf -bag_filenames /home/whu/Documents/ros/data/cartographer/b2-2016-04-05-14-44-52.bag echoes:=horizontal_laser_2d __name:=cartographer_offline_node
         
         
         
         


_parse_pbstream_file args:
-bag_filenames /home/whu/Documents/backup/data/lidar_slam/3lidar+imu/_2017-11-14-11-59-55.bag -pose_graph_filename /home/whu/Documents/backup/data/lidar_slam/3lidar+imu/_2017-11-14-11-59-55.bag.pbstream



cartograhper最终要的两个类,一个是LocalTrajectoryBuilder,一个是SparsePoseGraph
前者负责建图(位姿外推器和子图构建过程相互作用,构建出子图)
后者负责进行回环检测(构建稀稀疏位姿图进行回环检测)
