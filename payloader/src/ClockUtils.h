#ifndef CLOCKUTILS_H_
#define CLOCKUTILS_H_

#include <chrono>  // NOLINT

namespace payloader {

class ClockUtils {
 public:
  static inline int64_t durationToMs(payloader::duration duration) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
  }

  static inline uint64_t timePointToMs(payloader::time_point time_point) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(time_point.time_since_epoch()).count();
  }
};

}  // namespace payloader
#endif  // CLOCKUTILS_H_
