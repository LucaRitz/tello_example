//
// Created by curi on 17-6-28.
//

#ifndef OPENCVIDEO_MASTER_H264DECODER_H
#define OPENCVIDEO_MASTER_H264DECODER_H

//#define __STDC_CONSTANT_MACROS
#include <stdio.h>
// Opencv
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect.hpp>

extern "C" {
//#include "libavutil/avutil.h"
#include "../lib/ffmpeg/include/libavcodec/avcodec.h"
#include "../lib/ffmpeg/include/libavformat/avformat.h"
//新版里的图像转换结构需要引入的头文件
#include "../lib/ffmpeg/include/libswscale/swscale.h"
#include "./lib/ffmpeg/include/libavutil/frame.h"
};


class H264Decoder {

public :

    H264Decoder();
//    ~H264Decoder(); TODO
    void init();
    void decode(unsigned char *inputbuf, size_t size);
    void play();
    cv::Mat getMat();
    bool ready() const { return matReady; }

private:

    const AVCodec *codec;
    AVCodecContext *c = nullptr;
    int frame_count;
    AVFrame *frame;
    AVPacket avpkt;
    AVFrame *pFrameBGR;

    int BGRsize;
    uint8_t *out_buffer = nullptr;

    struct SwsContext *img_convert_ctx;
    cv::Mat pCvMat;
    bool matReady;

};

#endif //OPENCVIDEO_MASTER_H264DECODER_H