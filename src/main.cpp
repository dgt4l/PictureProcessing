#include "lib/mtci_image.h"
#include "lib/mtci_kernels.h"

int main() {
  Image img("../materials/kisa.jpg");

  img.convolve_cyclic(1, 3, 3, MTCI_KERNELS_EMBOS, 0, 0);
  img.write("../materials/test6_blur.png");
}
