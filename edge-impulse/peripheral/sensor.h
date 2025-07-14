#ifndef _SENSOR_H_
#define _SENSOR_H_

#include <stdint.h>

bool ei_inertial_init(void);
float *ei_fusion_inertial_read_data_icm42670(int n_samples);
float* ei_fusion_inertial_read_data_bmi323(int n_samples);

#endif