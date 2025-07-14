#ifndef _SENSOR_H_
#define _SENSOR_H_

#include <stdint.h>
#include "cmsis_os2.h"

#ifdef  __cplusplus
extern "C"
{
#endif

extern void sensorThread (void *argument);
extern bool ei_inertial_init(void);
extern float *ei_fusion_inertial_read_data_icm42670(int n_samples);
extern float* ei_fusion_inertial_read_data_bmi323(int n_samples);

#ifdef  __cplusplus
}
#endif

#endif