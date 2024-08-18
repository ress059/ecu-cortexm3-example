/**
 * @file
 * @brief !!!!!!!!!!!!!!!!!!!!! TODO
 * 
 * @author Ian Ress
 * @version 0.1
 * @date 2024-07-27
 * @copyright Copyright (c) 2024
 */



/*------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------- INCLUDES ------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------*/

#include <led/led_fsm.h>



/*------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------- MAIN --------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------*/

int main(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  // expands to:
  //do { \
                                                 __IO uint32_t tmpreg; \
                                                 SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOBEN); \
                                                 /* Delay after an RCC peripheral clock enabling */ \
                                                 tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOBEN); \
                                                 UNUSED(tmpreg); \
                                               } while(0)

//   /*Configure GPIO pin Output Level */
//   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);

//   /*Configure GPIO pin : PB3 */
//   GPIO_InitStruct.Pin = GPIO_PIN_3;
//   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//   GPIO_InitStruct.Pull = GPIO_NOPULL;
//   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//   HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    while (1)
    {
        /* TODO: */
    }
    return 0;
}
