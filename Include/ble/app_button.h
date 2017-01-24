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

/** @file
 *
 * @defgroup ble_sdk_lib_button Button Handler
 * @{
 * @ingroup ble_sdk_lib
 * @brief Buttons handling module.
 *
 * @details .
 */

#ifndef APP_BUTTON_H__
#define APP_BUTTON_H__

#include <stdint.h>
#include <stdbool.h>
#include "nrf_sdm.h"
#include "nrf_assert.h"

#define APP_BUTTON_DEBOUNCE_COUNTER_SIZE  sizeof(uint8_t)   /**< Size of a single debounce counter (only for use inside APP_BUTTON_BUF_SIZE()). */

/**@brief Compute number of bytes required to hold the Buttons handler data structures.
 *
 * @param[in]  BUTTONS_COUNT   Number of buttons to be handled by the Button Handler module.
 *
 * @return     Required Buttons handler buffer size (in bytes).
 */
#define APP_BUTTON_BUF_SIZE(BUTTONS_COUNT)  ((BUTTONS_COUNT) * APP_BUTTON_DEBOUNCE_COUNTER_SIZE)

/**@brief Application timeout handler type. */
typedef void (*app_button_handler_t)(uint8_t pin_no);

/**@brief Button types. */
typedef enum
{
    APP_BUTTON_NO_WAKEUP_NO_PULLUP,                         /**< Button not to be used as a wakeup source. Do not configure pullup. */
    APP_BUTTON_WAKEUP_NO_PULLUP,                            /**< Button to be used as a wakeup source. Do not configure pullup. */
    APP_BUTTON_NO_WAKEUP_WITH_PULLUP,                       /**< Button not to be used as a wakeup source. Configure pullup. */
    APP_BUTTON_WAKEUP_WITH_PULLUP                           /**< Button to be used as a wakeup source. Configure pullup. */
} app_button_type_t;

/**@brief Button configuration structure. */
typedef struct
{
    uint8_t              pin_no;                            /**< Pin to be used as a button. */
    app_button_type_t    button_type;                       /**< Type of button. */
    app_button_handler_t button_handler;                    /**< Handler to be called when button is pushed. */
} app_button_cfg_t;

/**@brief Macro for initializing the Button Handler module.
 *
 * @details It will initialize the specified pins as buttons (but it will not start the polling
 *          timer). It will also allocate the buffer needed by the Button Handler module.
 *
 * @param[in]  BUTTONS         Array of buttons to be used (type app_button_cfg_t, must be static!).
 * @param[in]  BUTTONS_COUNT   Number of buttons.
 * @param[in]  POLL_INTERVAL   Duration between each time the buttons state is to be polled
 *                             (in number of timer ticks).
 * @param[in]  DEBOUNCE_LIMIT  Number of consequtive positive read operations before a button is
 *                             reported as pushed.
 */
/*lint -emacro(506, APP_BUTTON_INIT) */ /* Suppress "Constant value Boolean */
#define APP_BUTTON_INIT(BUTTONS, BUTTONS_COUNT, POLL_INTERVAL, DEBOUNCE_LIMIT)                     \
    do                                                                                             \
    {                                                                                              \
        static uint8_t APP_BUTTON_BUF[APP_BUTTON_BUF_SIZE(BUTTONS_COUNT)];                         \
        uint32_t ERR_CODE = app_button_init((BUTTONS),                                             \
                                            (BUTTONS_COUNT),                                       \
                                            (POLL_INTERVAL),                                       \
                                            (DEBOUNCE_LIMIT),                                      \
                                            APP_BUTTON_BUF);                                       \
        if (ERR_CODE != NRF_SUCCESS)                                                               \
        {                                                                                          \
            ASSERT(false);                                                                         \
        }                                                                                          \
    } while (0)

/**@brief Initialize the Buttons module.
 *
 * @details This function will initialize the specified pins as buttons, but it will not start the
 *          polling timer.
 *
 * @note Normally initialization should be done using the APP_BUTTON_INIT() macro, as that will also
 *       allocate the buffer needed by the Buttons module.
 *
 * @param[in]  p_buttons        Array of buttons to be used (NOTE: Must be static!).
 * @param[in]  buttons_count    Number of buttons.
 * @param[in]  poll_interval    Duration between each time the buttons state is to be polled
 *                              (in number of timer ticks).
 * @param[in]  debounce_limit   Number of consequtive positive read operations before a button is
 *                              reported as pushed.
 * @param[in]  p_buffer         Pointer to memory buffer for internal use in the app_button
 *                              module. The size of the buffer can be computed using the
 *                              APP_BUTTON_BUF_SIZE() macro.
 *
 * @return   NRF_SUCCESS on success, otherwise an error code.
 */
uint32_t app_button_init(app_button_cfg_t * p_buttons,
                         uint8_t            buttons_count,
                         uint32_t           poll_interval,
                         uint8_t            debounce_limit,
                         void *             p_buffer);

/**@brief Enables button polling by starting the button polling timer.
 *
 * @return   NRF_SUCCESS on success, otherwise an error code.
 */
uint32_t app_button_enable(void);

/**@brief Disables button polling by stopping the button polling timer.
 *
 * @return   NRF_SUCCESS on success, otherwise an error code.
 */
uint32_t app_button_disable(void);

/**@brief Checks if a button is currently being pushed.
 *
 * @param[in]  pin_no        Button pin to be checked.
 * @param[out] p_is_pushed   Button state.
 *
 * @retval     NRF_SUCCESS   State successfully read.
 */
uint32_t app_button_is_pushed(uint8_t pin_no, bool * p_is_pushed);

#endif // APP_BUTTON_H__

/** @} */
