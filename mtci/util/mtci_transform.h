#ifndef SRC_UTIL_MTCI_TRANSFORM_H
#define SRC_UTIL_MTCI_TRANSFORM_H

// #include <chrono>
#include <functional>
#include <iostream>

#include "mtci_threads.h"
// #include "mtci_kernel.h"

Image apply_clarity_filter(Image img, int iterations);
Image apply_blur_filter(Image img, int iterations);
Image apply_embossing_filter(Image img, int iterations);
Image apply_borders_filter(Image img, int iterations);
Image apply_upscaling_filter(Image img, int iterations);
Image apply_negative_filter(Image img, int iterations);
Image iterate_cyclic(Image img, double kernel[], int iterations);

Image mt_apply_blur_filter(Image img, uint8_t threads, uint8_t channel);
Image mt_apply_clarity_filter(Image img, uint8_t threads, uint8_t channel);
Image mt_apply_embossing_filter(Image img, uint8_t threads, uint8_t channel);
Image mt_apply_borders_filter(Image img, uint8_t threads, uint8_t channel);
Image mt_apply_upscaling_filter(Image img, uint8_t threads, uint8_t channel);
Image mt_apply_negative_filter(Image img, uint8_t threads, uint8_t channel);

#endif  // !SRC_UTIL_MTCI_TRANSFORM_H