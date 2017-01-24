/* Copyright (c) 2011 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is confidential property of Nordic Semiconductor. The use,
 * copying, transfer or disclosure of such information is prohibited except by express written
 * agreement with Nordic Semiconductor.
 *
 */
/**
  @addtogroup BLE_COMMON Definitions common to all S110 Softdevice modules.
  @{
  @defgroup ble_api Events, type definitions and API calls.
  @{

  @brief Module independent events, type definitions and API calls for the S110 Softdevice.

 */

#ifndef BLE_H__
#define BLE_H__

#include "ble_ranges.h"
#include "ble_types.h"
#include "ble_gap.h"
#include "ble_l2cap.h"
#include "ble_gatt.h"
#include "ble_gattc.h"
#include "ble_gatts.h"

/**
 * @brief Common API SVC numbers.
 */
enum
{
  SVC_BLE_EVT_GET  = BLE_SVC_BASE,       /**< Get an event from the pending events queue. */
  SVC_BLE_TX_BUFFER_COUNT_GET,           /**< Get the total number of available transmission buffers from the stack. */
  SVC_BLE_VS_UUIDS_ASSIGN,               /**< Assign Vendor Specific UUIDs. */
  SVC_BLE_VERSION_GET                    /**< Get the local version information (company id, LMP Version, LMP Subversion) */
};


/**
 * @brief BLE Module Independent Event IDs.
 */
enum
{
  BLE_EVT_TX_COMPLETE  = BLE_EVT_BASE,  /**< Transmission Complete. */
};


/**
 * @brief TX complete event.
 */
typedef struct
{
  uint8_t count;                        /**< Number of packets transmitted. */
} ble_evt_tx_complete_t;


/**@brief Event structure for events not associated with a specific function module. */
typedef struct
{
  uint16_t conn_handle;                 /**< Connection Handle on which this event occured. */
  union
  {
    ble_evt_tx_complete_t tx_complete;  /**< Transmission Complete. */
  } params;
} ble_common_evt_t;

/**@brief BLE Event header */
typedef struct
{
  uint16_t evt_id;                      /**< Value from a BLE_<module>_EVT series. */
  uint16_t evt_len;                     /**< Length in octets excluding this header */
} ble_evt_hdr_t;

/**@brief Common BLE Event type, wrapping the module specific event reports. */
typedef struct
{
  ble_evt_hdr_t header;                 /**< Event header */
  union
  {
    ble_common_evt_t common_evt;        /**< Event not associated with a function module. */
    ble_gap_evt_t   gap_evt;            /**< GAP originated event, evt_id in BLE_GAP_EVT_* series. */
    ble_l2cap_evt_t l2cap_evt;          /**< L2CAP originated event, evt_id in BLE_L2CAP_EVT* series. */
    ble_gattc_evt_t gattc_evt;          /**< GATT client originated event, evt_id in BLE_GATTC_EVT* series. */
    ble_gatts_evt_t gatts_evt;          /**< GATT server originated event, evt_id in BLE_GATTS_EVT* series. */
  } evt;
} ble_evt_t;


/**
 * @brief Version Information.
 */
typedef struct
{
  uint8_t   version_number;             /**< LMP Version number for BT 4.0 spec is 6 (https://www.bluetooth.org/technical/assignednumbers/link_layer.htm) */
  uint16_t  company_id;                 /**< Company ID, Nordic Semiconductor's company ID is 89 (0x0059) (https://www.bluetooth.org/apps/content/Default.aspx?doc_id=49708) */
  uint16_t  subversion_number;          /**< LMP Sub Version number corresponds to the Softdevice Config ID */
} ble_version_t;


