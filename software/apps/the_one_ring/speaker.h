#pragma once

#include "microbit_v2.h"

#define RING_SPEAKER_OUT EDGE_P12

typedef enum
{
  CANNOT_HIDE,
  ASHNAZG,
  LERING_ACTIVATED,
  LEBRON_GO_OTHER_WAY,
  LEBRON_KEEP_WALKING,
  LEBRON_RING_COMMAND,
  LERING,
} file_name;

void speaker_play(file_name);