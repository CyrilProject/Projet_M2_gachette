
#include "adc.h"
#include "nrf_gpio.h"


void init_adc()
{
  //nrf_gpio_cfg_input(PIN_ADC,NRF_GPIO_PIN_NOPULL);
  NRF_ADC->CONFIG=(ADC_CONFIG_RES_8bit << ADC_CONFIG_RES_Pos)
                                        | (ADC_CONFIG_INPSEL_AnalogInputNoPrescaling << ADC_CONFIG_INPSEL_Pos)
                                        | (ADC_CONFIG_REFSEL_VBG << ADC_CONFIG_REFSEL_Pos)
                                        | (ADC_CONFIG_PSEL_AnalogInput5<< ADC_CONFIG_PSEL_Pos)
                                        | (ADC_CONFIG_EXTREFSEL_None << ADC_CONFIG_EXTREFSEL_Pos); 
  nrf_gpio_cfg_output(PIN_ADC_ON);
  nrf_gpio_pin_clear(PIN_ADC_ON);
  NRF_ADC->ENABLE=ADC_ENABLE_ENABLE_Enabled<<ADC_ENABLE_ENABLE_Pos;
}

uint8_t start_sampling()
{
  nrf_gpio_pin_set(PIN_ADC_ON);
  __NOP();//ad a delay
  while(NRF_ADC->BUSY);
  NRF_ADC->TASKS_START=1U;
  NRF_ADC->EVENTS_END=0U;
  while(NRF_ADC->EVENTS_END==0U)
  {
  }
  nrf_gpio_pin_clear(PIN_ADC_ON);
  return (uint8_t) (0x0000FF&NRF_ADC->RESULT); 
}