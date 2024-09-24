#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define BYTE_BOUND(value) value < 0 ? 0 : (value > 255 ? 255 : value)

#include "mtci_image.h"

#include <pthread.h>
#include <unistd.h>

#include <iostream>

#include "../include/stb_image.h"
#include "../include/stb_image_write.h"


Image::Image(): data(NULL), size(0) {}
Image::Image(const char* filename, int channel_force) {
  if (read(filename, channel_force)) {
    printf("Read %s\n", filename);
    size = w * h * channels;
  } else {
    printf("Failed to read %s\n", filename);
  }
}

Image::Image(int w, int h, int channels) : w(w), h(h), channels(channels) {
  size = w * h * channels;
  data = new uint8_t[size];
}

Image::Image(const Image& img) : Image(img.w, img.h, img.channels) {
  memcpy(data, img.data, size);
}

Image::~Image() { stbi_image_free(data); }

bool Image::read(const char* filename, int channel_force) {
  data = stbi_load(filename, &w, &h, &channels, channel_force);
  channels = channel_force == 0 ? channels : channel_force;
  return data != NULL;
}

bool Image::write(const char* filename) {
  ImageType type = get_file_type(filename);
  int success;
  switch (type) {
    case PNG:
      success = stbi_write_png(filename, w, h, channels, data, w * channels);
      break;
    case BMP:
      success = stbi_write_bmp(filename, w, h, channels, data);
      break;
    case JPG:
      success = stbi_write_jpg(filename, w, h, channels, data, 100);
      break;
    case TGA:
      success = stbi_write_tga(filename, w, h, channels, data);
      break;
  }
  if (success != 0) {
    printf("\e[32mWrote \e[36m%s\e[0m, %d, %d, %d, %zu\n", filename, w, h,
           channels, size);
    return true;
  } else {
    printf("\e[31;1m Failed to write \e[36m%s\e[0m, %d, %d, %d, %zu\n",
           filename, w, h, channels, size);
    return false;
  }
}

ImageType Image::get_file_type(const char* filename) {
  const char* ext = strrchr(filename, '.');
  if (ext != nullptr) {
    if (strcmp(ext, ".png") == 0) {
      return PNG;
    } else if (strcmp(ext, ".jpg") == 0) {
      return JPG;
    } else if (strcmp(ext, ".bmp") == 0) {
      return BMP;
    } else if (strcmp(ext, ".tga") == 0) {
      return TGA;
    }
  }
  return PNG;
}

Image& Image::convolve_linear(uint8_t channel, uint32_t ker_w, uint32_t ker_h,
                              double ker[], uint32_t cr, uint32_t cc) {
  uint8_t new_data[w * h];
  uint64_t center = cr * ker_w + cc;
  for (uint64_t k = channel; k < size; k += channels) {
    double c = 0;
    for (long i = -((long)cr); i < (long)ker_h - cr; ++i) {
      long row = ((long)k / channels) / w - i;
      if (row < 0 || row > h - 1) {
        continue;
      }
      for (long j = -((long)cc); j < (long)ker_w - cc; ++j) {
        long col = ((long)k / channels) % w - j;
        if (col < 0 || col > w - 1) {
          continue;
        }
        c += ker[center + i * (long)ker_w + j] *
             data[(row * w + col) * channels + channel];
      }
    }
    new_data[k / channels] = (uint8_t)BYTE_BOUND(round(c));
  }
  for (uint64_t k = channel; k < size; k += channels) {
    data[k] = new_data[k / channels];
  }
  return *this;
}

Image& Image::convolve_clamp_to_border(uint8_t channel, uint32_t ker_w,
                                       uint32_t ker_h, double ker[],
                                       uint32_t cr, uint32_t cc) {
  uint8_t* new_data = new uint8_t[w * h];
  uint64_t center = cr * ker_w + cc;
  for (uint64_t k = channel; k < size; k += channels) {
    double c = 0;
    for (long i = -((long)cr); i < (long)ker_h - cr; ++i) {
      long row = ((long)k / channels) / w - i;
      if (row < 0) {
        row = 0;
      } else if (row > h - 1) {
        row = h - 1;
      }
      for (long j = -((long)cc); j < (long)ker_w - cc; ++j) {
        long col = ((long)k / channels) % w - j;
        if (col < 0) {
          col = 0;
        } else if (col > w - 1) {
          col = w - 1;
        }
        c += ker[center + i * (long)ker_w + j] *
             data[(row * w + col) * channels + channel];
        // printf("[%d]\n", c);
      }
    }
    new_data[k / channels] = (uint8_t)BYTE_BOUND(round(c));
    // printf("[%d] = %d\n", k / channels, new_data[k / channels]);
  }
  for (uint64_t k = channel; k < size; k += channels) {
    data[k] = new_data[k / channels];
  }
  delete[] new_data;
  return *this;
}

