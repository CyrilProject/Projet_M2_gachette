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
 * @defgroup ble_sdk_lib_uuid UUID Encoding / Decoding
 * @{
 * @ingroup ble_sdk_lib
 * @brief Functions for encoding and decoding UUIDs.
 */

#ifndef BLE_UUID_UTIL_H__
#define BLE_UUID_UTIL_H__

#include <stdint.h>
#include <stdbool.h>
#include "ble_types.h"

 /**@brief Initialize the UUID module 128-bit base list.
 *
 * @details This function is used to replace the default UUID base lookup table. The first 128-bit
 *          (16-bytes) shall be the BT SIG Base, and the following 128-bit entries can be vendor
 *          specific. All entries in this table shall be in little-endian.
 *          @note This function must be called if vendor specific 128 UUIDs are to be used, e.g.
 *                in the advertising data.
 *
 * @param[in]   base_uuid128_count  Number of elements in the Base lookup table.
 * @param[in]   p_base_uuid128      Pointer to new Base Lookup Table.
 *
 * @return      NRF_SUCCESS if base list successfully set, otherwise an error code.
 *
 * @note First 16 bytes shall be BT SIG base. If this interface is not called, a default look up
 *       table with BT SIG will be used.
 */
uint32_t ble_uuid_base_set(uint16_t base_uuid128_count, const ble_uuid128_t * p_base_uuid128);

/**@brief Decode the UUID bytes (16-bit or 128-bit) in Little-Endian to uuid_t structure.
 *
 * @param[in]   uuid_len    Length in bytes of the uuid to be decoded (2 or 16 bytes).
 * @param[in]   p_encoded   Pointer pointing to LE UUID bytes.
 * @param[out]  p_uuid      Pointer to ble_uuid_t structure which shall be filled.
 *
 * @return      NRF_SUCCESS on successful read of the report, otherwise an error code.
 */
uint32_t ble_uuid_decode(uint8_t uuid_len, const uint8_t * p_encoded, ble_uuid_t * p_uuid);

 /**@brief Encode the uuid_t structure into Little-Endian UUID bytes (16-bit or 128-bit).
 *
 * @param[in]   p_uuid      Pointer to ble_uuid_t structure that shall be encoded.
 * @param[out]  p_uuid_len  Pointer to length of encoded data.
 * @param[out]  p_encoded   Pointer to buffer that shall be filled with little-endian
 *                             16 or 128 bit uuid. If p_encoded is NULL, this function will
 *                             still compute p_uuid_len.
 *
 * @return      NRF_SUCCESS on successful read of the report, otherwise an error code.
 */
uint32_t ble_uuid_encode(const ble_uuid_t * p_uuid, uint8_t * p_uuid_len, uint8_t * p_encoded);

#endif // BLE_UUID_UTIL_H__

/** @} */
