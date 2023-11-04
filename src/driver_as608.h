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
 * @file      driver_as608.h
 * @brief     driver as608 header file
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

#ifndef DRIVER_AS608_H
#define DRIVER_AS608_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup as608_driver as608 driver function
 * @brief    as608 driver modules
 * @{
 */

/**
 * @addtogroup as608_basic_driver
 * @{
 */

/**
 * @brief as608 bool enumeration definition
 */
typedef enum
{
    AS608_BOOL_FALSE = 0x00,        /**< disable */
    AS608_BOOL_TRUE  = 0x01,        /**< enable */
} as608_bool_t;

/**
 * @brief as608 level enumeration definition
 */
typedef enum
{
    AS608_LEVEL_1 = 0x0001,        /**< level 1 */
    AS608_LEVEL_2 = 0x0002,        /**< level 2 */
    AS608_LEVEL_3 = 0x0003,        /**< level 3 */
    AS608_LEVEL_4 = 0x0004,        /**< level 4 */
    AS608_LEVEL_5 = 0x0005,        /**< level 5 */
} as608_level_t;

/**
 * @brief as608 packet size enumeration definition
 */
typedef enum
{
    AS608_PACKET_SIZE_32_BYTES  = 0x0000,        /**< 32 bytes */
    AS608_PACKET_SIZE_64_BYTES  = 0x0001,        /**< 64 bytes */
    AS608_PACKET_SIZE_128_BYTES = 0x0002,        /**< 128 bytes */
    AS608_PACKET_SIZE_256_BYTES = 0x0003,        /**< 256 bytes */
} as608_packet_size_t;

/**
 * @brief as608 buffer number enumeration definition
 */
typedef enum
{
    AS608_BUFFER_NUMBER_1 = 0x01,        /**< buffer 1 */
    AS608_BUFFER_NUMBER_2 = 0x02,        /**< buffer 2 */
} as608_buffer_number_t;

/**
 * @brief as608 sensor type enumeration definition
 */
typedef enum
{
    AS608_SENSOR_TYPE_FPC1011C = 0x0000,        /**< fpc1011c */
    AS608_SENSOR_TYPE_C500     = 0x0002,        /**< c500 */
    AS608_SENSOR_TYPE_S500     = 0x0003,        /**< s500*/
    AS608_SENSOR_TYPE_XWSEMI   = 0x0007,        /**< xwsemi */
    AS608_SENSOR_TYPE_CUSTOM   = 0x0009,        /**< custom */
} as608_sensor_type_t;

/**
 * @brief as608 burn code mode enumeration definition
 */
typedef enum
{
    AS608_BURN_CODE_MODE_INFO = 0x00,        /**< only flash info */
    AS608_BURN_CODE_MODE_FULL = 0x01,        /**< full */
} as608_burn_code_mode_t;

/**
 * @brief as608 image enumeration definition
 */
typedef enum
{
    AS608_IMAGE_BIN        = 0x00,        /**< bin image */
    AS608_IMAGE_NO_FEATURE = 0x01,        /**< no feature */
    AS608_IMAGE_FEATURE    = 0x02,        /**< feature */
} as608_image_t;

/**
 * @brief as608 gpio number enumeration definition
 */
typedef enum
{
    AS608_GPIO_NUMBER_0 = 0x00,        /**< number 0 */
    AS608_GPIO_NUMBER_1 = 0x01,        /**< number 1 */
} as608_gpio_number_t;

/**
 * @brief as608 gpio level enumeration definition
 */
typedef enum
{
    AS608_GPIO_LEVEL_LOW  = 0x00,        /**< level low */
    AS608_GPIO_LEVEL_HIGH = 0x01,        /**< level high */
} as608_gpio_level_t;

/**
 * @brief as608 status enumeration definition
 */
typedef enum
{
    AS608_STATUS_OK                          = 0x00,        /**< ok */
    AS608_STATUS_FRAME_ERROR                 = 0x01,        /**< frame error */
    AS608_STATUS_NO_FINGERPRINT              = 0x02,        /**< no fingerprint */
    AS608_STATUS_INPUT_ERROR                 = 0x03,        /**< fingerprint image error */
    AS608_STATUS_IMAGE_TOO_DRY               = 0x04,        /**< fingerprint image too dry */
    AS608_STATUS_IMAGE_TOO_WET               = 0x05,        /**< fingerprint image too wet */
    AS608_STATUS_IMAGE_TOO_CLUTTER           = 0x06,        /**< fingerprint too clutter */
    AS608_STATUS_IMAGE_TOO_FEW_FEATURE       = 0x07,        /**< fingerprint feature too few */
    AS608_STATUS_NOT_MATCH                   = 0x08,        /**< not match */
    AS608_STATUS_NOT_FOUND                   = 0x09,        /**< not found */
    AS608_STATUS_FEATURE_COMBINE_ERROR       = 0x0A,        /**< feature combine error */
    AS608_STATUS_LIB_ADDR_OVER               = 0x0B,        /**< fingerprint lib addr is over */
    AS608_STATUS_LIB_READ_ERROR              = 0x0C,        /**< fingerprint lib read error */
    AS608_STATUS_UPLOAD_FEATURE_ERROR        = 0x0D,        /**< upload feature error */
    AS608_STATUS_NO_FRAME                    = 0x0E,        /**< no frame */
    AS608_STATUS_UPLOAD_IMAGE_ERROR          = 0x0F,        /**< upload image error */
    AS608_STATUS_LIB_DELETE_ERROR            = 0x10,        /**< delete lib error */
    AS608_STATUS_LIB_CLEAR_ERROR             = 0x11,        /**< clear lib error */
    AS608_STATUS_ENTER_LOW_POWER_ERROR       = 0x12,        /**< enter low power error */
    AS608_STATUS_COMMAND_INVALID             = 0x13,        /**< command invalid */
    AS608_STATUS_RESET_ERROR                 = 0x14,        /**< reset error */
    AS608_STATUS_BUFFER_INVALID              = 0x15,        /**< buffer invalid */
    AS608_STATUS_UPDATE_ERROR                = 0x16,        /**< update error */
    AS608_STATUS_NO_MOVE                     = 0x17,        /**< no move */
    AS608_STATUS_FLASH_ERROR                 = 0x18,        /**< flash error */
    AS608_STATUS_F0_RESPONSE                 = 0xF0,        /**< f0 response */
    AS608_STATUS_F1_RESPONSE                 = 0xF1,        /**< f1 response */
    AS608_STATUS_FLASH_WRITE_SUM_ERROR       = 0xF2,        /**< flash sum error */
    AS608_STATUS_FLASH_WRITE_HEADER_ERROR    = 0xF3,        /**< flash header error */
    AS608_STATUS_FLASH_WRITE_LENGTH_ERROR    = 0xF4,        /**< flash length error */
    AS608_STATUS_FLASH_WRITE_LENGTH_TOO_LONG = 0xF5,        /**< flash length too long */
    AS608_STATUS_FLASH_WRITE_ERROR           = 0xF6,        /**< flash write error */
    AS608_STATUS_UNKNOWN                     = 0x19,        /**< unknown */
    AS608_STATUS_REG_INVALID                 = 0x1A,        /**< reg invalid */
    AS608_STATUS_DATA_INVALID                = 0x1B,        /**< data invalid */
    AS608_STATUS_NOTE_PAGE_INVALID           = 0x1C,        /**< note page invalid */
    AS608_STATUS_PORT_INVALID                = 0x1D,        /**< port invalid */
    AS608_STATUS_ENROOL_ERROR                = 0x1E,        /**< enrool error */
    AS608_STATUS_LIB_FULL                    = 0x1F,        /**< lib full */
} as608_status_t;

/**
 * @brief as608 params structure definition
 */
typedef struct as608_params_s
{
    uint16_t status;                        /**< current status */
    as608_sensor_type_t sensor_type;        /**< sensor type */
    uint16_t fingerprint_size;              /**< fingerprint size */
    as608_level_t level;                    /**< level */
    uint32_t address;                       /**< device address */
    as608_packet_size_t packet_size;        /**< packet size */
    uint16_t n_9600;                        /**< n times of 9600 */
} as608_params_t;

/**
 * @brief as608 handle structure definition
 */
typedef struct as608_handle_s
{
    uint8_t (*uart_init)(void);                               /**< point to a uart_init function address */
    uint8_t (*uart_deinit)(void);                             /**< point to a uart_deinit function address */
    uint16_t (*uart_read)(uint8_t *buf, uint16_t len);        /**< point to a uart_read function address */
    uint8_t (*uart_flush)(void);                              /**< point to a uart_flush function address */
    uint8_t (*uart_write)(uint8_t *buf, uint16_t len);        /**< point to a uart_write function address */
    void (*delay_ms)(uint32_t ms);                            /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);          /**< point to a debug_print function address */
    uint8_t inited;                                           /**< inited flag */
    uint8_t buf[384];                                         /**< frame buf */
    uint8_t status;                                           /**< status */
    uint16_t packet_size;                                     /**< packet size */
} as608_handle_t;

