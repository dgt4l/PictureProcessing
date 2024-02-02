#include <pthread.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <vector>

#include "util/mtci_transform.h"

pthread_mutex_t file_mutex;

FILE *f = fopen("hui.log", "w");


int main() {
  char path[] = "../materials/road.jpg";
  Image img1(path), img2(path), img3(path);
  
  auto begin_mp = std::chrono::steady_clock::now();
  img1.write("../materials/output_mp.png");
  auto end_mp = std::chrono::steady_clock::now();
  auto elapsed_ms_mp =
      std::chrono::duration_cast<std::chrono::milliseconds>(end_mp - begin_mp);
  auto begin = std::chrono::steady_clock::now();
  img2.convolve_clamp_to_border(0, 3, 3, kernel, 1, 1);
  img2.write("../materials/output_default.png");
  auto end = std::chrono::steady_clock::now();
  auto elapsed_ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
  std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
  std::cout << "The mp time: " << elapsed_ms_mp.count() << " ms\n";
  std::cout << "The default time: " << elapsed_ms.count() << " ms\n";
}
