#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define BYTE_BOUND(value) value < 0 ? 0 : (value > 255 ? 255 : value)

#include "mtci_image.h"

#include "stb_image.h"
#include "stb_image_write.h"

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

Image& Image::std_convolve_clamp_to_0(uint8_t channel, uint32_t ker_w,
                                      uint32_t ker_h, double ker[], uint32_t cr,
                                      uint32_t cc) {
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

Image& Image::std_convolve_clamp_to_border(uint8_t channel, uint32_t ker_w,
                                           uint32_t ker_h, double ker[],
                                           uint32_t cr, uint32_t cc) {
  uint8_t new_data[w * h];
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
      }
    }
    new_data[k / channels] = (uint8_t)BYTE_BOUND(round(c));
  }
  for (uint64_t k = channel; k < size; k += channels) {
    data[k] = new_data[k / channels];
  }
  return *this;
}

Image& Image::std_convolve_cyclic(uint8_t channel, uint32_t ker_w,
                                  uint32_t ker_h, double ker[], uint32_t cr,
                                  uint32_t cc) {
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

void Image::fft(uint32_t n, std::complex<double> x[], std::complex<double>* X) {
	//x in standard order
	if(x != X) {
		memcpy(X, x, n*sizeof(std::complex<double>));
	}

	//Gentleman-Sande butterfly
	uint32_t sub_probs = 1;
	uint32_t sub_prob_size = n;
	uint32_t half;
	uint32_t i;
	uint32_t j_begin;
	uint32_t j_end;
	uint32_t j;
	std::complex<double> w_step;
	std::complex<double> w;
	std::complex<double> tmp1, tmp2;
	while(sub_prob_size>1) {
		half = sub_prob_size>>1;
		w_step = std::complex<double>(cos(-2*M_PI/sub_prob_size), sin(-2*M_PI/sub_prob_size));
		for(i=0; i<sub_probs; ++i) {
			j_begin = i*sub_prob_size;
			j_end = j_begin+half;
			w = std::complex<double>(1,0);
			for(j=j_begin; j<j_end; ++j) {
				tmp1 = X[j];
				tmp2 = X[j+half];
				X[j] = tmp1+tmp2;
				X[j+half] = (tmp1-tmp2)*w;
				w *= w_step;
			}
		}
		sub_probs <<= 1;
		sub_prob_size = half;
	}
	//X in bit reversed order
}
void Image::ifft(uint32_t n, std::complex<double> X[], std::complex<double>* x) {
	//X in bit reversed order
	if(X != x) {
		memcpy(x, X, n*sizeof(std::complex<double>));
	}

	//Cooley-Tukey butterfly
	uint32_t sub_probs = n>>1;
	uint32_t sub_prob_size;
	uint32_t half = 1;
	uint32_t i;
	uint32_t j_begin;
	uint32_t j_end;
	uint32_t j;
	std::complex<double> w_step;
	std::complex<double> w;
	std::complex<double> tmp1, tmp2;
	while(half<n) {
		sub_prob_size = half<<1;
		w_step = std::complex<double>(cos(2*M_PI/sub_prob_size), sin(2*M_PI/sub_prob_size));
		for(i=0; i<sub_probs; ++i) {
			j_begin = i*sub_prob_size;
			j_end = j_begin+half;
			w = std::complex<double>(1,0);
			for(j=j_begin; j<j_end; ++j) {
				tmp1 = x[j];
				tmp2 = w*x[j+half];
				x[j] = tmp1+tmp2;
				x[j+half] = tmp1-tmp2;
				w *= w_step;
			}
		}
		sub_probs >>= 1;
		half = sub_prob_size;
	}
	for(uint32_t i=0; i<n; ++i) {
		x[i] /= n;
	}
	//x in standard order
}

void Image::dft_2D(uint32_t m, uint32_t n, std::complex<double> x[], std::complex<double>* X) {
	//x in row-major & standard order
	std::complex<double>* intermediate = new std::complex<double>[m*n];
	//rows
	for(uint32_t i=0; i<m; ++i) {
		fft(n, x+i*n, intermediate+i*n);
	}
	//cols
	for(uint32_t j=0; j<n; ++j) {
		for(uint32_t i=0; i<m; ++i) {
			X[j*m+i] = intermediate[i*n+j]; //row-major --> col-major
		}
		fft(m, X+j*m, X+j*m);
	}
	delete[] intermediate;
	//X in column-major & bit-reversed (in rows then columns)
}
void Image::idft_2D(uint32_t m, uint32_t n, std::complex<double> X[], std::complex<double>* x) {
	//X in column-major & bit-reversed (in rows then columns)
	std::complex<double>* intermediate = new std::complex<double>[m*n];
	//cols
	for(uint32_t j=0; j<n; ++j) {
		ifft(m, X+j*m, intermediate+j*m);
	}
	//rows
	for(uint32_t i=0; i<m; ++i) {
		for(uint32_t j=0; j<n; ++j) {
			x[i*n+j] = intermediate[j*m+i]; //row-major <-- col-major
		}
		ifft(n, x+i*n, x+i*n);
	}
	delete[] intermediate;
	//x in row-major & standard order
}

uint32_t Image::rev(uint32_t n, uint32_t a) {
  uint8_t max_bits = (uint8_t)ceil(log2(n));
  uint32_t reversed_a = 0;
  for (uint8_t i = 0; i < max_bits; ++i) {
    if (a & (1 << i)) {
      reversed_a |= (1 << (max_bits - 1 - i));
    }
  }
  return reversed_a;
}
void Image::bit_rev(uint32_t n, std::complex<double> a[],
                    std::complex<double>* A) {
  for (uint32_t i = 0; i < n; ++i) {
    A[rev(n, i)] = a[i];
  }
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

Image& Image::grayscale_lum() {
  if (channels < 3) {
    printf(
        "Image %p has less than 3 channels, it is assumed to already be "
        "grayscale.",
        this);
  } else {
    for (int i = 0; i < size; i += channels) {
      int gray = 0.2126 * data[i] + 0.7152 * data[i + 1] + 0.0722 * data[i + 2];
      memset(data + i, gray, 3);
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