/**
 * @brief as608 information structure definition
 */
typedef struct as608_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} as608_info_t;

/**
 * @}
 */

/**
 * @defgroup as608_link_driver as608 link driver function
 * @brief    as608 link driver modules
 * @ingroup  as608_driver
 * @{
 */

/**
 * @brief     initialize as608_handle_t structure
 * @param[in] HANDLE points to an as608 handle structure
 * @param[in] STRUCTURE is as608_handle_t
 * @note      none
 */
#define DRIVER_AS608_LINK_INIT(HANDLE, STRUCTURE)   memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link uart_init function
 * @param[in] HANDLE points to an as608 handle structure
 * @param[in] FUC points to a uart_init function address
 * @note      none
 */
#define DRIVER_AS608_LINK_UART_INIT(HANDLE, FUC)   (HANDLE)->uart_init = FUC

/**
 * @brief     link uart_deinit function
 * @param[in] HANDLE points to an as608 handle structure
 * @param[in] FUC points to a uart_deinit function address
 * @note      none
 */
#define DRIVER_AS608_LINK_UART_DEINIT(HANDLE, FUC) (HANDLE)->uart_deinit = FUC

/**
 * @brief     link uart_read function
 * @param[in] HANDLE points to an as608 handle structure
 * @param[in] FUC points to a uart_read function address
 * @note      none
 */
