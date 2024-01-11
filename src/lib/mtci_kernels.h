#ifndef SRC_LIB_MTCI_KERNELS_H
#define SRC_LIB_MTCI_KERNELS_H

double MTCI_KERNELS_EMPTY[] = {0 / 1., 0 / 1., 0 / 1., 0 / 1., 1 / 1.,
                               0 / 1., 0 / 1., 0 / 1., 0 / 1.};

// Equal
double MTCI_KERNELS_STRHT[] = {1 / 1., 0 / 1., 0 / 1., 0 / 1., 1 / 1.,
                               0 / 1., 0 / 1., 0 / 1., 1 / 1.};

// Blurr
double MTCI_KERNELS_GAUSS[] = {1 / 16., 2 / 16., 1 / 16., 2 / 16., 4 / 16.,
                               2 / 16., 1 / 16., 2 / 16., 1 / 16.};

// Clarity
double MTCI_KERNELS_CLARI[] = {-1 / 1., -1 / 1., -1 / 1., -1 / 1., 9 / 1.,
                               -1 / 1., -1 / 1., -1 / 1., -1 / 1.};

// Embossing
double MTCI_KERNELS_EMBOS[] = {-2 / 1., -1 / 1., 0 / 1., -1 / 1., 1 / 1.,
                               1 / 1.,  0 / 1.,  1 / 1., 2 / 1.};

// Borders
double MTCI_KERNELS_BORDE[] = {1 / 1.,  0 / 1., -1 / 1., 1 / 1., 0 / 1.,
                               -1 / 1., 1 / 1., 0 / 1.,  -1 / 1.};

#endif  // !SRC_LIB_MTCI_KERNELS_H