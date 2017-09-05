// Copyright 2016 Massachusetts Institute of Technology

#pragma once

#include <sensor_msgs/Image.h>
#include <sensor_msgs/CameraInfo.h>
#include "svis/image_metadata.h"

namespace svis {

class CameraPacket {
 public:
  ImageMetadata metadata;
  sensor_msgs::CameraInfo info;
  sensor_msgs::Image image;
};

}  // namespace svis_ros