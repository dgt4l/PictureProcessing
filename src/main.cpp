#include "lib/mtci_threads.h"
#include "util/mtci_transform.h"

int main() {
  char path[] = "../materials/kisa.jpg";
  Image img1(path), img2(path), img3(path), img4(path), img5(path), img6(path);

  auto begin = std::chrono::steady_clock::now();

  // apply_clarity_filter(img1, 1).write("../materials/clarity.png");

  // apply_blur_filter(img2, 1).write("../materials/blur.png");

  // apply_embossing_filter(img3, 1).write("../materials/embossing.png");

  // apply_borders_filter(img4, 1).write("../materials/borders.png");

  // apply_upscaling_filter(img5, 1).write("../materials/upscaling.png");

  // apply_negative_filter(img6, 1).write("../materials/negative.png");
  double kernel[] = {0 / 1.0, 0 / 1.0, 0 / 1.0, 0 / 1.0, -1 / 1.0,
                     0 / 1.0, 0 / 1.0, 0 / 1.0, 0 / 1.0};
  uint32_t ker_w = 3;
  uint32_t ker_h = 3;
  uint32_t cr = 1;
  uint32_t cc = 1;
  uint64_t center = cr * ker_w + cc;
  mtc = MultiThreadConvert(img1.data, img1.size, img1.w, img1.h, img1.channels,
                           0, center, cr, cc, ker_h, ker_w, kernel);
  mtc.mt_convolution(0, 3, 3, kernel, 1, 1);
  img1.write("output.png");
  auto end = std::chrono::steady_clock::now();

  auto elapsed_ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

  std::cout << "The time: " << elapsed_ms.count() << " ms\n";
}