#define DRIVER_AS608_LINK_UART_READ(HANDLE, FUC)   (HANDLE)->uart_read = FUC

/**
 * @brief     link uart_write function
 * @param[in] HANDLE points to an as608 handle structure
 * @param[in] FUC points to a uart_write function address
 * @note      none
 */
#define DRIVER_AS608_LINK_UART_WRITE(HANDLE, FUC)  (HANDLE)->uart_write = FUC

/**
 * @brief     link uart_flush function
 * @param[in] HANDLE points to an as608 handle structure
 * @param[in] FUC points to a uart_flush function address
 * @note      none
 */
#define DRIVER_AS608_LINK_UART_FLUSH(HANDLE, FUC)  (HANDLE)->uart_flush = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE points to an as608 handle structure
 * @param[in] FUC points to a delay_ms function address
 * @note      none
 */
#define DRIVER_AS608_LINK_DELAY_MS(HANDLE, FUC)    (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE points to an as608 handle structure
 * @param[in] FUC points to a debug_print function address
 * @note      none
 */
#define DRIVER_AS608_LINK_DEBUG_PRINT(HANDLE, FUC) (HANDLE)->debug_print = FUC

/**
 * @}
 */

/**
 * @defgroup as608_basic_driver as608 basic driver function
 * @brief    as608 basic driver modules
 * @ingroup  as608_driver
 * @{
 */

/**
 * @brief      get chip's information
 * @param[out] *info points to an as608 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t as608_info(as608_info_t *info);

/**
 * @brief     initialize the chip
 * @param[in] *handle points to an as608 handle structure
 * @param[in] addr is the chip address
 * @return    status code
 *            - 0 success
 *            - 1 uart initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 *            - 4 read params failed
 * @note      none
 */
uint8_t as608_init(as608_handle_t *handle, uint32_t addr);

/**
 * @brief     close the chip
 * @param[in] *handle points to an as608 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 uart deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t as608_deinit(as608_handle_t *handle);

/**
 * @brief      get the last status
 * @param[in]  *handle points to an as608 handle structure
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t as608_get_last_status(as608_handle_t *handle, as608_status_t *status);

/**
 * @brief     print status
 * @param[in] *handle points to an as608 handle structure
 * @param[in] status is the print status
 * @return    status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t as608_print_status(as608_handle_t *handle, as608_status_t status);

/**
 * @brief      get image
 * @param[in]  *handle points to an as608 handle structure
 * @param[in]  addr is the chip address
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get image failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 decode failed
 *             - 5 addr is invalid
 *             - 6 len is invalid
 * @note       none
 */
uint8_t as608_get_image(as608_handle_t *handle, uint32_t addr, as608_status_t *status);