// double iterate_pixel(void* ptr) {
//   double c = 0;
//   for (long i = -((long)cr); i < (long)ker_h - cr; ++i) {
//     long row = ((long)k / channels) / w - i;
//     if (row < 0) {
//       row = 0;
//     } else if (row > h - 1) {
//       row = h - 1;
//     }

//     for (long j = -((long)cc); j < (long)ker_w - cc; ++j) {
//       long col = ((long)k / channels) % w - j;
//       if (col < 0) {
//         col = 0;
//       } else if (col > w - 1) {
//         col = w - 1;
//       }
//       c += ker[center + i * (long)ker_w + j] *
//            data[(row * w + col) * channels + channel];
//     }
//   }
//   return c;
// }

Image& Image::convolve_cyclic(uint8_t channel, uint32_t ker_w, uint32_t ker_h,
                              double ker[], uint32_t cr, uint32_t cc) {
  uint8_t new_data[w * h];
  uint64_t center = cr * ker_w + cc;
  for (uint64_t k = channel; k < size; k += channels) {
    double c = 0;
    for (long i = -((long)cr); i < (long)ker_h - cr; ++i) {
      long row = ((long)k / channels) / w - i;
      if (row < 0) {
        row = row % h + h;
      } else if (row > h - 1) {
        row %= h;
      }
      for (long j = -((long)cc); j < (long)ker_w - cc; ++j) {
        long col = ((long)k / channels) % w - j;
        if (col < 0) {
          col = col % w + w;
        } else if (col > w - 1) {
          col %= w;
        }
        c += ker[center + i * (long)ker_w + j] *
             data[(row * w + col) * channels + channel];
      }
    }
    new_data[k / channels] = (uint8_t)BYTE_BOUND(round(c));
  }
  for (uint64_t k = channel; k < size; k += channels) {
    data[k] = new_data[k / channels];
  }
  return *this;
}

Image& Image::grayscale_avg() {
  if (channels < 3) {
    printf(
        "Image %p has less than 3 channels, it is assumed to already be "
        "grayscale.",
        this);
  } else {
    for (int i = 0; i < size; i += channels) {
      //(r+g+b)/3
      int gray = (data[i] + data[i + 1] + data[i + 2]) / 3;
      memset(data + i, gray, 3);
    }
  }
  return *this;
}

Image& Image::color_mask(float r, float g, float b) {
  if (channels < 3) {
    printf(
        "\e[31m[ERROR] Color mask requires at least 3 channels, but this image "
        "has %d channels\e[0m\n",
        channels);
  } else {
    for (int i = 0; i < size; i += channels) {
      data[i] *= r;
      data[i + 1] *= g;
      data[i + 2] *= b;
    }
  }
  return *this;
}

Image& Image::flipX() {
  uint8_t tmp[4];
  uint8_t* px1;
  uint8_t* px2;
  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w / 2; ++x) {
      px1 = &data[(x + y * w) * channels];
      px2 = &data[((w - 1 - x) + y * w) * channels];

      memcpy(tmp, px1, channels);
      memcpy(px1, px2, channels);
      memcpy(px2, tmp, channels);
    }
  }
  return *this;
}
Image& Image::flipY() {
  uint8_t tmp[4];
  uint8_t* px1;
  uint8_t* px2;
  for (int x = 0; x < w; ++x) {
    for (int y = 0; y < h / 2; ++y) {
      px1 = &data[(x + y * w) * channels];
      px2 = &data[(x + (h - 1 - y) * w) * channels];

      memcpy(tmp, px1, channels);
      memcpy(px1, px2, channels);
      memcpy(px2, tmp, channels);
    }
  }
  return *this;
}
