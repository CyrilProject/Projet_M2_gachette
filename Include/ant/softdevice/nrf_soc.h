/* Copyright (c) 2011 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is confidential property of Nordic Semiconductor. The use,
 * copying, transfer or disclosure of such information is prohibited except by express written
 * agreement with Nordic Semiconductor.
 *
 */
 
/**
  @defgroup nrf_soc_api SoC Library API
  @{
  
  @brief APIs for the SoC library.
  
*/

#ifndef NRF_SOC_H__
#define NRF_SOC_H__

#include <stdint.h>
#include <stdbool.h>
#include "nrf.h"
#include "nrf_error_soc.h"

#ifndef SVCALL
#if defined(__arm__)
#define SVCALL(x) __svc(x)
#else 
#define SVCALL(x)
#endif
#endif  // SVCALL

/**@brief The number of the lowest SVC number reserved for the SoC library. */
#define SOC_SVC_BASE 0x20


/**@brief The SVC numbers used by the SVC functions in the SoC library. */
enum {
  SVC_MUTEX_NEW = SOC_SVC_BASE,
  SVC_MUTEX_ACQUIRE,
  SVC_MUTEX_RELEASE,
  SVC_NVIC_ENABLEIRQ,
  SVC_NVIC_DISABLEIRQ,
  SVC_NVIC_GETPENDINGIRQ,
  SVC_NVIC_SETPENDINGIRQ,
  SVC_NVIC_CLEARPENDINGIRQ,
  SVC_NVIC_SETPRIORITY,
  SVC_NVIC_GETPRIORITY,
  SVC_NVIC_SYSTEMRESET,
  SVC_NVIC_CRITICAL_REGION_ENTER,
  SVC_NVIC_CRITICAL_REGION_EXIT,
  SVC_RAND_APPLICATION_POOL_CAPACITY,
  SVC_RAND_APPLICATION_BYTES_AVAILABLE,
  SVC_RAND_APPLICATION_GET_VECTOR,
  SVC_POWER_MODE_SET,
  SVC_POWER_PERPOWER_SET,
  SVC_POWER_PERPOWER_CLR,
  SVC_POWER_PERPOWER_GET,
  SVC_POWER_PERRDY_GET,
  SVC_POWER_SYSTEM_OFF,
  SVC_POWER_RESET_REASON_GET,
  SVC_POWER_RESET_REASON_CLR,
  SVC_POWER_POF_ENABLE,
  SVC_POWER_POF_THRESHOLD_SET,
  SVC_POWER_RAMON_SET,
  SVC_POWER_RAMON_CLR,
  SVC_POWER_RAMON_GET,
  SVC_NRF_WAIT_FOR_APP_EVENT,
  SVC_CLOCK_HFCLK_REQUEST,
  SVC_CLOCK_HFCLK_RELEASE,
  SVC_CLOCK_HFCLK_IS_RUNNING,
  SVC_PPI_CHANNEL_ENABLE_GET,
  SVC_PPI_CHANNEL_ENABLE_SET,
  SVC_PPI_CHANNEL_ENABLE_CLR,
  SVC_PPI_CHANNEL_ASSIGN,
  SVC_PPI_GROUP_TASK_ENABLE,
  SVC_PPI_GROUP_TASK_DISABLE,
  SVC_PPI_GROUP_ASSIGN,
  SVC_PPI_GROUP_GET,  

  SVC_SOC_LAST
};


/**@brief Possible values of a ::nrf_mutex_t. */
enum {
  NRF_MUTEX_FREE,
  NRF_MUTEX_TAKEN
};

/**@brief Represents a mutex for use with the nrf_mutex functions.
 * @note Accessing the value directly is not safe, use the mutex functions!
 */
typedef volatile uint8_t nrf_mutex_t;

/**@brief The interrupt priorities available to the application while the softdevice is active. */
typedef enum {
  NRF_APP_PRIORITY_HIGH = 1,
  NRF_APP_PRIORITY_LOW = 3
} nrf_app_irq_priority_t;

/**@brief Possible values of ::nrf_power_mode_t. */
enum {
  NRF_POWER_MODE_CONSTLAT,  /**< Constant latency mode. See power management in the reference manual. */
  NRF_POWER_MODE_LOWPWR     /**< Low power mode. See power management in the reference manual. */
};

