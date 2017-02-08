#include <stdbool.h>
#include <stdint.h>

#include "initialization.h"
#include "adc.h"
#include "nrf.h"
#include "nrf51.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "nrf_gpiote.h"






 void gpiote_init(void)
{
 

//  *(uint32_t *)0x40000504 = 0xC007FFDF; // Workaround for PAN_028 rev1.1 anomaly 23 - System: Manual setup is required to enable use of peripherals

//  nrf_gpio_cfg_input(6,NRF_GPIO_PIN_NOPULL);
  // Configure GPIOTE channel BUUTTON to generate event when MOTION_INTERRUPT_PIN_NUMBER goes from Low to High
//
//  NRF_GPIO->PIN_CNF[PIN_SIGNAL_A]=(GPIO_PIN_CNF_SENSE_Low << GPIO_PIN_CNF_SENSE_Pos) //GPIO_PIN_CNF_SENSE_Low
//                                        | (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
//                                        | (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)
//                                        | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
//                                        | (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos);
//  
//  NRF_GPIO->PIN_CNF[PIN_SIGNAL_B]=(GPIO_PIN_CNF_SENSE_Low << GPIO_PIN_CNF_SENSE_Pos) //GPIO_PIN_CNF_SENSE_Low
//                                        | (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
//                                        | (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)
//                                        | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
//                                        | (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos);
  
//  nrf_gpio_cfg_input(PIN_SIGNAL_A, NRF_GPIO_PIN_NOPULL);
//  nrf_gpio_cfg_input(PIN_SIGNAL_B, NRF_GPIO_PIN_NOPULL);
//  
//  nrf_gpiote_event_config(0, PIN_SIGNAL_A, NRF_GPIOTE_POLARITY_LOTOHI);
//  nrf_gpiote_event_config(1, PIN_SIGNAL_B, NRF_GPIOTE_POLARITY_LOTOHI);

  nrf_gpio_cfg_output(UART_TX);
  nrf_gpio_cfg_input(UART_RX, NRF_GPIO_PIN_NOPULL);  

  NRF_UART0->PSELTXD = UART_TX;
  NRF_UART0->PSELRXD = UART_RX;


  // Enable interrupt for NRF_GPIOTE->EVENTS_IN[0] event
  //NRF_GPIOTE->INTENSET = GPIOTE_INTENSET_IN0_Enabled<<GPIOTE_INTENSET_PORT_Pos;
  
//NRF_GPIOTE->INTENSET = (GPIOTE_INTENSET_PORT_Enabled << GPIOTE_INTENSET_PORT_Pos); //&& (GPIOTE_INTENSET_IN1_Enabled << GPIOTE_INTENSET_IN0_Pos);
                                        
//nrf_gpio_cfg_output(LED2);
//nrf_gpio_pin_clear(LED2);
  nrf_gpio_cfg_output(1);
  nrf_gpio_cfg_output(2);
  nrf_gpio_cfg_output(3);
  nrf_gpio_cfg_output(4);
  nrf_gpio_cfg_output(5);
//  nrf_gpio_cfg_output(6);
//  nrf_gpio_cfg_output(7);
  nrf_gpio_cfg_output(12);
  nrf_gpio_cfg_output(13);
  nrf_gpio_cfg_output(14);
  nrf_gpio_cfg_output(15);
  nrf_gpio_cfg_output(16);
  nrf_gpio_cfg_output(17);
  nrf_gpio_cfg_output(20);
  nrf_gpio_cfg_output(21);
  nrf_gpio_cfg_output(22);
  nrf_gpio_cfg_output(23);
  nrf_gpio_cfg_output(24);
  nrf_gpio_cfg_output(25);
  nrf_gpio_cfg_output(26);
  nrf_gpio_cfg_output(27);
  nrf_gpio_cfg_output(28);
  nrf_gpio_cfg_output(29);
  nrf_gpio_cfg_output(30);
  //nrf_gpio_pin_clear(1);
  nrf_gpio_pin_clear(2);
  //nrf_gpio_pin_clear(3);
  nrf_gpio_pin_clear(4);
  nrf_gpio_pin_clear(5);
//  nrf_gpio_pin_clear(6);
//  nrf_gpio_pin_clear(7);
  nrf_gpio_pin_clear(12);
  nrf_gpio_pin_clear(13);
  nrf_gpio_pin_clear(14);
  nrf_gpio_pin_clear(15);
  nrf_gpio_pin_clear(16);
  nrf_gpio_pin_clear(17);
  nrf_gpio_pin_clear(20);
  nrf_gpio_pin_clear(21);
  nrf_gpio_pin_clear(22);
  nrf_gpio_pin_clear(23);
  nrf_gpio_pin_clear(24);
  nrf_gpio_pin_clear(25);
  nrf_gpio_pin_clear(26);
  nrf_gpio_pin_clear(27);
  nrf_gpio_pin_clear(28);
  nrf_gpio_pin_clear(29);
  nrf_gpio_pin_clear(30);
  
  nrf_gpio_cfg_output(LED);
  nrf_gpio_pin_clear(LED);
//nrf_gpio_cfg_output(PIN_BUCK);
//nrf_gpio_pin_clear(PIN_BUCK); // we will change it when the consuption will be ok
}

