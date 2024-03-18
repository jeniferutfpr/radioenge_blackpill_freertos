#include "cmsis_os.h"
#include "stm32f4xx_hal.h"
#include "radioenge_modem.h"
#include "uartRingBufDMA.h"

extern osTimerId_t PeriodicSendTimerHandle;
extern osThreadId_t AppSendTaskHandle;
extern ADC_HandleTypeDef hadc1;
extern osEventFlagsId_t ModemStatusFlagsHandle;
extern TIM_HandleTypeDef htim3;

extern osMessageQueueId_t TemperatureQueueHandle;
char msg[256];
void ReadFromADCTaskCode(void *argument)
{
    uint32_t read;
    int32_t temp_oCx100;
    while (1)
    {
        // read LM35 Temperature
        HAL_ADC_Start(&hadc1);
        HAL_ADC_PollForConversion(&hadc1, 100);
        read = HAL_ADC_GetValue(&hadc1);
        temp_oCx100 = (int32_t)(33000 * ((float)read / 4096));
        // Send Message
        osMessageQueuePut(TemperatureQueueHandle, &temp_oCx100, 0U,
                          osWaitForever);
        osDelay(200);
    }
}
void AppSendTaskCode(void *argument)
{
    uint32_t read;
    int32_t temp_oCx100;
    osStatus_t status;
    while (1)
    {
        status = osMessageQueueGet(TemperatureQueueHandle, &temp_oCx100,
                                   NULL, osWaitForever); // wait for message
        if (status == osOK)
        {
            sprintf(msg, "Temperature: %d.%d oC\n", temp_oCx100 / 100,
                    temp_oCx100 % 100);
            SendToUART(msg, strlen(msg));
        }
    }
}

void LoRaWAN_RxEventCallback(uint8_t *data, uint32_t length, uint32_t port, int32_t rssi, int32_t snr)
{
}

void PeriodicSendTimerCallback(void *argument)
{
}

//void AppSendTaskCode(void *argument)
//{
    /* USER CODE BEGIN 5 */
    /* Infinite loop */

  //  while (1)
    //{
      //  LoRaWaitDutyCycle();
        // write code to read from sensors and send via LoRaWAN

        //osDelay(30000);
    //}
//}
