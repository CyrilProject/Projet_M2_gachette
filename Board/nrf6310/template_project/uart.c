#include <stdint.h>

#include "nrf.h"
#include "uart.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"



uint8_t uart_get(void)
{
  while (NRF_UART0->EVENTS_RXDRDY != 1)
  {
    // Wait for RXD data to be received
  }
  
  NRF_UART0->EVENTS_RXDRDY = 0;
  return (uint8_t)NRF_UART0->RXD;
}

bool uart_get_with_timeout(int32_t timeout_ms, uint8_t *rx_data)
{
  bool ret = true;
  
  while (NRF_UART0->EVENTS_RXDRDY != 1)
  {
    if (timeout_ms-- >= 0)
    {
      // wait in 1ms chunk before checking for status
      nrf_delay_us(1000);
    }
    else
    {
      ret = false;
      break;
    }
  }  // Wait for RXD data to be received

  if (timeout_ms >= 0)
  {
    // clear the event and set rx_data with received byte
      NRF_UART0->EVENTS_RXDRDY = 0;
      *rx_data = (uint8_t)NRF_UART0->RXD;
  }

  return ret;
}

void simple_uart_put(uint8_t cr)
{
  NRF_UART0->TXD = (uint8_t)cr;

  while (NRF_UART0->EVENTS_TXDRDY!=1)
  {
    // Wait for TXD data to be sent
  }

  NRF_UART0->EVENTS_TXDRDY=0;
}

void uart_putstring(const uint8_t *str)
{
  uint_fast8_t i = 0;
  uint8_t ch = str[i++];
  while (ch != '\0')
  {
    simple_uart_put(ch);
    ch = str[i++];
  }
}

void uart_config()
{
//  nrf_gpio_cfg_output(txd_pin_number);
//  nrf_gpio_cfg_input(rxd_pin_number, NRF_GPIO_PIN_NOPULL);  
//
//  NRF_UART0->PSELTXD = txd_pin_number;
//  NRF_UART0->PSELRXD = rxd_pin_number;
//
//  if (hwfc)
//  {
//    nrf_gpio_cfg_output(rts_pin_number);
//    nrf_gpio_cfg_input(cts_pin_number, NRF_GPIO_PIN_NOPULL);
//    NRF_UART0->PSELCTS = cts_pin_number;
//    NRF_UART0->PSELRTS = rts_pin_number;
//    NRF_UART0->CONFIG  = (UART_CONFIG_HWFC_Enabled << UART_CONFIG_HWFC_Pos);
//  }

  NRF_UART0->BAUDRATE         = (UART_BAUDRATE_BAUDRATE_Baud115200<< UART_BAUDRATE_BAUDRATE_Pos);
  NRF_UART0->ENABLE           = (UART_ENABLE_ENABLE_Enabled << UART_ENABLE_ENABLE_Pos);
  NRF_UART0->TASKS_STARTTX    = 1;
  NRF_UART0->TASKS_STARTRX    = 1;
  NRF_UART0->EVENTS_RXDRDY    = 0;
  NRF_UART0->INTENSET         = (UART_INTENSET_RXDRDY_Enabled <<  UART_INTENSET_RXDRDY_Pos); 
  
}
                          
