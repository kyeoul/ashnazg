#include "nrfx_pwm.h"

#include "pwm.h"
#include "pcm_data.h"
#include "speaker.h"

#define MAXIMUM_SEQUENCE_LENGTH 32767

void speaker_play(file_name name)
{
  nrf_pwm_values_common_t sequence_data[MAXIMUM_SEQUENCE_LENGTH];
  for (int i = 0; i < MAXIMUM_SEQUENCE_LENGTH; i++)
  {
    int index;
    const int16_t *pcm_data;
    if (name == LERING_ACTIVATED)
    {
      pcm_data = pcm_data_lering_activated;
    }
    else if (name == RING_COMMAND) {
      pcm_data = pcm_data_ring_command;
    }
    else if (name == KEEP_WALKING) {
      pcm_data = pcm_data_keep_walking;
    }
    else if (name == GO_OTHER_WAY) {
      pcm_data = pcm_data_go_other_way;
    }
    else if (name == LERING) {
      pcm_data = pcm_data_lering;
    }
    
    if (name != ASHNAZG)
    {
      index = i;
    }
    else
    {
      index = i * 2;
    }

    sequence_data[i] = (uint16_t)(((pcm_data[index] + 32768) * PWM_TOP_VALUE) / 65535);
  }

  nrf_pwm_sequence_t sequence = {
      .values = {.p_common = sequence_data},
      .length = MAXIMUM_SEQUENCE_LENGTH,
      .repeats = 1,
      .end_delay = 0};

  pwm_write_sequence(SPEAKER_PWM_INSTANCE, sequence);
}