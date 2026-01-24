

#define NUM_SENSORS 6

#include <time.h>

typedef struct soil_moisture_sensor {
    char id[3];
    float moisture_level; 
    float threshold;
    short unsigned int signal_pin;
    time_t last_water_time; // initialize this with an old date
} soil_moisture_sensor_t;

typedef struct sensor_group {
    soil_moisture_sensor_t sensors[2];
    short unsigned int power_pin;
    time_t last_read_time; // initialize this with an old date
} sensor_group_t;

typedef struct soil_moisture_system {
    sensor_group_t groups[NUM_SENSORS / 2];
} soil_moisture_system_t;

void sensing(void);