/**
 * @brief      generate feature
 * @param[in]  *handle points to an as608 handle structure
 * @param[in]  addr is the chip address
 * @param[in]  num is the buffer number
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 generate feature failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 decode failed
 *             - 5 addr is invalid
 *             - 6 len is invalid
 * @note       none
 */
uint8_t as608_generate_feature(as608_handle_t *handle, uint32_t addr, as608_buffer_number_t num, as608_status_t *status);

/**
 * @brief      match feature
 * @param[in]  *handle points to an as608 handle structure
 * @param[in]  addr is the chip address
 * @param[out] *score points to a score buffer
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 match feature failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 decode failed
 *             - 5 addr is invalid
 *             - 6 len is invalid
 * @note       none
 */
uint8_t as608_match_feature(as608_handle_t *handle, uint32_t addr, uint16_t *score, as608_status_t *status);

/**
 * @brief      search feature
 * @param[in]  *handle points to an as608 handle structure
 * @param[in]  addr is the chip address
 * @param[in]  num is the buffer num
 * @param[in]  start_page is the start page
 * @param[in]  page_number is the page number
 * @param[out] *found_page points to a found page buffer
 * @param[out] *score points to a score buffer
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 search feature failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 decode failed
 *             - 5 addr is invalid
 *             - 6 len is invalid
 * @note       none
 */
uint8_t as608_search_feature(as608_handle_t *handle, uint32_t addr, as608_buffer_number_t num, 
                             uint16_t start_page, uint16_t page_number, uint16_t *found_page, 
                             uint16_t *score, as608_status_t *status);

/**
 * @brief      combine feature
 * @param[in]  *handle points to an as608 handle structure
 * @param[in]  addr is the chip address
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 combine feature failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 decode failed
 *             - 5 addr is invalid
 *             - 6 len is invalid
 * @note       none
 */
uint8_t as608_combine_feature(as608_handle_t *handle, uint32_t addr, as608_status_t *status);

/**
 * @brief      store feature
 * @param[in]  *handle points to an as608 handle structure
 * @param[in]  addr is the chip address
 * @param[in]  num is the buffer num
 * @param[in]  page_number is the page number
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 store feature failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 decode failed
 *             - 5 addr is invalid
 *             - 6 len is invalid
 * @note       none
 */
uint8_t as608_store_feature(as608_handle_t *handle, uint32_t addr, as608_buffer_number_t num, 
                            uint16_t page_number, as608_status_t *status);

/**
 * @brief      load feature
 * @param[in]  *handle points to an as608 handle structure
 * @param[in]  addr is the chip address
 * @param[in]  num is the buffer num
 * @param[in]  page_number is the page number
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 load feature failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 decode failed
 *             - 5 addr is invalid
 *             - 6 len is invalid
 * @note       none
 */
uint8_t as608_load_feature(as608_handle_t *handle, uint32_t addr, as608_buffer_number_t num, 
                           uint16_t page_number, as608_status_t *status);

/**
 * @brief         upload feature
 * @param[in]     *handle points to an as608 handle structure
 * @param[in]     addr is the chip address
 * @param[in]     num is the buffer num
 * @param[out]    *output_buffer points to an output buffer
 * @param[in,out] *output_len points to an output length buffer
 * @param[out]    *status points to a status buffer
 * @return        status code
 *                - 0 success
 *                - 1 upload feature failed
 *                - 2 handle is NULL
 *                - 3 handle is not initialized
 *                - 4 decode failed
 *                - 5 addr is invalid
 *                - 6 len is invalid
 *                - 7 buffer is full
 * @note          none
 */
uint8_t as608_upload_feature(as608_handle_t *handle, uint32_t addr, as608_buffer_number_t num, 
                             uint8_t *output_buffer, uint16_t *output_len, as608_status_t *status);

/**
 * @brief      download feature
 * @param[in]  *handle points to an as608 handle structure
 * @param[in]  addr is the chip address
 * @param[in]  num is the buffer num
 * @param[in]  *input_buffer points to an input buffer
 * @param[in]  input_len is the input length
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 download feature failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 decode failed
 *             - 5 addr is invalid
 *             - 6 len is invalid
 * @note       none
 */
uint8_t as608_download_feature(as608_handle_t *handle, uint32_t addr, as608_buffer_number_t num, 
                               uint8_t *input_buffer, uint16_t input_len, as608_status_t *status);

