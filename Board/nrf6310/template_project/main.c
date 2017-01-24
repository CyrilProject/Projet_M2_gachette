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


#define MAX_VALUE 255
#define SIZE_PACKET 15






/**
 * main function
 * \return 0. int return type required by ANSI/ISO standard. 
 */

void GPIOTE_IRQHandler(void);



int main(void)
{
/** GPIOTE interrupt handler.
* Triggered on motion interrupt pin input low-to-high transition.
*/
//   NRF_POWER->DCDCEN=POWER_DCDCEN_DCDCEN_Disabled<<POWER_DCDCEN_DCDCEN_Pos;
//   NRF_POWER->TASKS_LOWPWR = 1;
//   gpiote_init();


  /*********************
   *    DECLARATION    *
   *********************/
  
  
   volatile uint8_t data_to_send[SIZE_PACKET];
   volatile uint8_t flag_A = 0;
   volatile uint8_t flag_B = 0;
   volatile uint8_t count = 0;
  
   /*********************
   *  INITIALIZATION    *
   **********************/
  
   gpiote_init();


 
   
   NRF_POWER->DCDCEN = POWER_DCDCEN_DCDCEN_Disabled << POWER_DCDCEN_DCDCEN_Pos; // Disable internal DC/DC converter
   NRF_POWER->TASKS_LOWPWR = 1; // enable low power mode
   
   
   // Enable GPIOTE interrupt in Nested Vector Interrupt Controller
   NVIC_EnableIRQ(GPIOTE_IRQn);

   

   
   
   /**************************
    *        MAIN LOOP       *
    **************************/
   while (true)
   {     
     __WFI(); // idle mode
   }
}

/**
 *@}
 **/

void GPIOTE_IRQHandler(void)
{
  volatile uint8_t flag_A, flag_B, count;
  
  // Interruption on the pin for signal A
  if (NRF_GPIOTE->EVENTS_IN[0] == 1)
  {
    
    flag_A = 1;
    if( flag_B == 0 )
    {
      if ( count < MAX_VALUE)
      {
        
        count += 1;
      }
    }
    else 
    {
      flag_B = 0;
    }
    
    NRF_GPIOTE->EVENTS_IN[0] = 0;
  }
  else 
  {
    // Interruption on the pin for signal B
    
    flag_B = 1;
    if( flag_A == 0 )
    {
      if ( count > 0)
      {
        
        count -= 1;
      }
    }
    else 
    {
      flag_A = 0;
    }
    
    NRF_GPIOTE->EVENTS_IN[1] = 0;
  }
      
      
    
    
  
  // Event causing the interrupt must be cleared
  
  NVIC_DisableIRQ(GPIOTE_IRQn);
  
}



