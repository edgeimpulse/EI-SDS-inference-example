#include "sensor.h"
#include "Driver_IMU.h"
#include <string.h>
#include "../ei_main.h"
#include <stdio.h>
#include "Driver_I3C.h"

/* IMU Driver instance */
extern ARM_DRIVER_IMU BMI323;
static ARM_DRIVER_IMU *Drv_IMU = &BMI323;

/* i3c Driver */
extern ARM_DRIVER_I3C Driver_I3C;
static ARM_DRIVER_I3C *I3Cdrv = &Driver_I3C;

#define INERTIAL_AXIS_SAMPLED   6
/* Constant defines -------------------------------------------------------- */
#define CONVERT_G_TO_MS2 (9.80665f)
#define ACC_RAW_SCALING  (32767.5f)

#define ACC_SCALE_FACTOR (2.0f * CONVERT_G_TO_MS2) / ACC_RAW_SCALING
#define CONVERT_ADC_GYR (float)(250.0f / 32768.0f)

/* Private variables ------------------------------------------------------- */
static float imu_data[INERTIAL_AXIS_SAMPLED];
static float imu_data_bmi323[INERTIAL_AXIS_SAMPLED];

static void ei_fusion_inertial_read_data_icm42670(int n_samples);
static void ei_fusion_inertial_read_data_bmi323(int n_samples);

// Event flags
osEventFlagsId_t sensor_event = NULL;

static bool imu_init = false;

/**
 * @brief Initialize the inertial sensor
 */
bool ei_inertial_init(void)
{
    int32_t ret;

    #if (defined FAKE_DATA) && (FAKE_DATA == 1)
    printf("\r\n Fake data enabled, skipping IMU initialization.\r\n");
    imu_init = true;
    return true;
    #endif

    /* IMU initialization */
    ret = Drv_IMU->Initialize();
    if(ret != ARM_DRIVER_OK)
    {
        printf("\r\n Error: IMU Initialize failed.\r\n");
        return false;
    }

    /* IMU power up */
    ret = Drv_IMU->PowerControl(ARM_POWER_FULL);
    if(ret != ARM_DRIVER_OK)
    {
        printf("\r\n Error: IMU Power-up failed.\r\n");
        return false;
    }

    imu_init = true;
    return true;
}

