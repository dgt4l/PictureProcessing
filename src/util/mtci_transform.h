#ifndef SRC_UTIL_MTCI_TRANSFORM_H
#define SRC_UTIL_MTCI_TRANSFORM_H

#include <functional>

#include "../lib/mtci_image.h"
#include "mtci_kernel.h"

class Transform {
 private:
  Kernel kernel;
  std::function<bool(Kernel, Image)>* apply;

 public:
  Transform(Kernel _kernel, std::function<bool(Kernel, Image)>* _apply) {
    kernel = _kernel, apply = _apply;
  };
  bool applyTransform(Image img);
};

#endif  // !SRC_UTIL_MTCI_TRANSFORM_H