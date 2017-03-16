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
#include "uart.h"
   
#include "initialization.h"
#include "spi_master.h"
#include "common.h"

#include "radio_config.h"


//#include "nrf_drv_adc.h"
#include "nordic_common.h"


#define MAX_VALUE       255
#define SIZE_PACKET     6
#define ID_RF           0x2

/**
 * main function
 * \return 0. int return type required by ANSI/ISO standard. 
 */

void GPIOTE_IRQHandler(void);
void UART0_IRQHandler(void);

uint8_t encoder_plus = 0, encoder_minus = 0;
uint8_t count = 0;
uint8_t flag = 0;
uint8_t data[SIZE_PACKET];
int main(void)
{
  gpiote_init_encoder();
//  uart_config_encoder();
  radio_configure();
  NRF_POWER->DCDCEN = POWER_DCDCEN_DCDCEN_Disabled << POWER_DCDCEN_DCDCEN_Pos;
  NRF_POWER->TASKS_LOWPWR = 1;
  
  // Enable GPIOTE interrupt in Nested Vector Interrupt Controller
   NVIC_EnableIRQ(GPIOTE_IRQn);  
   
//  uart_putstring("Start ! \r\n");
 
   while (true)
   {
//     uart_putstring("encoder_plus : ");
//     itoac(encoder_plus, 0);
//     uart_putstring("\r\n");
//     
//     uart_putstring("encoder_minus : ");
//     itoac(encoder_minus, 0);
//     uart_putstring("\r\n");
//     
//     uart_putstring("encoder_count : ");
//     itoac(count, 0);
//     uart_putstring("\r\n");
     
       data[0] = 0x03;               // Set Length to 2 Bytes
       data[1] = 0xFF;
       data[2] = 0xFF;
       data[3] = 0xFF;
       data[4] = ID_RF;
       data[5] = count;
       
       rf_send(data);
     
     __WFI(); 
   }  
  
  
  
//  
//  /*********************
//   *    DECLARATION    *
//   *********************/
//  
//   volatile uint8_t data_to_send[SIZE_PACKET];
//   
//   volatile uint8_t count = 0;
//  
//   /*********************
//   *  INITIALIZATION    *
//   **********************/
//  
//   
//   
//
//
// 
//   NRF_POWER->DCDCEN = POWER_DCDCEN_DCDCEN_Disabled << POWER_DCDCEN_DCDCEN_Pos; // Disable internal DC/DC converter
//   NRF_POWER->TASKS_LOWPWR = 1; // enable low power mode
//   
//   gpiote_init();
//   
//   /**************************
//    *        MAIN LOOP       *
//    **************************/
////   nrf_gpio_pin_set(LED);
//   
//   while (true)
//   {     
//     __WFI(); 
//   }
}

/**
 *@}
 **/
void UART0_IRQHandler(void)
{
    
  if( (NRF_UART0->EVENTS_RXDRDY == 1) && (NRF_UART0->INTENSET & UART_INTENSET_RXDRDY_Msk))
    {
      NRF_UART0->EVENTS_RXDRDY = 0;
    
    }
  
}

void GPIOTE_IRQHandler(void)
{
  
//  uart_putstring("Inside GPIOT_IRQHandler\r\n");
  if ((NRF_GPIO->IN&0x00200000) == 0x00200000)
  {
//    uart_putstring("Test1\r\n");
    if (count < 255)
    {
        count += 1;
    }
    
//    encoder_plus = 1;
    nrf_gpio_pin_toggle(DEBUG_PIN);
  }
  else if ((NRF_GPIO->IN&0x00800000) == 0x00800000)
  {
//    uart_putstring("Test2\r\n");
   
    if (count > 0)
    {
       count -= 1;
    }
   //    encoder_minus = 1;
  }
  
  nrf_gpio_pin_toggle(DEBUG_PIN);

  // Event causing the interrupt must be cleared
  NRF_GPIOTE->EVENTS_PORT = 0;
}