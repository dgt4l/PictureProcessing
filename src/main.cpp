#include <pthread.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <vector>

#include "util/mtci_transform.h"

pthread_mutex_t file_mutex;

FILE *f = fopen("hui.log", "w");

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
    // data = new uint8_t[_size];
    // std::copy(data, _data + _size, data);
    // memcpy(data, _data, _size * sizeof(uint8_t));
    // new_data = new uint8_t[_size];
    // std::copy(data_, _data + _size, new_data);
    // memcpy(new_data, _data, _size * sizeof(uint8_t));
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
    // ker = new double[_ker_h * _ker_w];
    // memcpy(ker, _ker, _ker_h * _ker_w * sizeof(uint8_t));
  }

  MultiThreadConvert &mt_convolution(uint8_t channel, uint32_t ker_w,
                                     uint32_t ker_h, double ker[], uint32_t cr,
                                     uint32_t cc);
};

struct data_t {
  uint64_t k;
};

MultiThreadConvert mtc;

void *thread_convertion_task(void *ptr);

#define BYTE_BOUND(value) value < 0 ? 0 : (value > 255 ? 255 : value)

MultiThreadConvert &MultiThreadConvert::mt_convolution(
    uint8_t channel, uint32_t ker_w, uint32_t ker_h, double ker[], uint32_t cr,
    uint32_t cc) {
  uint64_t center = cr * ker_w + cc;
  pthread_t pool[mtc.threads];
  data_t *datas = new data_t[(int) mtc.threads];
  for (unsigned i = 0; i < mtc.threads; ++i) {
    datas[i] = {i * mtc.channels};
  }
  for (int i = 0; i < mtc.threads; ++i) {
    pthread_create(&pool[i], NULL, thread_convertion_task, &datas[i]);
  }
  for (int i = 0; i < mtc.threads; ++i) {
    pthread_join(pool[i], NULL);
  }
  return *this;
}

void *thread_convertion_task(void *ptr) {
  data_t *d = (data_t *)ptr;
  uint64_t k = d->k;
  for (int step = k; step < mtc.size; step += (mtc.threads * mtc.channels)) {
    double c = 0;
    for (long i = -((long)mtc.cr); i < (long)mtc.ker_h - mtc.cr; ++i) {
      long row = ((long)step / mtc.channels) / mtc.w - i;
      if (row < 0) {
        row = 0;
      } else if (row > mtc.h - 1) {
        row = mtc.h - 1;
      }
      for (long j = -((long)mtc.cc); j < (long)mtc.ker_w - mtc.cc; ++j) {
        long col = ((long)step / mtc.channels) % mtc.w - j;
        if (col < 0) {
          col = 0;
        } else if (col > mtc.w - 1) {
          col = mtc.w - 1;
        }
        c += mtc.ker[mtc.center + i * (long)mtc.ker_w + j] *
             mtc.data[(row * mtc.w + col) * mtc.channels + mtc.channel];
      }
    }
    mtc.new_data[step] = (uint8_t)BYTE_BOUND(round(c));
  }
}

int main() {
  char path[] = "../materials/road.jpg";
  Image img1(path), img2(path), img3(path);
  
  auto begin_mp = std::chrono::steady_clock::now();
  double kernel[] = {0 / 1.0, 0 / 1.0, 0 / 1.0, 0 / 1.0, -1 / 1.0,
                     0 / 1.0, 0 / 1.0, 0 / 1.0, 0 / 1.0};
  uint32_t ker_w = 3;
  uint32_t ker_h = 3;
  uint32_t cr = 1;
  uint32_t cc = 1;
  uint64_t center = cr * ker_w + cc;
  uint8_t channel = 0;
  uint16_t threads = 16;
  mtc = MultiThreadConvert(img1.data, img1.size, img1.w, img1.h, img1.channels,
                           0, center, cr, cc, ker_h, ker_w, kernel, threads);
  mtc.mt_convolution(channel, 3, 3, kernel, 1, 1);
  for (int i = 0; i < img1.size; i += mtc.channels) {
    img1.data[i] = mtc.new_data[i];
  }
  img1.write("../materials/output_mp.png");
  auto end_mp = std::chrono::steady_clock::now();
  auto elapsed_ms_mp =
      std::chrono::duration_cast<std::chrono::milliseconds>(end_mp - begin_mp);
  auto begin = std::chrono::steady_clock::now();
  img2.convolve_clamp_to_border(0, 3, 3, kernel, 1, 1);
  img2.write("../materials/output_default.png");
  auto end = std::chrono::steady_clock::now();
  auto elapsed_ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
  std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
  std::cout << "The mp time: " << elapsed_ms_mp.count() << " ms\n";
  std::cout << "The default time: " << elapsed_ms.count() << " ms\n";
}