/**@brief Represents a power mode, used in power mode functions */
typedef uint8_t nrf_power_mode_t;

/**@brief Possible values of ::nrf_power_failure_threshold_t */
enum {
  NRF_POWER_THRESHOLD_V21,  /**< 2.1 Volts power failure threshold */
  NRF_POWER_THRESHOLD_V23,  /**< 2.3 Volts power failure threshold */
  NRF_POWER_THRESHOLD_V25,  /**< 2.5 Volts power failure threshold */ 
  NRF_POWER_THRESHOLD_V27   /**< 2.7 Volts power failure threshold  */
};

/**@brief Represents a power failure threshold value */
typedef uint8_t nrf_power_failure_threshold_t;

/**@brief Power failure threshold callback */
typedef void (*nrf_power_failure_callback_t)(void);

/**@brief Initialize a mutex.
 *
 * @param[in] p_mutex Pointer to the mutex to initialize.
 *
 * @return ::NRF_SUCCESS
 */
uint32_t SVCALL(SVC_MUTEX_NEW) nrf_mutex_new(nrf_mutex_t * p_mutex);

/**@brief Attempt to acquire a mutex.
 *
 * @param[in] p_mutex Pointer to the mutex to acquire.
 *
 * @return ::NRF_SUCCESS - if mutex was successfully acquired.
 * @return ::NRF_ERROR_SOC_MUTEX_ALREADY_TAKEN - if mutex could not be acquired.
 */
uint32_t SVCALL(SVC_MUTEX_ACQUIRE) nrf_mutex_acquire(nrf_mutex_t * p_mutex);

/**@brief Release a mutex.
 *
 * @param[in] p_mutex Pointer to the mutex to release.
 *
 * @return ::NRF_SUCCESS
 */
uint32_t SVCALL(SVC_MUTEX_RELEASE) nrf_mutex_release(nrf_mutex_t * p_mutex);

/**@brief Enable External Interrupt.
 * @note Corresponds to NVIC_EnableIRQ in CMSIS.
 *
 * @pre{IRQn is valid and not reserved by the stack}
 *
 * @param[in] IRQn See the NVIC_EnableIRQ documentation in CMSIS.
 *
 * @return ::NRF_SUCCESS - if the interrupt was enabled.
 * @return ::NRF_ERROR_SOC_NVIC_INTERRUPT_NOT_AVAILABLE - if the interrupt is not available for the application.
 * @return ::NRF_ERROR_SOC_NVIC_INTERRUPT_PRIORITY_NOT_ALLOWED - if the interrupt has a priority not available for the application.
 */
uint32_t SVCALL(SVC_NVIC_ENABLEIRQ) nrf_nvic_EnableIRQ(IRQn_Type IRQn);

/**@brief  Disable External Interrupt.
 * @note Corresponds to NVIC_DisableIRQ in CMSIS.
 *
 * @pre{IRQn is valid and not reserved by the stack}
 *
 * @param[in] IRQn See the NVIC_DisableIRQ documentation in CMSIS
 *
 * @return ::NRF_SUCCESS - if the interrupt was disabled.
 * @return ::NRF_ERROR_SOC_NVIC_INTERRUPT_NOT_AVAILABLE - if the interrupt is not available for the application.
 */
uint32_t SVCALL(SVC_NVIC_DISABLEIRQ) nrf_nvic_DisableIRQ(IRQn_Type IRQn);

/**@brief  Get Pending Interrupt.
 * @note Corresponds to NVIC_GetPendingIRQ in CMSIS.
 *
 * @pre{IRQn is valid and not reserved by the stack}
 *
 * @param[in]   IRQn          See the NVIC_GetPendingIRQ documentation in CMSIS.
 * @param[out]  p_pending_irq Return value from NVIC_GetPendingIRQ.
 *
 * @return ::NRF_SUCCESS - if the interrupt is available for the application.
 * @return ::NRF_ERROR_SOC_NVIC_INTERRUPT_NOT_AVAILABLE - if IRQn is not available for the application.
 */
uint32_t SVCALL(SVC_NVIC_GETPENDINGIRQ) nrf_nvic_GetPendingIRQ(IRQn_Type IRQn, uint32_t * p_pending_irq);

