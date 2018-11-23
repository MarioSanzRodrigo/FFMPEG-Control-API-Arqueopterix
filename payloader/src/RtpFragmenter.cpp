#include "RtpFragmenter.h"

#include <string>
#include <cstdio>
#include <cstring>

#define MAX_SIZE 1100  // max fragment size including vp8 payload descriptor
#define VP8 0

using std::memcpy;

namespace payloader {

DEFINE_LOGGER(RtpFragmenter, "rtp.RtpFragmenter");

RtpFragmenter::RtpFragmenter(unsigned char* data, unsigned int length)
    : totalData_(data), totalLenth_(length) {
  calculatePackets();
}

RtpFragmenter::~RtpFragmenter() {
}

int RtpFragmenter::getPacket(unsigned char* data, unsigned int* length, bool* lastPacket) {
  if (fragmentQueue_.size() > 0) {
    const Fragment& test = fragmentQueue_.front();

    *length = writeFragment(test, data, length);
    fragmentQueue_.pop();
    if (fragmentQueue_.empty())
    *lastPacket = true;
  }
  return 0;
}

void RtpFragmenter::calculatePackets() {
  unsigned int remaining = totalLenth_;
  unsigned int currentPos = 0;
  while (remaining > 0) {
    // ELOG_DEBUG("Packetizing, remaining %u", remaining);
    Fragment newFragment;
    newFragment.first = false;
    newFragment.position = currentPos;
    if (currentPos == 0) {
      newFragment.first = true;
    }
    newFragment.size = remaining > MAX_SIZE - 1 ? MAX_SIZE - 1 : remaining;
    // ELOG_DEBUG("New fragment size %u, position %u", newFragment.size,
    //            newFragment.position);
    currentPos += newFragment.size;
    remaining -= newFragment.size;
    fragmentQueue_.push(newFragment);
  }
}

unsigned int RtpFragmenter::writeFragment(const Fragment& fragment, unsigned char* buffer, unsigned int* length) {
  if (VP8) {
    buffer[0] = 0x0;
    if (fragment.first)
    buffer[0] |= 0x10;  // S bit 1 // era 01
    memcpy(&buffer[1], &totalData_[fragment.position], fragment.size);
    return (fragment.size + 1);
  } else {
    memcpy(&buffer[0], &totalData_[fragment.position], fragment.size);
    return fragment.size;
  }
}

}  // namespace payloader
