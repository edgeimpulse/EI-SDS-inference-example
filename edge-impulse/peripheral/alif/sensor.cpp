#include "sensor.h"
#include <string.h>
#include "../../ei_main.h"
#include <stdio.h>

#if USE_OLD_IMU == 1
#include "inertial/bmi323_icm42670.h"
#else
#include "Driver_IMU.h"
/* IMU Driver instance */
extern ARM_DRIVER_IMU BMI323;
static ARM_DRIVER_IMU *Drv_IMU = &BMI323;

#endif


#define INERTIAL_AXIS_SAMPLED   6
/* Constant defines -------------------------------------------------------- */
#define CONVERT_G_TO_MS2 (9.80665f)
#define CONVERT_mG_TO_MS2 (9.80665f/1000.0f)
#define ACC_RAW_SCALING  (32767.5f)

#define ACC_SCALE_FACTOR (16.0f * CONVERT_G_TO_MS2) / ACC_RAW_SCALING
#define CONVERT_ADC_GYR (float)(250.0f / 32768.0f)

/* Private variables ------------------------------------------------------- */
//static float imu_data[INERTIAL_AXIS_SAMPLED];
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

#if USE_OLD_IMU == 1
    if (bmi323_icm42670_init() != 0) {
        printf("\r\n Error: BMI323/ICM42670 initialization failed.\r\n");        
    }
#else
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
#endif
    imu_init = true;
    return true;
}

/**
 * 
 */
static void ei_fusion_inertial_read_data_bmi323(int n_samples)
{

    int32_t ret;

    memset(imu_data_bmi323, 0, sizeof(imu_data_bmi323));

#if USE_OLD_IMU == 1
    xyz_accel_gyro_imu_s bmi_data;

    getBMI323AllSensors(&bmi_data);

    imu_data_bmi323[0] = (float)bmi_data.acc_x * ACC_SCALE_FACTOR;
    imu_data_bmi323[1] = (float)bmi_data.acc_y * ACC_SCALE_FACTOR;
    imu_data_bmi323[2] = (float)bmi_data.acc_z * ACC_SCALE_FACTOR;
#else
    ARM_IMU_COORDINATES data;
    ARM_IMU_STATUS      status;
    float               temperature;

    Drv_IMU->Control(IMU_SET_INTERRUPT, true);  // mandatory ?

    /* Gets IMU status */
    do {
        status = Drv_IMU->GetStatus();

        if(status.drdy_status & IMU_ACCELEROMETER_DATA_READY) {
            /* Read Accelerometer data */
            ret = Drv_IMU->Control(IMU_GET_ACCELEROMETER_DATA,
                                    (uint32_t)&data);
            imu_data_bmi323[0] = (float)data.x * ACC_SCALE_FACTOR;
            imu_data_bmi323[1] = (float)data.y * ACC_SCALE_FACTOR;
            imu_data_bmi323[2] = (float)data.z * ACC_SCALE_FACTOR;
            break;
            }
        else {
            osDelay(1);
        }
    }while(1);

#endif
}

__NO_RETURN void sensorThread (void *argument)
{    
    for(;;) {

        ei_fusion_inertial_read_data_bmi323(3);

        samples_callback(imu_data_bmi323, 3 * sizeof(float));
      
        // Sleep for a while to avoid busy waiting
        osDelay(10);    // fixed for now
    }
}