/**@brief  Set Pending Interrupt.
 * @note Corresponds to NVIC_SetPendingIRQ in CMSIS.
 *
 * @pre{IRQn is valid and not reserved by the stack}
 *
 * @param[in] IRQn See the NVIC_SetPendingIRQ documentation in CMSIS.
 *
 * @return ::NRF_SUCCESS - if the interrupt is available for the application.
 * @return ::NRF_ERROR_SOC_NVIC_INTERRUPT_NOT_AVAILABLE - if IRQn is not available for the application.
 */
uint32_t SVCALL(SVC_NVIC_SETPENDINGIRQ) nrf_nvic_SetPendingIRQ(IRQn_Type IRQn);

/**@brief  Clear Pending Interrupt.
 * @note Corresponds to NVIC_ClearPendingIRQ in CMSIS.
 *
 * @pre{IRQn is valid and not reserved by the stack}
 *
 * @param[in] IRQn See the NVIC_ClearPendingIRQ documentation in CMSIS.
 *
 * @return ::NRF_SUCCESS - if the interrupt is available for the application.
 * @return ::NRF_ERROR_SOC_NVIC_INTERRUPT_NOT_AVAILABLE - if IRQn is not available for the application.
 */
uint32_t SVCALL(SVC_NVIC_CLEARPENDINGIRQ) nrf_nvic_ClearPendingIRQ(IRQn_Type IRQn);

/**@brief Set Interrupt Priority.
 * @note Corresponds to NVIC_SetPriority in CMSIS.
 *
 * @pre{IRQn is valid and not reserved by the stack}
 * @pre{priority is valid and not reserved by the stack}
 *
 * @param[in] IRQn      See the NVIC_SetPriority documentation in CMSIS.
 * @param[in] priority  A valid IRQ priority for use by the application.
 *
 * @return ::NRF_SUCCESS - if the interrupt and priority level is available for the application.
 * @return ::NRF_ERROR_SOC_NVIC_INTERRUPT_NOT_AVAILABLE - if IRQn is not available for the application.
 * @return ::NRF_ERROR_SOC_NVIC_INTERRUPT_PRIORITY_NOT_ALLOWED - if priority is not available for the application.
 */
uint32_t SVCALL(SVC_NVIC_SETPRIORITY) nrf_nvic_SetPriority(IRQn_Type IRQn, nrf_app_irq_priority_t priority);

/**@brief Get Interrupt Priority.
 * @note Corresponds to NVIC_GetPriority in CMSIS.
 *
 * @pre{IRQn is valid and not reserved by the stack}
 *
 * @param[in]  IRQn         See the NVIC_GetPriority documentation in CMSIS.
 * @param[out] p_priority   Return value from NVIC_GetPriority.
 *
 * @return ::NRF_SUCCESS - if the interrupt is available for the application.
 * @return ::NRF_ERROR_SOC_NVIC_INTERRUPT_NOT_AVAILABLE - if IRQn is not available for the application.
 */
uint32_t SVCALL(SVC_NVIC_GETPRIORITY) nrf_nvic_GetPriority(IRQn_Type IRQn, nrf_app_irq_priority_t * p_priority);

/**@brief System Reset.
 * @note Corresponds to NVIC_SystemReset in CMSIS.
 *
 * @return ::NRF_ERROR_SOC_NVIC_SHOULD_NOT_RETURN
 */
uint32_t SVCALL(SVC_NVIC_SYSTEMRESET) nrf_nvic_SystemReset(void);

/**@brief Enters critical region.
 *
 * @post Application interrupts will be disabled.
 * @sa nrf_nvic_critical_region_exit
 *
 * @param[out]  p_is_nested_critical_region  1: If in a nested critical region.
 *                                           0: Otherwise.
 *
 * @return ::NRF_SUCCESS
 */
uint32_t SVCALL(SVC_NVIC_CRITICAL_REGION_ENTER) nrf_nvic_critical_region_enter(uint8_t * p_is_nested_critical_region);

/**@brief Exit critical region.
 *
 * @pre Application has entered a critical region using ::nrf_nvic_critical_region_enter.
 * @post If not in a nested critical region, the application interrupts will restored to the state before ::nrf_nvic_critical_region_enter was called. 
 *
 * @param[in] is_nested_critical_region If this is set to 1, the critical region won't be exited. @sa nrf_nvic_critical_region_enter.
 *
 * @return ::NRF_SUCCESS
 */
