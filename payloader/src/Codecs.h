#ifndef CODECS_H_
#define CODECS_H_

extern "C" {
  #include <libavcodec/avcodec.h>
}

namespace payloader {

 struct VideoCodecInfo {
    bool enabled;
    AVCodecID codec;
    int width;
    int height;
    int bitRate;
    int frameRate;
    AVPixelFormat pix_fmt;  
    VideoCodecInfo():enabled(false) { }
  };

  struct AudioCodecInfo {
    bool enabled;
    AVCodecID codec;
    int bitRate;
    int sampleRate;
    AudioCodecInfo():enabled(false) { }
  };
}
#endif /* CODECS_H_ */