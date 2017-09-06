// Copyright 2016 Massachusetts Institute of Technology

#pragma once

#include <string>

#include "svis/stamp.h"

namespace svis {

struct Header {
  uint32_t seq = 0;
  double stamp = 0.0;
  std::string frame_id = "";
};

}  // namespace svis
