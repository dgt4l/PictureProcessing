#include <pthread.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include <cmath>
#include <cstdio>
#include <iostream>

#include "util/mtci_transform.h"

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
    data = new uint8_t[_size];
    memcpy(data, _data, _size * sizeof(uint8_t));
    new_data = new uint8_t[_size];
    memcpy(new_data, _data, _size * sizeof(uint8_t));
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

MultiThreadConvert mtc;

void *thread_convertion_task(void *ptr);

#define BYTE_BOUND(value) value < 0 ? 0 : (value > 255 ? 255 : value)

MultiThreadConvert &MultiThreadConvert::mt_convolution(
    uint8_t channel, uint32_t ker_w, uint32_t ker_h, double ker[], uint32_t cr,
    uint32_t cc) {
  std::cout << w << "x" << h << std::endl;
  // uint8_t* new_data = new uint8_t[w * h];
  uint64_t center = cr * ker_w + cc;
  pthread_t pool[50];
  for (uint64_t k = channel; k < size; k += channels) {
    for (int i = 0; i < 100; ++i) {
      data_t data = {k};
      k += channels;
      pthread_create(&pool[i], NULL, thread_convertion_task, &data);
    }
    for (int i = 0; i < 100; ++i) {
      data_t data = {k};
      pthread_join(pool[i], NULL);
    }
  }
  for (uint64_t k = channel; k < size; k += channels) {
    data[k] = mtc.new_data[k / channels];
  }
  return *this;
}

void *thread_convertion_task(void *ptr) {
  data_t *d = (data_t *)ptr;
  uint64_t k = d->k;
  double c = 0;
  printf("%d", k);
  for (long i = -((long)mtc.cr); i < (long)mtc.ker_h - mtc.cr; ++i) {
    long row = ((long)k / mtc.channels) / mtc.w - i;
    if (row < 0) {
      row = 0;
    } else if (row > mtc.h - 1) {
      row = mtc.h - 1;
    }
    for (long j = -((long)mtc.cc); j < (long)mtc.ker_w - mtc.cc; ++j) {
      long col = ((long)d->k / mtc.channels) % mtc.w - j;
      if (col < 0) {
        col = col % mtc.w + mtc.w;
      } else if (col > mtc.w - 1) {
        col %= mtc.w;
      }
      c += mtc.ker[mtc.center + i * (long)mtc.ker_w + j] *
           mtc.data[(row * mtc.w + col) * mtc.channels + mtc.channel];
      printf("%lf +", mtc.ker[mtc.center + i * (long)mtc.ker_w + j] *
           mtc.data[(row * mtc.w + col) * mtc.channels + mtc.channel]);
    }
  }
  mtc.new_data[k / mtc.channels] = (uint8_t)BYTE_BOUND(round(c));
  printf("[%d] %d\n", k, c);
  //printf("%lf\n", c);
}

int main() {
  char path[] = "../materials/kisa.jpg";
  Image img1(path);

  auto begin = std::chrono::steady_clock::now();

  // apply_clarity_filter(img1, 1).write("../materials/clarity.png");

  // apply_blur_filter(img2, 1).write("../materials/blur.png");

  // apply_embossing_filter(img3, 1).write("../materials/embossing.png");

  // apply_borders_filter(img4, 1).write("../materials/borders.png");

  // apply_upscaling_filter(img5, 1).write("../materials/upscaling.png");

  //apply_negative_filter(img1, 1).write("../materials/output.png");
  for (int i = 0; i < 1000; ++i) printf("%d\n", img1.data[i]);
  double kernel[] = {0 / 1.0, 0 / 1.0, 0 / 1.0, 0 / 1.0, -1 / 1.0,
                     0 / 1.0, 0 / 1.0, 0 / 1.0, 0 / 1.0};
  uint32_t ker_w = 3;
  uint32_t ker_h = 3;
  uint32_t cr = 1;
  uint32_t cc = 1;
  uint64_t center = cr * ker_w + cc;
  mtc = MultiThreadConvert(img1.data, img1.size, img1.w, img1.h, img1.channels,
                           0, center, cr, cc, ker_h, ker_w, kernel);
  mtc.mt_convolution(0, 3, 3, kernel, 1, 1);
  auto end = std::chrono::steady_clock::now();

  auto elapsed_ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

  std::cout << "The time: " << elapsed_ms.count() << " ms\n";
}