/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_as608_basic.h
 * @brief     driver as608 basic header file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2023-09-30
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2023/09/30  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#ifndef DRIVER_AS608_BASIC_H
#define DRIVER_AS608_BASIC_H

#include "driver_as608_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup as608_example_driver as608 example driver function
 * @brief    as608 example driver modules
 * @ingroup  as608_driver
 * @{
 */

/**
 * @brief as608 basic send command configure
 */
#ifndef AS608_BASIC_SEND_CONFIG
    #define AS608_BASIC_SEND_CONFIG      0        /**< don't send */
#endif

/**
 * @brief as608 basic example default definition
 */
#define AS608_BASIC_DEFAULT_PORT                AS608_BOOL_TRUE                      /**< enable */
#define AS608_BASIC_DEFAULT_BAUD_RATE           6                                    /**< 57600 bps */
#define AS608_BASIC_DEFAULT_LEVEL               AS608_LEVEL_3                        /**< level 3 */
#define AS608_BASIC_DEFAULT_PACKET_SIZE         AS608_PACKET_SIZE_128_BYTES          /**< 128 bytes */
#define AS608_BASIC_DEFAULT_PASSWORD            0x00000000                           /**< 0x00000000 */
#define AS608_BASIC_DEFAULT_ADDRESS             0xFFFFFFFF                           /**< 0xFFFFFFFF */
#define AS608_BASIC_DEFAULT_FEATURE             AS608_BUFFER_NUMBER_1                /**< buffer number 1 */
#define AS608_BASIC_DEFAULT_TIMEOUT             10                                   /**< 10s */

/**
 * @brief     basic example init
 * @param[in] addr is the chip address
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t as608_basic_init(uint32_t addr);

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t as608_basic_deinit(void);

/**
 * @brief     basic example print status
 * @param[in] status is the print status
 * @return    status code
 *             - 0 success
 *             - 1 print failed
 * @note       none
 */
uint8_t as608_basic_print_status(as608_status_t status);

/**
 * @brief      basic example input fingerprint
 * @param[out] *callback points to a callback function
 * @param[out] *score points to a score buffer
 * @param[out] *page_number points to a page number buffer
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 input fingerprint failed
 *             - 2 timeout
 * @note       callback status
 *             - -1 error
 *             - 0 please put your finger on the sensor
 *             - 1 please put your finger on the sensor again
 *             - 2 generate feature success
 */
uint8_t as608_basic_input_fingerprint(void (*callback)(int8_t status, const char *const fmt, ...), 
                                      uint16_t *score,
                                      uint16_t *page_number,
                                      as608_status_t *status);

/**
 * @brief      basic example verify
 * @param[out] *found_page points to a found page buffer
 * @param[out] *score points to a score buffer
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 verify failed
 * @note       none
 */
uint8_t as608_basic_verify(uint16_t *found_page, uint16_t *score, as608_status_t *status);

/**
 * @brief      basic example high speed verify
 * @param[out] *found_page points to a found page buffer
 * @param[out] *score points to a score buffer
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 high speed verify failed
 * @note       none
 */
uint8_t as608_basic_high_speed_verify(uint16_t *found_page, uint16_t *score, as608_status_t *status);

/**
 * @brief      basic example delete fingerprint
 * @param[in]  page_number is the page number
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 delete fingerprint failed
 * @note       none
 */
uint8_t as608_basic_delete_fingerprint(uint16_t page_number, as608_status_t *status);

/**
 * @brief      basic example empty fingerprint
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 empty fingerprint failed
 * @note       none
 */
uint8_t as608_basic_empty_fingerprint(as608_status_t *status);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
