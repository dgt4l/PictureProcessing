#ifndef MTCI_THREADS
#define MTCI_THREADS
#include <pthread.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include <cmath>
#include <cstdio>

class MultiThreadConvert {
 public:
  uint8_t *data = NULL;
  uint8_t *new_data = NULL;
  size_t size = 0;
  int w;
  int h;
  int channels;
  size_t new_size = 0;
  int new_w;
  int new_h;
  int new_channels;
  uint8_t channel;
  uint64_t center;
  uint32_t cr;
  uint32_t cc;
  uint32_t ker_h;
  uint32_t ker_w;
  double *ker;

  MultiThreadConvert() { size = 0; };
  MultiThreadConvert(uint8_t *_data, size_t _size, int _w, int _h,
                     int _channels, uint8_t _channel, uint64_t _center,
                     uint32_t _cr, uint32_t _cc, uint32_t _ker_h,
                     uint32_t _ker_w, double *_ker) {
    memcpy(data, _data, _size);
    new_data = new uint8_t[_w * _h];
    memcpy(new_data, _data, _size);
    w = _w;
    h = _h;
    channels = _channels;
    channel = _channel;
    center = _center;
    cr = _cr;
    cc = _cc;
    ker_h = _ker_h;
    ker_w = _ker_w;
    ker = _ker;
  }
  ~MultiThreadConvert() {
    delete[] new_data;
    delete[] data;
  }

  MultiThreadConvert &mt_convolution(uint8_t channel, uint32_t ker_w,
                                     uint32_t ker_h, double ker[], uint32_t cr,
                                     uint32_t cc);
};

typedef struct {
  uint64_t k;
} data_t;

extern MultiThreadConvert mtc;

void *thread_convertion_task(void *ptr);
#endif