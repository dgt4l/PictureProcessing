#ifndef SRC_UTIL_MTCI_TRANSFORM_H
#define SRC_UTIL_MTCI_TRANSFORM_H

#include <functional>

#include "../lib/mtci_image.h"
// #include "mtci_kernel.h"

Image apply_clarity_filter(Image img, int iterations);
Image apply_blur_filter(Image img, int iterations);
Image apply_embossing_filter(Image img, int iterations);
Image apply_borders_filter(Image img, int iterations);
Image apply_upscaling_filter(Image img, int iterations);
Image apply_erosion_filter(Image img, int iterations);

#endif  // !SRC_UTIL_MTCI_TRANSFORM_H