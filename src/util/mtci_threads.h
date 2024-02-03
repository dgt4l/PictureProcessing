#ifndef SRC_UTIL_MTCI_THREADS_H
#define SRC_UTIL_MTCI_THREADS_H

#include <pthread.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <chrono>
#include <iostream>
#include <vector>
#include "../lib/mtci_image.h"

class MultiThreadConvert {
 public:
  std::vector<uint8_t> data;
  std::vector<uint8_t> new_data;
  size_t size = 0;
  int w;
  int h;
  int channels;
  size_t new_size = 0;
  int new_w;
  int new_h;
  int new_channels;
  uint8_t channel;
  uint16_t threads;
  uint64_t center;
  uint32_t cr;
  uint32_t cc;
  uint32_t ker_h;
  uint32_t ker_w;
  std::vector<double> ker;

  MultiThreadConvert() { size = 0; };
  MultiThreadConvert(uint8_t *_data, size_t _size, int _w, int _h,
                     int _channels, uint8_t _channel, uint64_t _center,
                     uint32_t _cr, uint32_t _cc, uint32_t _ker_h,
                     uint32_t _ker_w, double *_ker, uint16_t _threads)
      : data(_data, _data + _size),
        new_data(_data, _data + _size),
        ker(_ker, _ker + (_ker_h) * (_ker_w)) {
    size = _size;
    w = _w;
    h = _h;
    channels = _channels;
    channel = _channel;
    center = _center;
    cr = _cr;
    cc = _cc;
    ker_h = _ker_h;
    ker_w = _ker_w;
    threads = _threads;
  }

  MultiThreadConvert &mt_convolution(uint8_t channel, uint32_t ker_w,
                                     uint32_t ker_h, double ker[], uint32_t cr,
                                     uint32_t cc);
};

struct data_t {
  uint64_t k;
};

void *thread_convertion_task(void *ptr);

void mt_convolve_clamp_to_border(const Image &img1, uint8_t channel, uint32_t ker_w,
                                       uint32_t ker_h, double ker[],
                                       uint32_t cr, uint32_t cc, uint8_t threads);

#endif  // !SRC_UTIL_MTCI_THREADS_H

