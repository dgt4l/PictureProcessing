#include <stdint.h>

#include <complex>
#include <cstdio>

// legacy feature of C
#undef __STRICT_ANSI__
#define _USE_MATH_DEFINES
#include <cmath>
#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

// #include "../util/mtci_transform.h"

#define STEG_HEADER_SIZE sizeof(uint32_t) * 8

enum ImageType { PNG, JPG, BMP, TGA };

struct Image {
  uint8_t* data = NULL;
  size_t size = 0;
  int w;
  int h;
  int channels;

  Image();
  Image(const char* filename, int channel_force = 0);
  Image(int w, int h, int channels = 3);
  Image(const Image& img);
  ~Image();

  bool read(const char* filename, int channel_force = 0);
  bool write(const char* filename);

  ImageType get_file_type(const char* filename);

  Image& convolve_linear(uint8_t channel, uint32_t ker_w, uint32_t ker_h,
                         double ker[], uint32_t cr, uint32_t cc);
  Image& convolve_clamp_to_border(uint8_t channel, uint32_t ker_w,
                                  uint32_t ker_h, double ker[], uint32_t cr,
                                  uint32_t cc);
  Image& mt_convolve_clamp_to_border(uint8_t channel, uint32_t ker_w,
                                     uint32_t ker_h, double ker[], uint32_t cr,
                                     uint32_t cc);
  Image& convolve_cyclic(uint8_t channel, uint32_t ker_w, uint32_t ker_h,
                         double ker[], uint32_t cr, uint32_t cc);
  Image& grayscale_avg();

  Image& color_mask(float r, float g, float b);

  Image& flipX();
  Image& flipY();
};
