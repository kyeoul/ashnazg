#define TRIG_OUTPUT EDGE_P3
#define ECHO_INPUT EDGE_P4

void distance_sensor_init();

float distance_measure_blocking();

void start_sensor();

uint32_t read_echo_output();