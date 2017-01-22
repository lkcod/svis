// Copyright 2016 Massachusetts Institute of Technology

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/ioctl.h>
#include <termios.h>

#include <ros/ros.h>
#include <nodelet/nodelet.h>
#include <pluginlib/class_list_macros.h>

extern "C" {
#include "hid.h"
}

#define IMU_DATA_SIZE 6  // (int16_t) [ax, ay, az, gx, gy, gz]
#define IMU_BUFFER_SIZE 10  // store 10 samples (imu_stamp, imu_data) in circular buffers
#define IMU_PACKET_SIZE 17  // (int8_t) [2, imu_stamp[0], ... , imu_stamp[3], imu_data[0], ... , imu_data[11]]
#define STROBE_BUFFER_SIZE 10  // store 10 samples (strobe_stamp, strobe_count) in circular buffers
#define STROBE_PACKET_SIZE 6  // (int8_t) [1, strobe_stamp[0], ... , strobe_stamp[3], strobe_count]
#define SEND_BUFFER_SIZE 64  // (int8_t) size of HID USB packets
#define SEND_HEADER_SIZE 8  // (int8_t) [header1, header2, packet_count[0], ... , packet_count[3], imu_count, strobe_count];

namespace svis_ros {

/**
 * \brief A simple visual inertial synchronization approach.
 *
 * This package contains the ROS portion of a Simple Visual Inertial Synchronization approach that accepts camera strobe messages * from the Teensy microcontroller and synchronizes them with camera image messages.
 */
class SVISNodelet : public nodelet::Nodelet {
 public:
  /**
   * \brief Constructor.
   *
   * NOTE: Default, no-args constructor must exist.
   */
  SVISNodelet() = default;

  ~SVISNodelet() = default;
  
  SVISNodelet(const SVISNodelet& rhs) = delete;
  SVISNodelet& operator=(const SVISNodelet& rhs) = delete;

  SVISNodelet(SVISNodelet&& rhs) = delete;
  SVISNodelet& operator=(SVISNodelet&& rhs) = delete;

  /**
   * \brief Nodelet initialization.
   *
   * Subclasses of nodelet::Nodelet need to override this virtual method.
   * It takes the place of the nodelet constructor.
   */
  virtual void onInit() {
    // Grab a handle to the parent node.
    ros::NodeHandle nh;
    ros::NodeHandle pnh("~");

    Run();

    return;
  }

  /**
   * \brief Main processing loop.
   */
  void Run() {
    int i, r, num;
    char buf[64];

    // C-based example is 16C0:0480:FFAB:0200
    r = rawhid_open(1, 0x16C0, 0x0480, 0xFFAB, 0x0200);
    if (r <= 0) {
      // Arduino-based example is 16C0:0486:FFAB:0200
      r = rawhid_open(1, 0x16C0, 0x0486, 0xFFAB, 0x0200);
      if (r <= 0) {
        printf("no rawhid device found\n");
        return;
      }
    }
    printf("found rawhid device\n");

    while (ros::ok()) {
      // check if any Raw HID packet has arrived
      num = rawhid_recv(0, buf, 64, 220);
      if (num < 0) {
        printf("\nerror reading, device went offline\n");
        rawhid_close(0);
        return;
      }
      
      if (num > 0) {
        printf("\nrecv %d bytes:\n", num);
        for (i=0; i<num; i++) {
          printf("%02X ", buf[i] & 255);
          if (i % 16 == 15 && i < num-1) printf("\n");
        }
        printf("\n");
      }
    }
  }

 private:

};

}  // namespace svis_ros

// Export as a plugin.
#include <pluginlib/class_list_macros.h>
PLUGINLIB_EXPORT_CLASS(svis_ros::SVISNodelet, nodelet::Nodelet)
