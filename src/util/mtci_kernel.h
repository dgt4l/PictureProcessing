#ifndef SRC_UTIL_MTCI_KERNELS_H
#define SRC_UTIL_MTCI_KERNELS_H

const int DEFAULT_KERNEL_SIZE = 3;

class Kernel {
 private:
  double *kernel;
  int rows;
  int cols;
  double normal = 1;
  void applyNormalization();

 public:
  Kernel(double _kernel[]);
  Kernel(double _kernel[], int dim);

  Kernel(double _kernel[], int _rows, int _cols);
  Kernel(double _kernel[], int _rows, int _cols, double _normal);
};

#endif  // !SRC_UTIL_MTCI_KERNELS_H