#if (defined FAKE_DATA) && (FAKE_DATA == 1)
static const float fake_data[] =
{
5.9900, -4.1300, -2.7600, 8.2500, -3.6900, -3.0200, 10.7100, -4.0200, -2.0800, 10.7100, -4.0200, -2.0800, 11.7800, -4.2200, -1.4800, 10.1200, -4.5000, -2.0800, 8.2800, -4.6100, -2.7300, 11.0500, -3.9300, -2.9000, 14.5400, -4.2000, -1.8000, 16.0100, -4.2600, -0.5600, 16.0100, -4.2600, -0.5600, 16.1300, -3.5800, -0.9700, 17.2600, -3.1000, -2.1900, 18.1600, -3.4900, -1.5700, 18.2400, -3.4000, -1.0900, 17.3500, -3.1600, -1.4900, 17.3500, -3.1600, -1.4900, 15.7200, -3.0700, -0.9800, 14.1300, -3.6000, -0.3300, 12.7000, -4.2800, -0.1200, 11.3000, -4.5500, -0.5300, 10.5200, -4.6000, -0.8300, 9.7300, -5.3400, -0.2300, 9.7300, -5.3400, -0.2300, 9.8300, -5.9000, 0.1500, 9.7800, -5.3700, -0.3700, 8.9300, -4.7900, -1.0100, 7.6700, -4.4000, -1.2300, 6.3600, -4.0700, -1.6200, 5.2000, -4.8500, -1.4800, 5.2000, -4.8500, -1.4800, 4.6500, -5.5000, -1.4700, 5.6000, -5.4800, -0.7300, 5.5100, -5.8300, 0.4500, 3.9900, -5.9400, 0.7700, 2.5300, -5.0300, -0.0400, 0.5500, -4.4500, -1.2500, 0.5500, -4.4500, -1.2500, -1.4900, -4.8000, -1.5300, -3.1200, -5.3700, -1.4100, -3.6300, -5.5700, -1.4800, -4.0000, -5.9200, -0.8800, -5.5300, -6.2500, -0.8600, -6.3700, -6.2400, -1.7200, -6.3700, -6.2400, -1.7200, -6.9300, -6.1400, -2.8500, -6.9900, -6.1100, -2.9400, -6.6100, -5.9700, -2.0400, -6.7700, -6.4500, -1.3700, -6.4000, -6.5000, -2.4800, -5.8900, -5.4400, -4.1900, -5.8900, -5.4400, -4.1900, -7.0000, -4.5300, -5.8500, -8.0300, -4.4000, -6.1700, -7.0100, -5.5300, -4.7600, -6.0200, -6.3000, -3.4700, -8.0200, -5.8400, -3.7700, -9.7400, -5.4100, -5.3500, -9.7400, -5.4100, -5.3500, -10.4500, -5.9600, -5.3700, -11.4900, -6.7200, -4.5000, -12.9200, -7.0900, -3.9900, -14.7000, -6.8600, -4.1100, -16.0900, -7.6300, -4.1600, -16.0900, -7.6300, -4.1600, -16.7000, -7.8600, -3.8700, -17.1700, -7.5400, -3.3700, -18.8000, -6.7300, -4.1000, -18.4500, -5.6300, -5.7300, -16.8400, -5.3700, -5.8700, -14.2500, -4.9700, -4.9000, -14.2500, -4.9700, -4.9000, -12.0200, -4.8300, -4.3800, -10.6000, -4.0700, -5.0100, -10.8000, -3.8500, -5.8500, -10.8200, -3.8500, -5.4100, -9.6900, -4.0000, -3.7300, -8.4200, -3.5600, -2.9500, -8.4200, -3.5600, -2.9500, -7.4800, -3.6900, -3.2600, -5.8900, -3.8400, -2.9800, -5.2600, -3.6000, -2.5500, -5.4100, -2.6100, -3.5400, -5.5000, -2.3300, -4.5300, -5.9300, -2.9000, -3.7500, -5.9300, -2.9000, -3.7500, -4.1700, -2.9400, -3.3900, -1.7400, -3.7400, -1.5700, 0.8900, -4.5100, 0.0300, 0.3500, -3.8800, -0.4400, -1.0900, -2.7600, -3.4500, -0.1200, -2.7400, -3.7200, -0.1200, -2.7400, -3.7200, 1.9500, -3.6700, -2.6300, 3.0400, -3.6600, -0.0700, 4.7000, -4.3000, 0.9700, 5.5800, -3.4100, 0.6900, 4.5800, -3.0700, -0.9100, 6.6300, -2.2200, -1.0200, 6.6300, -2.2200, -1.0200, 9.1300, -2.0900, 0.8700, 9.9100, -2.6500, 1.9000, 11.1600, -2.3900, 0.7900, 13.1200, -1.6300, -1.2800, 16.5300, -1.7800, -1.2900, 18.8900, -2.8800, 0.5800, 18.8900, -2.8800, 0.5800, 17.8900, -3.3300, 0.7100, 17.0200, -2.2800, -1.6300, 18.5300, -2.0000, -1.9700, 19.8700, -2.5700, -0.8300, 18.4500, -2.7200, -0.0100, 18.4500, -2.7200, -0.0100, 15.4100, -1.6800, -1.6500, 13.2500, -0.9800, -3.1200, 13.7700, -0.7400, -2.3100, 14.4300, -1.2900, -0.4900, 14.5800, -0.7900, -0.5000, 14.0300, 0.3100, -2.4500, 14.0300, 0.3100, -2.4500, 13.6400, 0.8100, -3.2800, 14.2000, 0.9000, -2.7700, 14.6900, -0.0300, -1.5300, 12.9000, -0.9800, -0.4300, 11.1600, -1.0100, -0.9400, 9.5000, -1.2000, -1.8000, 8.9300, -1.3000, -2.0500, 8.9300, -1.3000, -2.0500, 7.9700, -1.4000, -2.6000, 7.9700, -1.4000, -2.6000, 6.8700, -1.5000, -2.7000, 6.8700, -1.5000, -2.7000
};


static int fake_data_index = 0;

#endif

/**
 * 
 */
static void ei_fusion_inertial_read_data_bmi323(int n_samples)
{
    ARM_IMU_COORDINATES data;
    ARM_IMU_STATUS      status;
    int32_t ret;

    memset(imu_data_bmi323, 0, sizeof(imu_data_bmi323));

#if (defined FAKE_DATA) && (FAKE_DATA == 1)
    if ( (fake_data_index + n_samples) > (sizeof(fake_data) / sizeof(fake_data[0]))) {
        fake_data_index = 0; // Reset index if it exceeds the array size
    }

    for (int i = 0; i < n_samples; i++) {
        imu_data_bmi323[i] = fake_data[fake_data_index++];
    }
#else
    /* Gets IMU status */
    status = Drv_IMU->GetStatus();

    if(status.drdy_status & IMU_ACCELEROMETER_DATA_READY) {
        /* Read Accelerometer data */
        ret = Drv_IMU->Control(IMU_GET_ACCELEROMETER_DATA,
                                   (uint32_t)&data);
        imu_data_bmi323[0] = (float)data.x * ACC_SCALE_FACTOR;
        imu_data_bmi323[1] = (float)data.y * ACC_SCALE_FACTOR;
        imu_data_bmi323[2] = (float)data.z * ACC_SCALE_FACTOR;
    }
#endif
}

__NO_RETURN void sensorThread (void *argument)
{    
    for(;;) {

        ei_fusion_inertial_read_data_bmi323(3);

        samples_callback(imu_data, 3 * sizeof(float));
        // Sleep for a while to avoid busy waiting
        osDelay(10);    // fixed for now
    }
}
