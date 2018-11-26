#ifndef RTPFRAGMENTER_H_
#define RTPFRAGMENTER_H_

#include <queue>
#include "../logger.h"

namespace payloader {

class RtpFragmenter {
  DECLARE_LOGGER();

 public:
  RtpFragmenter(unsigned char* data, unsigned int length);
  virtual ~RtpFragmenter();

  int getPacket(unsigned char* data, unsigned int* length, bool* lastPacket);

 private:
  struct Fragment {
    unsigned int position;
    unsigned int size;
    bool first;
  };
  void calculatePackets();
  unsigned int writeFragment(const Fragment& fragment, unsigned char* buffer, unsigned int* length);
  unsigned char* totalData_;
  unsigned int totalLenth_;
  unsigned int maxlength_;
  std::queue<Fragment> fragmentQueue_;
};
}  // namespace payloader
#endif  // RTPFRAGMENTER_H_