/**
 * @brief         upload image
 * @param[in]     *handle points to an as608 handle structure
 * @param[in]     addr is the chip address
 * @param[out]    *output_buffer points to an output buffer
 * @param[in,out] *output_len points to an output length buffer
 * @param[out]    *status points to a status buffer
 * @return        status code
 *                - 0 success
 *                - 1 upload image failed
 *                - 2 handle is NULL
 *                - 3 handle is not initialized
 *                - 4 decode failed
 *                - 5 addr is invalid
 *                - 6 len is invalid
 *                - 7 buffer is full
 * @note          none
 */
uint8_t as608_upload_image(as608_handle_t *handle, uint32_t addr, uint8_t *output_buffer, 
                           uint16_t *output_len, as608_status_t *status);

/**
 * @brief      download image
 * @param[in]  *handle points to an as608 handle structure
 * @param[in]  addr is the chip address
 * @param[in]  *input_buffer points to an input buffer
 * @param[in]  input_len is the input length
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 download image failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 decode failed
 *             - 5 addr is invalid
 *             - 6 len is invalid
 * @note       none
 */
uint8_t as608_download_image(as608_handle_t *handle, uint32_t addr, uint8_t *input_buffer,
                             uint16_t input_len, as608_status_t *status);

/**
 * @brief      delete feature
 * @param[in]  *handle points to an as608 handle structure
 * @param[in]  addr is the chip address
 * @param[in]  page_number is the page number
 * @param[in]  number is the deleted number
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 delete feature failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 decode failed
 *             - 5 addr is invalid
 *             - 6 len is invalid
 * @note       none
 */
uint8_t as608_delete_feature(as608_handle_t *handle, uint32_t addr, uint16_t page_number, uint16_t number, as608_status_t *status);

/**
 * @brief      empty all feature
 * @param[in]  *handle points to an as608 handle structure
 * @param[in]  addr is the chip address
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 empty all feature failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 decode failed
 *             - 5 addr is invalid
 *             - 6 len is invalid
 * @note       none
 */
uint8_t as608_empty_all_feature(as608_handle_t *handle, uint32_t addr, as608_status_t *status);

/**
 * @brief      set baud rate
 * @param[in]  *handle points to an as608 handle structure
 * @param[in]  addr is the chip address
 * @param[in]  n_9600 is the n times of 9600
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 set baud rate failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 decode failed
 *             - 5 addr is invalid
 *             - 6 len is invalid
 * @note       none
 */
uint8_t as608_set_baud_rate(as608_handle_t *handle, uint32_t addr, uint8_t n_9600, as608_status_t *status);

/**
 * @brief      set level
 * @param[in]  *handle points to an as608 handle structure
 * @param[in]  addr is the chip address
 * @param[in]  level is the set level
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 set level failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 decode failed
 *             - 5 addr is invalid
 *             - 6 len is invalid
 * @note       none
 */
uint8_t as608_set_level(as608_handle_t *handle, uint32_t addr, as608_level_t level, as608_status_t *status);

/**
 * @brief      set packet size
 * @param[in]  *handle points to an as608 handle structure
 * @param[in]  addr is the chip address
 * @param[in]  size is the set packet size
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 set packet size failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 decode failed
 *             - 5 addr is invalid
 *             - 6 len is invalid
 * @note       none
 */
uint8_t as608_set_packet_size(as608_handle_t *handle, uint32_t addr, as608_packet_size_t size, as608_status_t *status);

/**
 * @brief      get params
 * @param[in]  *handle points to an as608 handle structure
 * @param[in]  addr is the chip address
 * @param[out] *param points to a param structure
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get params failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 decode failed
 *             - 5 addr is invalid
 *             - 6 len is invalid
 * @note       none
 */
uint8_t as608_get_params(as608_handle_t *handle, uint32_t addr, as608_params_t *param, as608_status_t *status);

/**
 * @brief      enroll
 * @param[in]  *handle points to an as608 handle structure
 * @param[in]  addr is the chip address
 * @param[out] *page_number points to a page number buffer
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 enroll failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 decode failed
 *             - 5 addr is invalid
 *             - 6 len is invalid
 * @note       none
 */
uint8_t as608_enroll(as608_handle_t *handle, uint32_t addr, uint16_t *page_number, as608_status_t *status);

