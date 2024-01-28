#include "util/mtci_transform.h"

int main() {
  Image img1("../materials/kisa.png"), img2("../materials/kisa.png"),
      img3("../materials/kisa.png"), img4("../materials/kisa.png"),
      img5("../materials/kisa.png"), img6("../materials/kisa.png");

  apply_clarity_filter(img1, 1).write("../materials/clarity.png");

  apply_blur_filter(img2, 1).write("../materials/blur.png");

  apply_embossing_filter(img3, 1).write("../materials/embossing.png");

  apply_borders_filter(img4, 1).write("../materials/borders.png");

  apply_upscaling_filter(img5, 1).write("../materials/upscaling.png");

  apply_erosion_filter(img6, 1).write("../materials/erosion.png");
}