uint32_t SVCALL(SVC_NVIC_CRITICAL_REGION_EXIT) nrf_nvic_critical_region_exit(uint8_t is_nested_critical_region);

/**@brief Query the capacity of the application random pool.
 *
 * @param[out] p_pool_capacity The capacity of the pool.
 *
 * @return ::NRF_SUCCESS
 */
uint32_t SVCALL(SVC_RAND_APPLICATION_POOL_CAPACITY) nrf_rand_application_pool_capacity_get(uint8_t * p_pool_capacity);

/**@brief Get number of random bytes available to the application.
 *
 * @param[out] p_bytes_available The number of bytes currently available in the pool.
 *
 * @return ::NRF_SUCCESS
 */
uint32_t SVCALL(SVC_RAND_APPLICATION_BYTES_AVAILABLE) nrf_rand_application_bytes_available_get(uint8_t * p_bytes_available);

/**@brief Get random bytes from the application pool.

  @param[out]  p_buff pointer to unit8_t buffer for storing the bytes.
  @param[in]  length number of bytes to take from pool and place in p_buff.

  @return ::NRF_SUCCESS - if the requested bytes were written to p_buff.
  @return ::NRF_ERROR_SOC_RAND_NOT_ENOUGH_VALUES - if no bytes were written, because there were not enough bytes available.
*/
uint32_t SVCALL(SVC_RAND_APPLICATION_GET_VECTOR) nrf_rand_application_vector_get(uint8_t * p_buff, uint8_t length);

/**@brief Gets the reset reason register. 
 *
 * @param[out]  p_reset_reason  Contents of the NRF_POWER->RESETREAS register.
 *
 * @return ::NRF_SUCCESS
 */
uint32_t SVCALL(SVC_POWER_RESET_REASON_GET) nrf_power_reset_reason_get(uint32_t * p_reset_reason);

/**@brief Clears the bits of the reset reason register. 
 *
 * @param[in] reset_reason_clr_msk Contains the bits to clear from the reset reason register.
 *
 * @return ::NRF_SUCCESS
 */
uint32_t SVCALL(SVC_POWER_RESET_REASON_CLR) nrf_power_reset_reason_clr(uint32_t reset_reason_clr_msk);

/**@brief Sets the power mode when in CPU sleep.
 *
 * @param[in] power_mode The power mode to use when in CPU sleep. @sa nrf_wait_for_app_event
 *
 * @return ::NRF_SUCCESS - if the power mode was set.
 * @return ::NRF_ERROR_SOC_POWER_MODE_UNKNOWN - if the power mode was unknown.
 */
uint32_t SVCALL(SVC_POWER_MODE_SET) nrf_power_mode_set(nrf_power_mode_t power_mode);

/**@brief Sets bits in the NRF_POWER->PERPOWER register.
 *
 * @param[in] peripheral_power_msk Contains the bits of the peripherals to turn on. 1's turn peripherals on, 0's has no effect.
 *
 * @return ::NRF_SUCCESS
 */
uint32_t SVCALL(SVC_POWER_PERPOWER_SET) nrf_power_perpower_set(uint32_t peripheral_power_msk);

/**@brief Clears bits in the NRF_POWER->PERPOWER register.
 *
 * @param[in] peripheral_power_msk Contains the bits of the peripherals to turn off. 1's turn peripherals off, 0's has no effect.
 *
 * @return ::NRF_SUCCESS
 */
uint32_t SVCALL(SVC_POWER_PERPOWER_CLR) nrf_power_perpower_clr(uint32_t peripheral_power_msk);

/**@brief Get contents of NRF_POWER->PERRDY register, indicates which peripherals are powered on/off.
 *
 * @param[out] p_perrdy Content of NRF_POWER->PERRDY register.
 *
 * @return ::NRF_SUCCESS
 */
uint32_t SVCALL(SVC_POWER_PERRDY_GET) nrf_power_perrdy_get(uint32_t * p_perrdy);

/**@brief Puts the chip in System OFF mode. 
 *
 * @return ::NRF_ERROR_SOC_POWER_OFF_SHOULD_NOT_RETURN
 */
uint32_t SVCALL(SVC_POWER_SYSTEM_OFF) nrf_power_system_off(void);

