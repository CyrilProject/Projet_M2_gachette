/* Copyright (c) 2009 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file
* @brief Example template project.
* @defgroup nrf_templates_example Example template
* @{
* @ingroup nrf_examples_nrf6310
*
* @brief Example template.
*
*/

#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "nrf.h"
#include "nrf51.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "nrf_gpiote.h"
   
#include "initialization.h"
#include "spi_master.h"
#include "common.h"

#include "radio_config.h"


//#include "nrf_drv_adc.h"
#include "nordic_common.h"


#define MAX_VALUE       255
#define SIZE_PACKET     15






/**
 * main function
 * \return 0. int return type required by ANSI/ISO standard. 
 */

void GPIOTE_IRQHandler(void);
void UART0_IRQHandler(void);


int main(void)
{
  /*********************
   *    DECLARATION    *
   *********************/
  
  
   volatile uint8_t data_to_send[SIZE_PACKET];
   
   volatile uint8_t count = 0;
  
   /*********************
   *  INITIALIZATION    *
   **********************/
  
   
   


 
   NRF_POWER->DCDCEN = POWER_DCDCEN_DCDCEN_Disabled << POWER_DCDCEN_DCDCEN_Pos; // Disable internal DC/DC converter
   NRF_POWER->TASKS_LOWPWR = 1; // enable low power mode
   
   gpiote_init();
   
   /**************************
    *        MAIN LOOP       *
    **************************/
//   nrf_gpio_pin_set(LED);
   
   while (true)
   {     
     __WFI(); 
   }
}

/**
 *@}
 **/
void UART0_IRQHandler(void)
{
  uint8_t data[4];
  
  data[0] = 0x02;               // Set Length to 2 Bytes
  data[1] = 0xFF;
  data[2] = uart_get();
  data[3] = uart_get();
  
  rf_send(data);
  
  if( (NRF_UART0->EVENTS_RXDRDY == 1) && (NRF_UART0->INTENSET & UART_INTENSET_RXDRDY_Msk))
    {
      NRF_UART0->EVENTS_RXDRDY = 0;
    
    }
  
}

