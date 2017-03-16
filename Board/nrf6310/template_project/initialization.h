
#ifndef INITIALIZATION_H
#define INITIALIZATION_H

#define DELAY_MS              100        /*!< Timer Delay in milli-seconds */
#define PIN_SIGNAL_A           23
#define PIN_SIGNAL_B           21
#define LED                    18
#define DEBUG_PIN               0

void gpiote_init_encoder();
void gpiote_init(void);


#endif

