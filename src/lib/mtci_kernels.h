#ifndef SRC_LIB_MTCI_KERNELS_H
#define SRC_LIB_MTCI_KERNELS_H

const double MTCI_KERNELS_NONE[] = {1 / 1., 0 / 1., 0 / 1., 0 / 1., 1 / 1.,
                                    0 / 1., 0 / 1., 0 / 1., 1 / 1.};
const double MTCI_KERNELS_GAUSS[] = {1 / 16., 2 / 16., 1 / 16.,
                                     2 / 16., 4 / 16., 2 / 16.,
                                     1 / 16., 2 / 16., 1 / 16.};

#endif  // !SRC_LIB_MTCI_KERNELS_H