#include <stdbool.h>
#include <stdint.h>

#include "initialization.h"
#include "adc.h"
#include "nrf.h"
#include "nrf51.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "nrf_gpiote.h"



void gpiote_init_encoder()
{ 
  nrf_gpio_cfg_output(BUCK_ON);            
  nrf_gpio_cfg_output(1);               // unused
  nrf_gpio_cfg_output(LED);             
  nrf_gpio_cfg_output(3);               // unused
  nrf_gpio_cfg_input(BAT_LVL, GPIO_PIN_CNF_PULL_Disabled);          
  nrf_gpio_cfg_output(5);               // unused
  nrf_gpio_cfg_output(BAT_LVL_ON);
  nrf_gpio_cfg_output(7);               // unused
  nrf_gpio_cfg_output(8);               // unused
  nrf_gpio_cfg_output(9);               // unused
  nrf_gpio_cfg_output(10);              // unused
  nrf_gpio_cfg_output(11);              // unused
  nrf_gpio_cfg_output(12);              // unused
  nrf_gpio_cfg_output(13);              // unused
  nrf_gpio_cfg_output(14);              // unused
  nrf_gpio_cfg_output(15);              // unused
  nrf_gpio_cfg_output(16);              // unused
  nrf_gpio_cfg_output(17);              // unused
  nrf_gpio_cfg_output(18);              // unused
  nrf_gpio_cfg_output(19);              // unused
  nrf_gpio_cfg_output(20);              // unused
  // SIGNAL A encoder
  nrf_gpio_cfg_output(22);              // unused
  // SIGNAL B encoder
  nrf_gpio_cfg_output(24);              // unused
  nrf_gpio_cfg_output(25);              // unused
  nrf_gpio_cfg_output(26);              // unused
  nrf_gpio_cfg_output(27);              // unused
  nrf_gpio_cfg_output(28);              // unused
  nrf_gpio_cfg_output(29);              // unused
  nrf_gpio_cfg_output(30);              // unused
  
  nrf_gpio_pin_set(BUCK_ON);            // turn off buck
  nrf_gpio_pin_clear(1);                // unused
  nrf_gpio_pin_clear(LED);              // turn off led
  nrf_gpio_pin_clear(3);                // unused
  // BAT_LVL is an input
  nrf_gpio_pin_clear(5);                // unused
  nrf_gpio_pin_clear(BAT_LVL_ON);       // turn off bat checking
  nrf_gpio_pin_clear(7);                // unused
  nrf_gpio_pin_clear(8);                // unused
  nrf_gpio_pin_clear(9);                // unused
  nrf_gpio_pin_clear(10);               // unused
  nrf_gpio_pin_clear(11);               // unused
  nrf_gpio_pin_clear(12);               // unused
  nrf_gpio_pin_clear(13);               // unused
  nrf_gpio_pin_clear(14);               // unused
  nrf_gpio_pin_clear(15);               // unused
  nrf_gpio_pin_clear(16);               // unused
  nrf_gpio_pin_clear(17);               // unused
  nrf_gpio_pin_clear(18);               // unused
  nrf_gpio_pin_clear(19);               // unused
  nrf_gpio_pin_clear(20);               // unused
  // SIGNAL A encoder
  nrf_gpio_pin_clear(22);               // unused
  // SIGNAL B encoder
  nrf_gpio_pin_clear(24);               // unused
  nrf_gpio_pin_clear(25);               // unused
  nrf_gpio_pin_clear(26);               // unused
  nrf_gpio_pin_clear(27);               // unused
  nrf_gpio_pin_clear(28);               // unused
  nrf_gpio_pin_clear(29);               // unused
  nrf_gpio_pin_clear(30);               // unused
 
  /* SIGNAL A&B init */
  
  NRF_GPIO->PIN_CNF[PIN_SIGNAL_A]=(GPIO_PIN_CNF_SENSE_High << GPIO_PIN_CNF_SENSE_Pos)
                                        | (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
                                        | (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)
                                        | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
                                        | (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos);
  
  NRF_GPIO->PIN_CNF[PIN_SIGNAL_B]=(GPIO_PIN_CNF_SENSE_High << GPIO_PIN_CNF_SENSE_Pos)
                                        | (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
                                        | (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)
                                        | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
                                        | (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos);
  
  NRF_GPIOTE->INTENSET = GPIOTE_INTENSET_PORT_Enabled << GPIOTE_INTENSET_PORT_Pos; // enable the device to power on after IDLE
}