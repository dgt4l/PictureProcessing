#include <pthread.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <vector>

#include "../mtci/util/mtci_transform.h"

void print_help(char *exe_name) {
  printf(
    "USAGE: %s -i <input_filename> \\\n"
    "    -o <output_filename> \\\n"
    "    -t <blur,clarity,embossing,borders,upscaling,negative> \\\n"
    "    -j <jobs number>\n"
    "    -c <channel: {0 - red, 1 - green, 2 - blue}>\n",
    exe_name
  );
}

enum filter_type {
  FILTER_BLUR, FILTER_CLARITY, FILTER_EMBOSSING, FILTER_BORDERS,
  FILTER_UPSCALING, FILTER_NEGATIVE
};

int main(int argc, char **argv) {
  char *inp_filename = NULL;
  char *out_filename = NULL;
  Image inp_img;
  uint8_t channel;
  int threads_number = 0;
  enum filter_type filter;

  std::vector<std::pair<const char *, enum filter_type>> types_avail{
    {"blur", FILTER_BLUR},
    {"clarity", FILTER_CLARITY},
    {"embossing", FILTER_EMBOSSING},
    {"borders", FILTER_BORDERS},
    {"upscaling", FILTER_UPSCALING},
    {"negative", FILTER_NEGATIVE},
  };

  if (argc < 11) {
    print_help(argv[0]);
    printf("argc: %d\n", argc);
    
    return 1;
  }

  for (int idx = 1; idx != argc; idx += 2) {
    if (strcmp(argv[idx], "-c") == 0){
      char *end = NULL;
      channel = strtol(argv[idx + 1], &end, 10);
      if (channel != 0 &&
          channel != 1 &&
          channel != 2){
            printf("Error: Unknown channel\n");
          }
      if (end != argv[idx + 1] + strlen(argv[idx + 1])) {
        printf("Error: Invalid channel value\n");
        return 1;
      }
    } else if (strcmp(argv[idx], "-i") == 0) {
      inp_filename = argv[idx + 1];
      if (!inp_img.read(inp_filename, 0)) {
        printf("Error: Failed to read input filemane `%s`\n", inp_filename);
        return 1;
      }
    } else if (strcmp(argv[idx], "-o") == 0) {
      out_filename = argv[idx + 1];
    } else if (strcmp(argv[idx], "-j") == 0) {
      char *end = NULL;
      threads_number = strtol(argv[idx + 1], &end, 10);
      if (end != argv[idx + 1] + strlen(argv[idx + 1])) {
        printf("Error: Invalid number of threads!\n");
        return 1;
      }
    } else if (strcmp(argv[idx], "-t") == 0) {
      bool flag = false;
      for (auto &el : types_avail) {
        if (strcmp(el.first, argv[idx + 1]) == 0) {
          filter = el.second;
          flag = true;
          break;
        }
      }
      if (!flag) {
        printf("Error: Unknown filter type `%s`\navailble options:\n", argv[idx + 1]);
        for (auto &el : types_avail) {
          printf(" - %s\n", el.first);
        }
        return 1;
      }
    } else {
      printf("Unknown option: `%s`\n", argv[idx]);
      print_help(argv[0]);
      return 1;
    }
  }

   

  bool flag = false;
  switch (filter) {
    case FILTER_BLUR:
      flag = mt_apply_blur_filter(inp_img, threads_number, channel).write(out_filename);
      break;
    case FILTER_BORDERS:
      flag = mt_apply_borders_filter(inp_img, threads_number, channel).write(out_filename);
      break;
    case FILTER_CLARITY:
      flag = mt_apply_clarity_filter(inp_img, threads_number, channel).write(out_filename);
      break;
    case FILTER_EMBOSSING:
      flag = mt_apply_embossing_filter(inp_img, threads_number, channel).write(out_filename);
      break;
    case FILTER_NEGATIVE:
      flag = mt_apply_negative_filter(inp_img, threads_number, channel).write(out_filename);
      break;
    case FILTER_UPSCALING:
      flag = mt_apply_upscaling_filter(inp_img, threads_number, channel).write(out_filename);
      break;
  }
  if (!flag) {
    printf("Error: Unable to write output file: `%s`\n", out_filename);
    return 1;
  }
  return 0;
}
