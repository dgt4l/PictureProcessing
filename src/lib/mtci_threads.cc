#include "mtci_threads.h"

#include <iostream>

#define BYTE_BOUND(value) value < 0 ? 0 : (value > 255 ? 255 : value)

MultiThreadConvert &MultiThreadConvert::mt_convolution(
    uint8_t channel, uint32_t ker_w, uint32_t ker_h, double ker[], uint32_t cr,
    uint32_t cc) {
  std::cout << w << "x" << h << std::endl;
  // uint8_t* new_data = new uint8_t[w * h];
  uint64_t center = cr * ker_w + cc;
  pthread_t pool[50];
  mtc = MultiThreadConvert(data, size, w, h, channels, channel, center, cr, cc,
                           ker_h, ker_w, ker);

  for (uint64_t k = channel; k < size; k += channels) {
    for (int i = 0; i < 100; ++i) {
      data_t data = {k};
      pthread_create(&pool[i], NULL, thread_convertion_task, &data);
    }
  }
  for (uint64_t k = channel; k < size; k += channels) {
    data[k] = mtc.new_data[k / channels];
  }
  delete[] mtc.new_data;
  delete[] mtc.data;
  return *this;
}

void *thread_convertion_task(void *ptr) {
  data_t *d = (data_t *)ptr;
  uint64_t k = d->k;
  double c = 0;
  for (long i = -((long)mtc.cr); i < (long)mtc.ker_h - mtc.cr; ++i) {
    long row = ((long)k / mtc.channels) / mtc.w - i;
    if (row < 0) {
      row = row % mtc.h + mtc.h;
    } else if (row > mtc.h - 1) {
      row %= mtc.h;
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
    }
  }
  mtc.new_data[k / mtc.channels] = (uint8_t)BYTE_BOUND(round(c));
  printf("%lf\n", c);
}
