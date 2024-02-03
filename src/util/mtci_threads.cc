#include "mtci_threads.h"

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
    datas[i] = {i * mtc.channels + channel};
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

void mt_convolve_clamp_to_border(const Image &img1, uint8_t channel, uint32_t ker_w,
                                       uint32_t ker_h, double ker[],
                                       uint32_t cr, uint32_t cc, uint8_t threads) {
  uint64_t center = cr * ker_w + cc;
  mtc = MultiThreadConvert(img1.data, img1.size, img1.w, img1.h, img1.channels,
                           channel, center, cr, cc, ker_h, ker_w, ker, threads);
  mtc.mt_convolution(channel, 3, 3, ker, 1, 1);
  for (int i = channel; i < img1.size; i += mtc.channels) {
    img1.data[i] = mtc.new_data[i];
  }
}
