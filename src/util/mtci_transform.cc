#include "mtci_transform.h"

Image iterate_cyclic(Image img, double kernel[], int iterations) {
  for (int i = 0; i < iterations; ++i) {
    img.convolve_linear(0, 3, 3, kernel, 0, 0);
    img.convolve_clamp_to_border(1, 3, 3, kernel, 0, 0);
    img.convolve_cyclic(2, 3, 3, kernel, 0, 0);
  }

  return img;
}

Image apply_clarity_filter(Image img, int iterations) {
  double kernel[] = {-1 / 1., -1 / 1., -1 / 1., -1 / 1., 9 / 1.,
                     -1 / 1., -1 / 1., -1 / 1., -1 / 1.};

  return iterate_cyclic(img, kernel, 2);
}

Image apply_blur_filter(Image img, int iterations) {
  double kernel[] = {1 / 16., 2 / 16., 1 / 16., 2 / 16., 4 / 16.,
                     2 / 16., 1 / 16., 2 / 16., 1 / 16.};
  return iterate_cyclic(img, kernel, 5);
}

Image apply_embossing_filter(Image img, int iterations) {
  double kernel[] = {-2 / 1., -1 / 1., 0 / 1., -1 / 1., 1 / 1.,
                     1 / 1.,  0 / 1.,  1 / 1., 2 / 1.};
  return iterate_cyclic(img, kernel, 3);
}

Image apply_borders_filter(Image img, int iterations) {
  double kernel[] = {1 / 1.,  0 / 1., -1 / 1., 1 / 1., 0 / 1.,
                     -1 / 1., 1 / 1., 0 / 1.,  -1 / 1.};
  return iterate_cyclic(img, kernel, 3);
}

Image apply_upscaling_filter(Image img, int iterations) {
  double kernel[] = {0 / 1., 1 / 1., 0 / 1., 1 / 1., 1 / 1.,
                     1 / 1., 0 / 1., 1 / 1., 0 / 1.};
  return iterate_cyclic(img, kernel, 2);
}

Image apply_erosion_filter(Image img, int iterations) {
  double kernel[] = {1 / 1., 0 / 1., 1 / 1., 0 / 1., 0 / 1.,
                     0 / 1., 1 / 1., 0 / 1., 1 / 1.};
  return iterate_cyclic(img, kernel, 3);
}