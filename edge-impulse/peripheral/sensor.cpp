#include "sensor.h"
#include "inertial/bmi323_icm42670.h"
#include <string.h>
#include "../ei_main.h"

#define INERTIAL_AXIS_SAMPLED   6
/* Constant defines -------------------------------------------------------- */
#define CONVERT_G_TO_MS2 (9.80665f)
#define ACC_RAW_SCALING  (32767.5f)

#define ACC_SCALE_FACTOR (2.0f * CONVERT_G_TO_MS2) / ACC_RAW_SCALING
#define CONVERT_ADC_GYR (float)(250.0f / 32768.0f)

/* Private variables ------------------------------------------------------- */
static float imu_data[INERTIAL_AXIS_SAMPLED];
static float imu_data_bmi323[INERTIAL_AXIS_SAMPLED];

// Event flags
osEventFlagsId_t sensor_event = NULL;

/**
 * @brief Initialize the inertial sensor
 */
bool ei_inertial_init(void)
{
    int retval = bmi323_icm42670_init();

    return true;
}

/**
 * 
 */
float *ei_fusion_inertial_read_data_icm42670(int n_samples)
{
    xyz_accel_gyro_imu_s icm_data;

    memset(imu_data, 0, sizeof(imu_data));

    getICM42670PAllSensors(&icm_data);
    imu_data[0] = (float)icm_data.acc_x * ACC_SCALE_FACTOR;
    imu_data[1] = (float)icm_data.acc_y * ACC_SCALE_FACTOR;
    imu_data[2] = (float)icm_data.acc_z * ACC_SCALE_FACTOR;

    if ((n_samples > 3)) {
        imu_data[3] = (float)icm_data.gyro_x * CONVERT_ADC_GYR;
        imu_data[4] = (float)icm_data.gyro_y * CONVERT_ADC_GYR;
        imu_data[5] = (float)icm_data.gyro_z * CONVERT_ADC_GYR;
    }

    return imu_data;
}

/**
 * 
 */
float* ei_fusion_inertial_read_data_bmi323(int n_samples)
{
    xyz_accel_gyro_imu_s bmi_data;

    memset(imu_data_bmi323, 0, sizeof(imu_data_bmi323));

    getBMI323AllSensors(&bmi_data);
    imu_data_bmi323[0] = (float)bmi_data.acc_x * ACC_SCALE_FACTOR;
    imu_data_bmi323[1] = (float)bmi_data.acc_y * ACC_SCALE_FACTOR;
    imu_data_bmi323[2] = (float)bmi_data.acc_z * ACC_SCALE_FACTOR;

    if ((n_samples > 3)) {
        imu_data_bmi323[3] = (float)bmi_data.gyro_x * CONVERT_ADC_GYR;
        imu_data_bmi323[4] = (float)bmi_data.gyro_y * CONVERT_ADC_GYR;
        imu_data_bmi323[5] = (float)bmi_data.gyro_z * CONVERT_ADC_GYR;
    }

    return imu_data_bmi323;
}

__NO_RETURN void sensorThread (void *argument)
{

    for(;;) {

        samples_callback(imu_data, 3 * sizeof(float));
        // Sleep for a while to avoid busy waiting
        osDelay(100);
    }
}
