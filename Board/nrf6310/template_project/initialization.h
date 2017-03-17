
#ifndef INITIALIZATION_H
#define INITIALIZATION_H

#define DELAY_MS              100        /*!< Timer Delay in milli-seconds */

/* Pins definitions */

#define PIN_SIGNAL_A           21
#define PIN_SIGNAL_B           23
#define BAT_LVL_ON              6
#define BAT_LVL                 4
#define LED                     2
#define BUCK_ON                 0

void gpiote_init_encoder();
void gpiote_init(void);


#endif

