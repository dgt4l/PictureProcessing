// void Kernel::applyNormalization() {
//   for (int i = 0; i < rows * cols; ++i) {
//     kernel[i] /= normal;
//   }
// }

// Kernel::Kernel(double _kernel[]) {
//   Kernel(_kernel, DEFAULT_KERNEL_SIZE, DEFAULT_KERNEL_SIZE);
// };

// Kernel::Kernel(double _kernel[], int dim) { Kernel(_kernel, dim, dim); };

// Kernel::Kernel(double _kernel[], int _rows, int _cols) {
//   kernel = _kernel, rows = _rows, cols = _cols;
// }

// Kernel::Kernel(double _kernel[], int _rows, int _cols, double _normal) {
//   kernel = _kernel, rows = _rows, cols = _cols, normal = _normal;
//   applyNormalization();
// }

// double MTCI_KERNEL_EMPTY_ARRAY[] = {0 / 1., 0 / 1., 0 / 1., 0 / 1., 1 / 1.,
//                                     0 / 1., 0 / 1., 0 / 1., 0 / 1.};
// double MTCI_KERNEL_STRHT_ARRAY[] = {1 / 1., 0 / 1., 0 / 1., 0 / 1., 1 / 1.,
//                                     0 / 1., 0 / 1., 0 / 1., 1 / 1.};
// double MTCI_KERNEL_GAUSS_ARRAY[] =
// double MTCI_KERNEL_CLARI_ARRAY[] = {-1 / 1., -1 / 1., -1 / 1., -1 / 1., 9
// / 1.,
//                                     -1 / 1., -1 / 1., -1 / 1., -1 / 1.};
// double MTCI_KERNEL_EMBOS_ARRAY[] = {-2 / 1., -1 / 1., 0 / 1., -1 / 1., 1
// / 1.,
//                                     1 / 1.,  0 / 1.,  1 / 1., 2 / 1.};
// double MTCI_KERNEL_BORDE_ARRAY[] = {1 / 1.,  0 / 1., -1 / 1., 1 / 1., 0 / 1.,
//                                     -1 / 1., 1 / 1., 0 / 1.,  -1 / 1.};

// const Kernel MTCI_KERNEL_EMPTY(MTCI_KERNEL_EMPTY_ARRAY);
// const Kernel MTCI_KERNEL_STRHT(MTCI_KERNEL_STRHT_ARRAY);
// const Kernel MTCI_KERNEL_GAUSS(MTCI_KERNEL_GAUSS_ARRAY);
// const Kernel MTCI_KERNEL_CLARI(MTCI_KERNEL_CLARI_ARRAY);
// const Kernel MTCI_KERNEL_EMBOS(MTCI_KERNEL_EMBOS_ARRAY);
// const Kernel MTCI_KERNEL_BORDE(MTCI_KERNEL_BORDE_ARRAY);
