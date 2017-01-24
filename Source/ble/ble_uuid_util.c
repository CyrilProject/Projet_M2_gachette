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
 
#include "ble_uuid_util.h"
#include <string.h>
#include "nrf_error.h"
#include "ble.h"
#include "ble_types.h"
#include "ble_sdk_util.h"


/**@brief Number of entries in default UUID 128-bit BASE List. */
#define BASE_UUID128_DEFAULT_COUNT (sizeof(s_base_uuid128_default) / sizeof(ble_uuid128_t))

/**@brief Default UUID 128-bit BASE List (BT SIG UUID). */
static const ble_uuid128_t s_base_uuid128_default[] =
{
    {
        {
            0xFB, 0x34, 0x9B, 0x5F, 0x80, 0x00, 0x00, 0x80,
            0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        }
    }
};


static const ble_uuid128_t * volatile sp_base_uuid128      = s_base_uuid128_default;       /**< UUID 128-bit BASE List. */
static volatile uint8_t               s_base_uuid128_count = BASE_UUID128_DEFAULT_COUNT;   /**< Number of entries in UUID 128-bit BASE List. */


uint32_t ble_uuid_base_set(uint16_t base_uuid128_count, const ble_uuid128_t * p_base_uuid128)
{
    sp_base_uuid128      = p_base_uuid128;
    s_base_uuid128_count = base_uuid128_count;
    
    return ble_uuid_vs_assign(base_uuid128_count, p_base_uuid128);
}


uint32_t ble_uuid_decode(uint8_t uuid_len, const uint8_t * p_encoded, ble_uuid_t * p_uuid)
{
    p_uuid->type = BLE_UUID_TYPE_UNKNOWN;
    p_uuid->uuid = 0x0000;

    if (uuid_len == sizeof(uint16_le_t))
    {
        p_uuid->type = BLE_UUID_TYPE_BLE;
        p_uuid->uuid = uint16_decode(p_encoded);
        
        if (p_uuid->uuid != 0x0000)
        {
            return NRF_SUCCESS;
        }
    }
    else if (uuid_len == sizeof(ble_uuid128_t))
    {
        uint8_t i;

        p_uuid->uuid = uint16_decode(&p_encoded[12]);
        
        // Determine the type
        i = s_base_uuid128_count;
        while (i--)
        {
            if (
                (memcmp((void *)&p_encoded[0], &sp_base_uuid128[i].uuid128[0], 12) == 0)
                &&
                (p_encoded[14] == sp_base_uuid128[i].uuid128[14])
                &&
                (p_encoded[15] == sp_base_uuid128[i].uuid128[15])
            )
            {
                // Table indexed from 1 by caller, hence increment by 1
                p_uuid->type = i + 1;
                return NRF_SUCCESS;
            }
        }
    }
    
    return NRF_ERROR_INVALID_DATA;
}


uint32_t ble_uuid_encode(const ble_uuid_t * p_uuid, uint8_t * p_uuid_len, uint8_t * p_encoded)
{
    *p_uuid_len = 0;

    if (p_uuid->type == BLE_UUID_TYPE_BLE)
    {
        if (p_encoded != NULL)
        {
            (void)uint16_encode(p_uuid->uuid, p_encoded);
        }
        *p_uuid_len = sizeof(uint16_le_t);
        return NRF_SUCCESS;
    }
    else if (
        (p_uuid->type >= BLE_UUID_TYPE_VENDOR_BEGIN)
        &&
        (p_uuid->type <= s_base_uuid128_count)
    )
    {
        if (p_encoded != NULL)
        {
            memcpy(p_encoded,
                   &(sp_base_uuid128[p_uuid->type - 1].uuid128[0]),
                   sizeof(ble_uuid128_t));
            (void)uint16_encode(p_uuid->uuid, &p_encoded[12]);
        }
        *p_uuid_len = sizeof(ble_uuid128_t);
        return NRF_SUCCESS;
    }

    return NRF_ERROR_INVALID_PARAM;
}
