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

#include "ble_stack_handler.h"
#include <stdlib.h>
#include "nordic_common.h"
#include "nrf_assert.h"


static ble_stack_evt_handler_t       m_evt_handler;         /**< Application event handler for handling BLE events. */
static ble_stack_evt_schedule_func_t m_evt_schedule_func;   /**< Pointer to function for propagating button events to the scheduler. */
static uint8_t *                     m_evt_buffer;          /**< Buffer for receiving BLE events. */
static uint16_t                      m_evt_buffer_size;     /**< Size of BLE event buffer. */


/**@brief SoftDevice assert callback function.
 *
 * @details A pointer to this function will be passed to the SoftDevice. This function will be
 *          called if an ASSERT statement in the SoftDevice fails.
 *
 * @param[in]   pc         The value of the program counter when the ASSERT call failed.
 * @param[in]   line_num   Line number of the failing ASSERT call.
 * @param[in]   file_name  File name of the failing ASSERT call.
 */
static void softdevice_assertion_handler(uint32_t pc, uint16_t line_num, const uint8_t * file_name)
{
    UNUSED_PARAMETER(pc);
    assert_nrf_callback(line_num, file_name);
}


uint32_t ble_stack_handler_init(nrf_clock_lfclksrc_t          clock_source,
                                void *                        p_evt_buffer,
                                uint16_t                      evt_buffer_size,
                                ble_stack_evt_handler_t       evt_handler,
                                ble_stack_evt_schedule_func_t evt_schedule_func)
{
    uint32_t err_code;

    // Check that buffer is correctly aligned
    if (((uint32_t)p_evt_buffer & 0x00000003) != 0)
    {
        return NRF_ERROR_INVALID_PARAM;
    }
    
    // Save configuration
    m_evt_buffer        = (uint8_t *)p_evt_buffer;
    m_evt_buffer_size   = evt_buffer_size;
    m_evt_handler       = evt_handler;
    m_evt_schedule_func = evt_schedule_func;

    // Initialise SoftDevice
    err_code = nrf_softdevice_enable(clock_source, softdevice_assertion_handler);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Enable BLE event interrupt (interrupt priority has already been set by the stack)
    return nrf_nvic_EnableIRQ(SWI2_IRQn);
}


void intern_ble_stack_events_execute(void)
{
    for (;;)
    {
        uint32_t err_code;
        uint16_t evt_len = m_evt_buffer_size;

        // Pull event from stack
        err_code = ble_evt_get(m_evt_buffer, &evt_len);
        if (err_code == NRF_ERROR_NOT_FOUND)
        {
            break;
        }
        else if (err_code != NRF_SUCCESS)
        {
            ASSERT(false);
        }
        
        // Call application's BLE stack event handler
        m_evt_handler((ble_evt_t *)m_evt_buffer);
    }
}


/**@brief BLE Stack event interrupt handler.
 *
 * @details This function is called whenever an event is ready to be pulled.
 */
void SWI2_IRQHandler(void)
{
    if (m_evt_schedule_func != NULL)
    {
        uint32_t err_code = m_evt_schedule_func();
        if (err_code != NRF_SUCCESS)
        {
            ASSERT(false);
        }
    }
    else
    {
        intern_ble_stack_events_execute();
    }
}
