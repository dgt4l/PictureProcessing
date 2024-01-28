#include "mtci_transform.h"

Image iterate_cyclic(Image img, double kernel[], int iterations) {
  for (int i = 0; i < iterations; ++i) {
    img.convolve_clamp_to_border(0, 3, 3, kernel, 1, 1);
    img.convolve_clamp_to_border(1, 3, 3, kernel, 1, 1);
    img.convolve_clamp_to_border(2, 3, 3, kernel, 1, 1);
  }

  return img;
}

Image apply_clarity_filter(Image img, int iterations) {
  double kernel[] = {0 / 1.0, -1 / 1.0,  0 / 1.0, 
                    -1 / 1.0,  5 / 1.0, -1 / 1.0, 
                     0 / 1.0, -1 / 1.0,  0 / 1.0};
  return iterate_cyclic(img, kernel, 1);
}

Image apply_blur_filter(Image img, int iterations) {
  double kernel[] = {1 / 16.0, 2 / 16.0, 1 / 16.0, 
                     2 / 16.0, 4 / 16.0, 2 / 16.0, 
                     1 / 16.0, 2 / 16.0, 1 / 16.0};
  return iterate_cyclic(img, kernel, 7);
}

Image apply_embossing_filter(Image img, int iterations) {
  double kernel[] = {-2 / 9.0, -1 / 9.0, 0 / 9.0, 
                     -1 / 9.0,  1 / 9.0, 1 / 9.0,  
                      0 / 9.0,  1 / 9.0, 2 / 9.0};
  return iterate_cyclic(img, kernel, 1);
}

// Image apply_borders_filter(Image img, int iterations) {
//   double kernel[] = {0 / 1.0, -1 / 1.0,  0 / 1.0, 
//                     -1 / 1.0,  4 / 1.0, -1 / 1.0, 
//                      0 / 1.0, -1 / 1.0,  0 / 1.0};
//   return iterate_cyclic(img, kernel, 1);
// }

Image apply_borders_filter(Image img, int iterations) {
  double kernel[] = {1 / 1.0, 0 / 1.0, -1 / 1.0, 
                     1 / 1.0, 0 / 1.0, -1 / 1.0, 
                     1 / 1.0, 0 / 1.0, -1 / 1.0};
  return iterate_cyclic(img, kernel, 1);
}

Image apply_upscaling_filter(Image img, int iterations) {
  double kernel[] = {0 / 1.0, 1 / 1.0, 0 / 1.0, 
                     1 / 1.0, 1 / 1.0, 1 / 1.0, 
                     0 / 1.0, 1 / 1.0, 0 / 1.0};
  return iterate_cyclic(img, kernel, 1);
}

Image apply_erosion_filter(Image img, int iterations) {
  double kernel[] = {1 / 10.0, 1 / 10.0, 1 / 10.0, 
                     1 / 10.0, 5 / 10.0, 1 / 10.0, 
                     1 / 10.0, 1 / 10.0, 1 / 10.0};
  return iterate_cyclic(img, kernel, 1);
}