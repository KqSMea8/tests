//
// Created by Zhang,Tai on 17/9/23.
//

#ifndef CAPI_EXAMPLE_COMMON_H
#define CAPI_EXAMPLE_COMMON_H
#include <stdio.h>
#include <stdlib.h>

#define CHECK(stmt)                                                      \
  do {                                                                   \
    paddle_error __err__ = stmt;                                         \
    if (__err__ != kPD_NO_ERROR) {                                       \
      fprintf(stderr, "Invoke paddle error %d in " #stmt "\n", __err__); \
      exit(__err__);                                                     \
    }                                                                    \
  } while (0)

#endif