/**
 * @brief      identify
 * @param[in]  *handle points to an as608 handle structure
 * @param[in]  addr is the chip address
 * @param[out] *page_number points to a page number buffer
 * @param[out] *score points to a score buffer
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 identify failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 decode failed
 *             - 5 addr is invalid
 *             - 6 len is invalid
 * @note       none
 */
uint8_t as608_identify(as608_handle_t *handle, uint32_t addr, uint16_t *page_number, uint16_t *score, as608_status_t *status);

/**
 * @brief      set password
 * @param[in]  *handle points to an as608 handle structure
 * @param[in]  addr is the chip address
 * @param[in]  password is the set password
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 set password failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 decode failed
 *             - 5 addr is invalid
 *             - 6 len is invalid
 * @note       none
 */
uint8_t as608_set_password(as608_handle_t *handle, uint32_t addr, uint32_t password, as608_status_t *status);

/**
 * @brief      verify password
 * @param[in]  *handle points to an as608 handle structure
 * @param[in]  addr is the chip address
 * @param[in]  password is the verify password
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 verify password failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 decode failed
 *             - 5 addr is invalid
 *             - 6 len is invalid
 * @note       none
 */
uint8_t as608_verify_password(as608_handle_t *handle, uint32_t addr, uint32_t password, as608_status_t *status);

/**
 * @brief      get random
 * @param[in]  *handle points to an as608 handle structure
 * @param[in]  addr is the chip address
 * @param[out] *randn points to a random buffer
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get random failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 decode failed
 *             - 5 addr is invalid
 *             - 6 len is invalid
 * @note       none
 */
uint8_t as608_get_random(as608_handle_t *handle, uint32_t addr, uint32_t *randn, as608_status_t *status);

/**
 * @brief      set the chip address
 * @param[in]  *handle points to an as608 handle structure
 * @param[in]  addr is the chip address
 * @param[in ] new_addr is the new address
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 set chip address failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 decode failed
 *             - 5 addr is invalid
 *             - 6 len is invalid
 * @note       none
 */
uint8_t as608_set_chip_address(as608_handle_t *handle, uint32_t addr, uint32_t new_addr, as608_status_t *status);

/**
 * @brief         get flash information
 * @param[in]     *handle points to an as608 handle structure
 * @param[in]     addr is the chip address
 * @param[out]    *output_buffer points to an output buffer
 * @param[in,out] *output_len points to an output length buffer
 * @param[out]    *status points to a status buffer
 * @return        status code
 *                - 0 success
 *                - 1 get flash information failed
 *                - 2 handle is NULL
 *                - 3 handle is not initialized
 *                - 4 decode failed
 *                - 5 addr is invalid
 *                - 6 len is invalid
 *                - 7 buffer is full
 * @note          none
 */
uint8_t as608_get_flash_information(as608_handle_t *handle, uint32_t addr,
                                    uint8_t *output_buffer, uint16_t *output_len, as608_status_t *status);

/**
 * @brief      enable or disable port
 * @param[in]  *handle points to an as608 handle structure
 * @param[in]  addr is the chip address
 * @param[in ] enable is a bool value
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 set port failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 decode failed
 *             - 5 addr is invalid
 *             - 6 len is invalid
 * @note       none
 */
uint8_t as608_set_port(as608_handle_t *handle, uint32_t addr, as608_bool_t enable, as608_status_t *status);

/**
 * @brief      write notepad
 * @param[in]  *handle points to an as608 handle structure
 * @param[in]  addr is the chip address
 * @param[in]  page_number is the page number
 * @param[in]  *data points to an input buffer
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 write notepad failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 decode failed
 *             - 5 addr is invalid
 *             - 6 len is invalid
 *             - 7 page_number > 16
 * @note       0 <= page_number <= 15
 */
uint8_t as608_write_notepad(as608_handle_t *handle, uint32_t addr, uint8_t page_number, uint8_t data[32], as608_status_t *status);

/**
 * @brief      read notepad
 * @param[in]  *handle points to an as608 handle structure
 * @param[in]  addr is the chip address
 * @param[in]  page_number is the page number
 * @param[out] *data points to an output buffer
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 read notepad failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 decode failed
 *             - 5 addr is invalid
 *             - 6 len is invalid
 *             - 7 page_number > 16
 * @note       0 <= page_number <= 15
 */
uint8_t as608_read_notepad(as608_handle_t *handle, uint32_t addr, uint8_t page_number, uint8_t data[32], as608_status_t *status);

