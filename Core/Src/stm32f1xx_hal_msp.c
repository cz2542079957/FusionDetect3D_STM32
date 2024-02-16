
// #include "main.h"

// void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
// /**
//  * Initializes the Global MSP.
//  */
// void HAL_MspInit(void)
// {
//     /* USER CODE BEGIN MspInit 0 */

//     /* USER CODE END MspInit 0 */

//     __HAL_RCC_AFIO_CLK_ENABLE();
//     __HAL_RCC_PWR_CLK_ENABLE();

//     /* System interrupt init*/

//     /** NOJTAG: JTAG-DP Disabled and SW-DP Enabled
//      */
//     __HAL_AFIO_REMAP_SWJ_NOJTAG();

//     /* USER CODE BEGIN MspInit 1 */

//     /* USER CODE END MspInit 1 */
// }

// /**
//  * @brief UART MSP Initialization
//  * This function configures the hardware resources used in this example
//  * @param huart: UART handle pointer
//  * @retval None
//  */
// void HAL_UART_MspInit(UART_HandleTypeDef *huart)
// {
//     GPIO_InitTypeDef GPIO_InitStruct = {0};
//     if (huart->Instance == USART1)
//     {
//         /* USER CODE BEGIN USART1_MspInit 0 */

//         /* USER CODE END USART1_MspInit 0 */
//         /* Peripheral clock enable */
//         __HAL_RCC_USART1_CLK_ENABLE();

//         __HAL_RCC_GPIOA_CLK_ENABLE();
//         /**USART1 GPIO Configuration
//         PA9     ------> USART1_TX
//         PA10     ------> USART1_RX
//         */
//         GPIO_InitStruct.Pin = GPIO_PIN_9;
//         GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//         GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//         HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

//         GPIO_InitStruct.Pin = GPIO_PIN_10;
//         GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//         GPIO_InitStruct.Pull = GPIO_NOPULL;
//         HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

//         /* USART1 DMA Init */
//         /* USART1_TX Init */
//         hdma_usart1_tx.Instance = DMA1_Channel4;
//         hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
//         hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
//         hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
//         hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
//         hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
//         hdma_usart1_tx.Init.Mode = DMA_NORMAL;
//         hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
//         if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
//         {
//             Error_Handler();
//         }

//         __HAL_LINKDMA(huart, hdmatx, hdma_usart1_tx);

//         /* USART1 interrupt Init */
//         HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
//         HAL_NVIC_EnableIRQ(USART1_IRQn);
//         /* USER CODE BEGIN USART1_MspInit 1 */

//         /* USER CODE END USART1_MspInit 1 */
//     }
// }

// /**
//  * @brief UART MSP De-Initialization
//  * This function freeze the hardware resources used in this example
//  * @param huart: UART handle pointer
//  * @retval None
//  */
// void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
// {
//     if (huart->Instance == USART1)
//     {
//         /* USER CODE BEGIN USART1_MspDeInit 0 */

//         /* USER CODE END USART1_MspDeInit 0 */
//         /* Peripheral clock disable */
//         __HAL_RCC_USART1_CLK_DISABLE();

//         /**USART1 GPIO Configuration
//         PA9     ------> USART1_TX
//         PA10     ------> USART1_RX
//         */
//         HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);

//         /* USART1 DMA DeInit */
//         HAL_DMA_DeInit(huart->hdmatx);

//         /* USART1 interrupt DeInit */
//         HAL_NVIC_DisableIRQ(USART1_IRQn);
//         /* USER CODE BEGIN USART1_MspDeInit 1 */

//         /* USER CODE END USART1_MspDeInit 1 */
//     }
// }

// /* USER CODE BEGIN 1 */

// /* USER CODE END 1 */
