#include "util/mtci_transform.h"

int main() {
  Image img1("../materials/kisa.jpg"), img2("../materials/kisa.jpg"),
      img3("../materials/kisa.jpg"), img4("../materials/kisa.jpg"),
      img5("../materials/kisa.jpg"), img6("../materials/kisa.jpg");

  apply_clarity_filter(img1, 1).write("../materials/clarity.png");

  apply_blur_filter(img2, 1).write("../materials/blur.png");

  apply_embossing_filter(img3, 1).write("../materials/embossing.png");

  apply_borders_filter(img4, 1).write("../materials/borders.png");

  apply_upscaling_filter(img5, 1).write("../materials/upscaling.png");

  apply_negative_filter(img6, 1).write("../materials/negative.png");
}