/**
 * @brief      burn code
 * @param[in]  *handle points to an as608 handle structure
 * @param[in]  addr is the chip address
 * @param[in]  mode is the burn mode
 * @param[in]  *input_buffer points to an input buffer
 * @param[in]  input_len is the input length
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 burn code failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 decode failed
 *             - 5 addr is invalid
 *             - 6 len is invalid
 * @note       none
 */
uint8_t as608_burn_code(as608_handle_t *handle, uint32_t addr, as608_burn_code_mode_t mode, 
                        uint8_t *input_buffer, uint16_t input_len, as608_status_t *status);

/**
 * @brief      high speed search
 * @param[in]  *handle points to an as608 handle structure
 * @param[in]  addr is the chip address
 * @param[in]  buffer_number is the buffer number
 * @param[in]  start_page is the start page
 * @param[in]  page_number is the page number
 * @param[out] *found_page points to a found page buffer
 * @param[out] *score points to a score buffer
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 high speed search failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 decode failed
 *             - 5 addr is invalid
 *             - 6 len is invalid
 * @note       none
 */
uint8_t as608_high_speed_search(as608_handle_t *handle, uint32_t addr, as608_buffer_number_t buffer_number, 
                                uint16_t start_page, uint16_t page_number, 
                                uint16_t *found_page, uint16_t *score,
                                as608_status_t *status);

/**
 * @brief      generate bin image
 * @param[in]  *handle points to an as608 handle structure
 * @param[in]  addr is the chip address
 * @param[in]  image is the image type
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 generate bin image failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 decode failed
 *             - 5 addr is invalid
 *             - 6 len is invalid
 * @note       none
 */
uint8_t as608_generate_bin_image(as608_handle_t *handle, uint32_t addr, as608_image_t image, as608_status_t *status);

/**
 * @brief      get valid template number
 * @param[in]  *handle points to an as608 handle structure
 * @param[in]  addr is the chip address
 * @param[out] *num points to a number buffer
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get valid template number failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 decode failed
 *             - 5 addr is invalid
 *             - 6 len is invalid
 * @note       none
 */
uint8_t as608_get_valid_template_number(as608_handle_t *handle, uint32_t addr, uint16_t *num, as608_status_t *status);

/**
 * @brief      set gpio level
 * @param[in]  *handle points to an as608 handle structure
 * @param[in]  addr is the chip address
 * @param[in]  gpio is the gpio number
 * @param[in]  input_level is the input level
 * @param[out] *output_level points to an output level buffer
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 set gpio level failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 decode failed
 *             - 5 addr is invalid
 *             - 6 len is invalid
 * @note       none
 */
uint8_t as608_set_gpio_level(as608_handle_t *handle, uint32_t addr, as608_gpio_number_t gpio, 
                             as608_gpio_level_t input_level, as608_gpio_level_t *output_level, as608_status_t *status);

/**
 * @brief      get index table
 * @param[in]  *handle points to an as608 handle structure
 * @param[in]  addr is the chip address
 * @param[in]  num is the index number
 * @param[out] *table points to a table buffer
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get index table failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 decode failed
 *             - 5 addr is invalid
 *             - 6 len is invalid
 *             - 7 num > 3
 * @note       none
 */
uint8_t as608_get_index_table(as608_handle_t *handle, uint32_t addr, uint8_t num, 
                              uint8_t table[32], as608_status_t *status);

/**
 * @}
 */

/**
 * @defgroup as608_extern_driver as608 extern driver function
 * @brief    as608 extern driver modules
 * @ingroup  as608_driver
 * @{
 */

/**
 * @brief         write read data to chip
 * @param[in]     *handle points to an as608 handle structure
 * @param[in]     addr is the chip address
 * @param[in]     type is the frame type
 * @param[in]     *input_buffer points to an input buffer
 * @param[in]     input_len is the input length
 * @param[in]     ms is the delay in ms
 * @param[out]    *output_buffer points to an output buffer
 * @param[in,out] *output_len points to an output length buffer
 * @return        status code
 *                - 0 success
 *                - 1 command write read failed
 *                - 2 handle is NULL
 *                - 3 handle is not initialized
 *                - 4 decode error
 *                - 5 addr is invalid
 * @note          none
 */
uint8_t as608_command_write_read(as608_handle_t *handle, uint32_t addr, uint8_t type, 
                                 uint8_t *input_buffer, uint16_t input_len, 
                                 uint16_t ms, uint8_t *output_buffer, uint16_t *output_len);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
