/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
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

#include "app_button.h"
#include <stdlib.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf_gpio.h"
#include "ble_sdk_util.h"
#include "app_timer.h"


static app_button_cfg_t * m_buttons;                /**< Button configuration. */
static uint8_t            m_buttons_count;          /**< Number of configured buttons. */
static uint8_t            m_debounce_limit;         /**< Number of consequtive positive reads required to count as a pushed button. */
static uint32_t           m_poll_interval;          /**< Buttons state polling interval. */
static app_timer_id_t     m_buttons_poll_timer_id;  /**< Polling timer id. */
static uint8_t *          m_debounce_counters;      /**< Array of debounce counters for all buttons. */

STATIC_ASSERT(APP_BUTTON_DEBOUNCE_COUNTER_SIZE == sizeof(uint8_t));


/**@brief Timeout handler for polling the button states.
 *
 * @param[in]  p_context   Pointer used for passing some arbitrary information (context) from the
 *                         app_start_timer() call to the timeout handler (unused).
 */
static void buttons_poll_timeout_handler(void * p_context)
{
    uint8_t i;
    
    UNUSED_PARAMETER(p_context);

    // Check all buttons
    for (i = 0; i < m_buttons_count; i++)
    {
        app_button_cfg_t * p_btn = &m_buttons[i];
        
        if (p_btn->button_handler != NULL)
        {
            if (nrf_gpio_pin_read(p_btn->pin_no) == 0)
            {
                // Button is pushed, handle debounce
                m_debounce_counters[i]++;
                if (m_debounce_counters[i] == m_debounce_limit)
                {
                    // Button was pushed, execute button handler
                    p_btn->button_handler(p_btn->pin_no);
                }
            }
            else
            {
                m_debounce_counters[i] = 0;
            }
        }
    }
}


uint32_t app_button_init(app_button_cfg_t * p_buttons,
                         uint8_t            buttons_count,
                         uint32_t           poll_interval,
                         uint8_t            debounce_limit,
                         void *             p_buffer)
{
    // Save configuration
    m_buttons           = p_buttons;
    m_buttons_count     = buttons_count;
    m_poll_interval     = poll_interval;
    m_debounce_limit    = debounce_limit;
    m_debounce_counters = (uint8_t *)p_buffer;
    
    // Configure pins
    while (buttons_count--)
    {
        app_button_cfg_t * p_btn = &p_buttons[buttons_count];
        
        switch (p_btn->button_type)
        {
            case APP_BUTTON_NO_WAKEUP_NO_PULLUP:
                GPIO_BUTTON_CONFIG(p_btn->pin_no);
                break;
                
            case APP_BUTTON_WAKEUP_NO_PULLUP:
                GPIO_WAKEUP_BUTTON_CONFIG(p_btn->pin_no);
                break;
                
            case APP_BUTTON_NO_WAKEUP_WITH_PULLUP:
                GPIO_BUTTON_WITH_PULLUP_CONFIG(p_btn->pin_no);
                break;
                
            case APP_BUTTON_WAKEUP_WITH_PULLUP:
                GPIO_WAKEUP_BUTTON_WITH_PULLUP_CONFIG(p_btn->pin_no);
                break;
                
            default:
                break;
        }
    }
    
    // Create polling timer
    return app_timer_create(&m_buttons_poll_timer_id,
                            APP_TIMER_MODE_REPEATED,
                            buttons_poll_timeout_handler);
}


uint32_t app_button_enable(void)
{
    memset(m_debounce_counters, 0, sizeof(uint8_t) * m_buttons_count);
    return app_timer_start(m_buttons_poll_timer_id, m_poll_interval, NULL);
}


uint32_t app_button_disable(void)
{
    return app_timer_stop(m_buttons_poll_timer_id);
}


uint32_t app_button_is_pushed(uint8_t pin_no, bool * p_is_pushed)
{
    *p_is_pushed = (nrf_gpio_pin_read(pin_no) == 0);
    return NRF_SUCCESS;
}
