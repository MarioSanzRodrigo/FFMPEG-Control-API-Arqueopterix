#ifndef CLOCK_H_
#define CLOCK_H_

#include <chrono>  // NOLINT

namespace payloader {

using clock = std::chrono::steady_clock;
using time_point = std::chrono::steady_clock::time_point;
using duration = std::chrono::steady_clock::duration;

}  // namespace payloader
#endif  // CLOCK_H_
