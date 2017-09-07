// Copyright 2016 Massachusetts Institute of Technology

#pragma once

#include <csignal>
#include <limits>
#include <termios.h>

#include <sensor_msgs/Image.h>
#include <sensor_msgs/CameraInfo.h>
#include <sensor_msgs/Imu.h>
#include <image_transport/image_transport.h>
#include <image_transport/camera_subscriber.h>
#include <dynamic_reconfigure/StrParameter.h>
#include <dynamic_reconfigure/Reconfigure.h>
#include <dynamic_reconfigure/Config.h>

#include "fla_utils/param_utils.h"

#include "svis/svis.h"
#include "svis/imu_packet.h"
#include "svis/strobe_packet.h"
#include "svis/camera_strobe_packet.h"
#include "svis/image.h"
#include "svis/camera_info.h"
#include "svis_ros/SvisImu.h"
#include "svis_ros/SvisStrobe.h"
#include "svis_ros/SvisTiming.h"

namespace svis_ros {

class SVISRos {
 public:
  SVISRos();
  void Run();

  static volatile std::sig_atomic_t stop_signal_;

 private:
  void GetParams();
  void InitSubscribers();
  void InitPublishers();

  // callbacks
  void CameraCallback(const sensor_msgs::Image::ConstPtr& image_msg,
                      const sensor_msgs::CameraInfo::ConstPtr& info_msg);

  // publishers
  void PublishImuRaw(const std::vector<svis::ImuPacket>& imu_packets);
  void PublishImu(const std::vector<svis::ImuPacket>& imu_packets_filt);
  void PublishStrobeRaw(const std::vector<svis::StrobePacket>& strobe_packets);
  void PublishTiming(const svis::Timing& timing);
  void PublishCamera(std::vector<svis::CameraStrobePacket>& camera_strobe_packets);
  double TimeNow();
  
  void ConfigureCamera();

  // conversions
  svis::Image* RosImageToSvis(const sensor_msgs::Image& ros_image);
  sensor_msgs::Image* SvisToRosImage(const svis::Image& svis_image);
  svis::CameraInfo* RosCameraInfoToSvis(const sensor_msgs::CameraInfo& ros_info);
  sensor_msgs::CameraInfo* SvisToRosCameraInfo(const svis::CameraInfo& svis_info);

  // ros
  ros::NodeHandle nh_;
  ros::NodeHandle pnh_;
  image_transport::ImageTransport it_;

  // publishers
  image_transport::CameraPublisher camera_pub_;
  ros::Publisher imu_pub_;
  ros::Publisher svis_imu_pub_;
  ros::Publisher svis_strobe_pub_;
  ros::Publisher svis_timing_pub_;

  // subscribers
  image_transport::CameraSubscriber camera_sub_;

  bool received_camera_ = false;
  svis::SVIS svis_;
};

}  // namespace svis_ros
