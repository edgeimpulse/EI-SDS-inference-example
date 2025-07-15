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

#ifdef  __cplusplus
}
#endif

#endif