/**@brief Enables or disables the power-fail comparator.
 *
 * The pof_callback will be called in SVC priority.
 *
 * @param[in] pof_enable true if the power-fail comparator should be enabled, false if it should be disabled.
 * @param[in] pof_callback power failure callback function to register, can be set to NULL-pointer when disabling.
 *
 * @return ::NRF_SUCCESS
 */
uint32_t SVCALL(SVC_POWER_POF_ENABLE) nrf_power_pof_enable(uint8_t pof_enable, nrf_power_failure_callback_t pof_callback);

/**@brief Sets the power-fail threshold value.
 *
 * @param[in] threshold The power-fail threshold value to use.
 *
 * @return ::NRF_SUCCESS - if the threshold was set.
 * @return ::NRF_ERROR_SOC_POWER_POF_THRESHOLD_UNKNOWN - if the threshold is unknown.
 */
uint32_t SVCALL(SVC_POWER_POF_THRESHOLD_SET) nrf_power_pof_threshold_set(nrf_power_failure_threshold_t threshold);

/**@brief Sets bits in the NRF_POWER->RAMON register.
 *
 * @param[in] ramon Contains the bits needed to be set in the NRF_POWER->RAMON register.
 *
 * @return ::NRF_SUCCESS
 */
uint32_t SVCALL(SVC_POWER_RAMON_SET) nrf_power_ramon_set(uint32_t ramon);

/** @brief Clears bits in the NRF_POWER->RAMON register.
 *
 * @param ramon Contains the bits needed to be cleared in the NRF_POWER->RAMON register.
 *
 * @return ::NRF_SUCCESS
 */
uint32_t SVCALL(SVC_POWER_RAMON_CLR) nrf_power_ramon_clr(uint32_t ramon);

/**@brief Get contents of NRF_POWER->RAMON register, indicates power status of ram blocks.
 *
 * @param[out] p_ramon Content of NRF_POWER->RAMON register.
 *
 * @return ::NRF_SUCCESS
 */
uint32_t SVCALL(SVC_POWER_RAMON_GET) nrf_power_ramon_get(uint32_t * p_ramon);

/**@brief Request the high frequency crystal oscillator.
 *
 * Will start the high frequency crystal oscillator, the startup time of the crystal varies
 * and the ::nrf_clock_hfclk_is_running function can be polled to check if it has started.
 *
 * @see nrf_clock_hfclk_is_running
 * @see nrf_clock_hfclk_release
 *
 * @return ::NRF_SUCCESS
 */
uint32_t SVCALL(SVC_CLOCK_HFCLK_REQUEST) nrf_clock_hfclk_request(void);

/**@brief Releases the high frequency crystal oscillator.
 *
 * Will stop the high frequency crystal oscillator, this happens immediately.
 *
 * @see nrf_clock_hfclk_is_running
 * @see nrf_clock_hfclk_request
 *
 * @return ::NRF_SUCCESS
 */
uint32_t SVCALL(SVC_CLOCK_HFCLK_RELEASE) nrf_clock_hfclk_release(void);

/**@brief Checks if the high frequency crystal oscillator is running.
 *
 * @see nrf_clock_hfclk_request
 * @see nrf_clock_hfclk_release
 *
 * @param[out] p_is_running 1 if the external crystal oscillator is running, 0 if not.
 *
 * @return ::NRF_SUCCESS
 */
uint32_t SVCALL(SVC_CLOCK_HFCLK_IS_RUNNING) nrf_clock_hfclk_is_running(uint32_t * p_is_running);

/**@brief Waits for an application event.
 * 
 * An application event is either an application interrupt or a pended interrupt when the
 * interrupt is disabled. When the interrupt is enabled it will be taken immediately since
 * this function will wait in thread mode, then the execution will return in the application's
 * main thread. When an interrupt is disabled and gets pended it will return to the application's 
 * thread main. The application must ensure that the pended flag is cleared using 
 * ::nrf_nvic_ClearPendingIRQ in order to sleep using this function. This is only necessary for
 * disabled interrupts, as the interrupt handler will clear the pending flag automatically for
 * enabled interrupts.
 *
 * In order to wake up from disabled interrupts, the SEVONPEND flag has to be set in the Cortex-M0
 * System Control Register (SCR). @sa CMSIS_SCB
 *
 * @note If an application interrupt has happened since the last time nrf_wait_for_app_event was
 *       called this function will return immediately and not go to sleep. This is to avoid race
 *       conditions that can occur when a flag is updated in the interrupt handler and processed
 *       in the main loop.
 *
 * @post An application interrupt has happened or a interrupt pending flag is set.
 *
 * @return ::NRF_SUCCESS
 */