/**@brief Get an event from the pending events queue.
 *
 * @param[in] p_dest Pointer to buffer to be filled in with an event, or NULL to retrieve the event length. This buffer <b>must be 4-byte aligned in memory</b>.
 * @param[in, out] p_len Pointer the length of the buffer, on return it is filled with the event length.
 *
 * @details This call allows the application to pull a BLE event from the BLE stack. The application is signalled that an event is 
 * available from the BLE Stack by the triggering of the SWI2 interrupt (mapped to IRQ 22).
 * The application is free to choose whether to call this function from thread mode (main context) or directly from the Interrupt Service Routine
 * that maps to SWI2. In any case however, and because the BLE stack runs at a higher priority than the application, this function should be called
 * in a loop (until @ref NRF_ERROR_NOT_FOUND is returned) every time SWI2 is raised to ensure that all available events are pulled from the stack. 
 * Failure to do so could potentially leave events in the internal queue without the application being aware of this fact.
 * Sizing the p_dest buffer is equally important, since the application needs to provide all the memory necessary for the event to be copied into
 * application memory. If the buffer provided is not large enough to fit the entire contents of the event, @ref NRF_ERROR_DATA_SIZE will be returned
 * and the application can then call again with a larger buffer size.
 * Please note that because of the variable length nature of some events, sizeof(ble_evt_t) will not always be large enough to fit certain events, 
 * and so it is the application's responsability to provide an amount of memory large enough so that the relevant event is copied in full.
 * The application may "peek" the event length by providing p_dest as a NULL pointer and inspecting the value of *p_len upon return.
 *
 * @return @ref NRF_SUCCESS If an event has been pulled.
 * @return @ref NRF_ERROR_NOT_FOUND If no events are ready to be pulled.
 * @return @ref NRF_ERROR_DATA_SIZE If no event did not fit in the application provided buffer 
 */
SVCALL(SVC_BLE_EVT_GET, uint32_t, ble_evt_get(uint8_t* p_dest, uint16_t *p_len));


/**@brief Get the total number of available transmission buffers available in BLE stack.
 *
 * @details This call allows the application to obtain the total number of
 *          transmission buffers available for application data. Please note that
 *          this does not give the number of free buffers, but rather the total amount of them.
 *
 * @param[out] p_count Pointer to a unit8_t which will contain the number of buffers upon
 *                     successful return.
 *
 * @return @ref NRF_SUCCESS If the number of buffers has been returned successfully.
 */
SVCALL(SVC_BLE_TX_BUFFER_COUNT_GET, uint32_t, ble_tx_buffer_count_get(uint8_t* p_count));


/**@brief Assign Vendor Specific UUIDs.
 *
 * @details This call allows the application to assign a set of vendor specific UUIDs with the BLE stack,
 *          for later use in both GAP, GATTC and GATTS.
 *
 * @param[in] vs_uuid_count Number of 16-octet (128-bit) Vendor Specific UUIDs supplied in the call.
 * @param[in] p_vs_uuids    Pointer to array of 16-octet (128-bit) little endian Vendor Specific UUIDs.
 *
 * @return @ref NRF_SUCCESS If the number of uuid's have been stored correctly
 * @return @ref NRF_ERROR_NO_MEM If the size exceeds the number of free slots for VS uuid's
 * @return @ref NRF_ERROR_INVALID_PARAM If the size of vs_uuid_count is 0
 * @return @ref NRF_ERROR_INVALID_ADDR if p_vs_uuids is NULL
 */
SVCALL(SVC_BLE_VS_UUIDS_ASSIGN, uint32_t, ble_uuid_vs_assign(uint16_t vs_uuid_count, ble_uuid128_t const * const p_vs_uuids));

/**@brief Get Version Information.
 *
 * @details This call allows the application to get version information of BLE Stack.
 *
 * @param[in] p_version Pointer to ble_version_t structure to be filled in.
 *
 * @return @ref NRF_SUCCESS  The version information was stored correctly
 * @return @ref NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @return @ref NRF_ERROR_BUSY The stack is busy (typically doing a locally-initiated disconnect procedure)
 */
SVCALL(SVC_BLE_VERSION_GET, uint32_t, ble_version_get(ble_version_t * p_version));

#endif /* BLE_H__ */

/**
  @}
  @}
*/
