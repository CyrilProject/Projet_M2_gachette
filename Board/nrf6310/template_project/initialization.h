
#ifndef INITIALIZATION_H
#define INITIALIZATION_H

#define DELAY_MS              100        /*!< Timer Delay in milli-seconds */
#define PIN_SIGNAL_A            5
#define PIN_SIGNAL_B            7
#define LED                    18
#define UART_TX_PIN             3
#define UART_RX_PIN             1


void gpiote_init_encoder();
void gpiote_init(void);


#endif