uint32_t SVCALL(SVC_NRF_WAIT_FOR_APP_EVENT) nrf_wait_for_app_event(void);

/**@brief Get PPI channel enable register contents.
 *
 * @param[out] p_channel_enable The contents of the PPI CHEN register.
 *
 * @return ::NRF_SUCCESS
 */
uint32_t SVCALL(SVC_PPI_CHANNEL_ENABLE_GET) nrf_ppi_channel_enable_get(uint32_t * p_channel_enable);

/**@brief Set PPI channel enable register.
 *
 * @param[in] channel_enable_set_msk Mask containing the bits to set in the PPI CHEN register.
 *
 * @return ::NRF_SUCCESS
 */
uint32_t SVCALL(SVC_PPI_CHANNEL_ENABLE_SET) nrf_ppi_channel_enable_set(uint32_t channel_enable_set_msk);

/**@brief Clear PPI channel enable register.
 *
 * @param[in] channel_enable_clr_msk Mask containing the bits to clear in the PPI CHEN register.
 *
 * @return ::NRF_SUCCESS
 */
uint32_t SVCALL(SVC_PPI_CHANNEL_ENABLE_CLR) nrf_ppi_channel_enable_clr(uint32_t channel_enable_clr_msk);

/**@brief Assign endpoints to a PPI channel.
 *
 * @param[in] channel_num Number of the PPI channel to assign.
 * @param[in] event_endpoint Event endpoint of the PPI channel.
 * @param[in] task_endpoint Task endpoint of the PPI channel.
 *
 * @return ::NRF_ERROR_SOC_PPI_INVALID_CHANNEL - If the channel number is invalid.
 * @return ::NRF_SUCCESS - On success.
 */
uint32_t SVCALL(SVC_PPI_CHANNEL_ASSIGN) nrf_ppi_channel_assign(uint8_t channel_num, const volatile void * event_endpoint, const volatile void * task_endpoint);

/**@brief Task to enable a channel group.
 *
 * @param[in] group_num Number of the channel group.
 *
 * @return ::NRF_ERROR_SOC_PPI_INVALID_GROUP - If the group number is invalid
 * @return ::NRF_SUCCESS - On success
 */
uint32_t SVCALL(SVC_PPI_GROUP_TASK_ENABLE) nrf_ppi_group_task_enable(uint8_t group_num);

/**@brief Task to disable a channel group.
 *
 * @param[in] group_num Number of the PPI group.
 *
 * @return ::NRF_ERROR_SOC_PPI_INVALID_GROUP - If the group number is invalid.
 * @return ::NRF_SUCCESS - On success.
 */
uint32_t SVCALL(SVC_PPI_GROUP_TASK_DISABLE) nrf_ppi_group_task_disable(uint8_t group_num);

/**@brief Assign PPI channels to a channel group.
 *
 * @param[in] group_num Number of the channel group.
 * @param[in] channel_msk Mask of the channels to assign to the group.
 *
 * @return ::NRF_ERROR_SOC_PPI_INVALID_GROUP - If the group number is invalid.
 * @return ::NRF_SUCCESS - On success.
 */
uint32_t SVCALL(SVC_PPI_GROUP_ASSIGN) nrf_ppi_group_assign(uint8_t group_num, uint32_t channel_msk);

/**@brief Gets the PPI channels of a channel group.
 *
 * @param[in]   group_num Number of the channel group.
 * @param[out]  p_channel_msk Mask of the channels assigned to the group.
 *
 * @return ::NRF_ERROR_SOC_PPI_INVALID_GROUP - If the group number is invalid.
 * @return ::NRF_SUCCESS - On success.
 */
uint32_t SVCALL(SVC_PPI_GROUP_GET) nrf_ppi_group_get(uint8_t group_num, uint32_t * p_channel_msk);
                                                       
#endif // NRF_SOC_H__

/**
  @}
 */
