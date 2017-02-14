#ifndef SIMPLE_UART_H
#define SIMPLE_UART_H

/*lint ++flb "Enter library region" */

#include <stdbool.h>
#include <stdint.h>


/** Reads a character from UART.
Execution is blocked until UART peripheral detects character has been received.
\return cr Received character.
*/
uint8_t uart_get(void);

/** Reads a character from UART with timeout on how long to wait for the byte to be received
Execution is blocked until UART peripheral detects character has been received or until the timeout expires, which even occurs first
\return bool True, if byte is received before timeout, else returns False.
@param timeout_ms maximum time to wait for the data.
@param rx_data pointer to the memory where the received data is stored.
*/
bool uart_get_with_timeout(int32_t timeout_ms, uint8_t *rx_data);

/** Sends a character to UART.
Execution is blocked until UART peripheral reports character to have been send.
@param cr Character to send.
*/
void uart_put(uint8_t cr);

/** Sends a string to UART.
Execution is blocked until UART peripheral reports all characters to have been send.
Maximum string length is 254 characters including null character in the end.
@param str Null terminated string to send.
*/
void uart_putstring(const uint8_t *str);

/* Configures UART to use 38400 baud rate.
*/
void uart_config();

void uart_conv_ascii(uint32_t number);

void itoac(double data, uint8_t decimal);

void uart_config_encoder();

#endif
