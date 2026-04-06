#pragma once


#include "stm32f4xx_ll_fmc.h"

enum HAL_SRAM_StateTypeDef
{
  HAL_SRAM_STATE_RESET     = 0x00U,  /*!< SRAM not yet initialized or disabled           */
  HAL_SRAM_STATE_READY     = 0x01U,  /*!< SRAM initialized and ready for use             */
  HAL_SRAM_STATE_BUSY      = 0x02U,  /*!< SRAM internal process is ongoing               */
  HAL_SRAM_STATE_ERROR     = 0x03U,  /*!< SRAM error state                               */
  HAL_SRAM_STATE_PROTECTED = 0x04U   /*!< SRAM peripheral NORSRAM device write protected */
  
};



typedef struct
{
    FMC_NORSRAM_TypeDef           *Instance;  /*!< Register base address                        */

    FMC_NORSRAM_EXTENDED_TypeDef  *Extended;  /*!< Extended mode register base address          */

    FMC_NORSRAM_InitTypeDef       Init;       /*!< SRAM device control configuration parameters */

    HAL_LockTypeDef               Lock;       /*!< SRAM locking object                          */

    __IO HAL_SRAM_StateTypeDef    State;      /*!< SRAM device access state                     */

    DMA_HandleTypeDef             *hdma;      /*!< Pointer DMA handler                          */

}SRAM_HandleTypeDef;


HAL_StatusTypeDef HAL_SRAM_Init(SRAM_HandleTypeDef *hsram, FMC_NORSRAM_TimingTypeDef *Timing, FMC_NORSRAM_TimingTypeDef *ExtTiming);
