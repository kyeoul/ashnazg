#pragma once

#include "microbit_v2.h"

#define RING_SPEAKER_OUT EDGE_P1

typedef enum {
  CANNOT_HIDE,
  ASHNAZG
} file_name;

void speaker_play(file_name);