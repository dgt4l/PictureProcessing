#include "lib/mtci_image.h"
#include "util/mtci_kernels.h"

int main() {
  Image img("../materials/kisa.jpg");

  double *kernel = MTCI_KERNELS_CLARI;

  img.convolve_cyclic(0, 3, 3, kernel, 0, 0);
  img.convolve_cyclic(1, 3, 3, kernel, 0, 0);
  img.convolve_cyclic(2, 3, 3, kernel, 0, 0);

  img.write("../materials/output.png");
}
