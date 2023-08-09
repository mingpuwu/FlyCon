/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "stdint.h"
#include "led.h"
#include "oled.h"
#include "mpu6050.h"
#include "my_delay.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "imu_view.h"
#include "motor.h"
#include "control.h"
#include "mpu6050.h"
#include "bmp280.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
extern UART_HandleTypeDef huart1;
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
    /* USER CODE BEGIN 1 */
    float pitch, roll, yaw;
    short aacx, aacy, aacz;
    short gyrox, gyroy, gyroz;
    // short temp;
    struct bmp280_config conf;
    struct bmp280_uncomp_data ucomp_data;
    int32_t temp32;
    uint32_t pres32, pres64;
    double temp;
    double pres;
    int8_t rslt;
    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_I2C2_Init();
    MX_SPI1_Init();
    MX_USART1_UART_Init();
    MX_TIM14_Init();
    MX_I2C1_Init();
    /* USER CODE BEGIN 2 */
    OLED_Init();
    OLED_Display_On();
    OLED_Clear();
    HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);
    __HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, 1950);
    while (MPU_Init())
    {
    }
    printf("%s\r\n", "MPU_Init");
    while (mpu_dmp_init())
    {
        delay_ms(200);
        printf("%s\r\n", "Mpu6050 Init Wrong!");
    }
    printf("%s\r\n", "Mpu6050 Init OK!");

    rslt = bmp280_init();
    printf("bmp280_init status %d\r\n", rslt);

    /* Always read the current settings before writing, especially when
     * all the configuration is not modified
     */
    rslt = bmp280_get_config(&conf);
    printf("bmp280_get_config status %d\r\n", rslt);

    /* configuring the temperature oversampling, filter coefficient and output data rate */
    /* Overwrite the desired settings */
    conf.filter = BMP280_FILTER_COEFF_2;

    /* Temperature oversampling set at 4x */
    conf.os_temp = BMP280_OS_4X;

    /* Pressure over sampling none (disabling pressure measurement) */
    conf.os_pres = BMP280_OS_NONE;

    /* Setting the output data rate as 1HZ(1000ms) */
    conf.odr = BMP280_ODR_1000_MS;
    rslt = bmp280_set_config(&conf);
    printf("bmp280_set_config status %d\r\n", rslt);

    /* Always set the power mode after setting the configuration */
    rslt = bmp280_set_power_mode(BMP280_NORMAL_MODE);
    printf("bmp280_set_power_mode status %d\r\n", rslt);
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        // rslt = bmp280_get_uncomp_data(&ucomp_data);
        // rslt = bmp280_get_comp_temp_double(&temp, ucomp_data.uncomp_temp);
        // rslt = bmp280_get_comp_pres_double(&pres, ucomp_data.uncomp_press);
        // printf("T32: %f, P32 %lf, %d\r\n", temp, pres, rslt);
        if (mpu_dmp_get_data(&pitch, &roll, &yaw) == 0)
        {
            temp = MPU_Get_Temperature();
            float tempf = temp / 100;
            MPU_Get_Accelerometer(&aacx, &aacy, &aacz);
            MPU_Get_Gyroscope(&gyrox, &gyroy, &gyroz);
            MapPitchToContorl(pitch);
            //OLED_ShowImu(pitch, roll, yaw);
            // Test_Send_User1(aacx, aacy, aacz, gyrox, gyroy, gyroz, roll, pitch, yaw);
            Vofa_view(roll, pitch, yaw);
            //delay_ms(10);
        }
        delay_ms(10);
        // LedBlink();
    }
    /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
    {
        Error_Handler();
    }
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_I2C1;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
    PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
