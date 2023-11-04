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
 * @file      driver_as608_advance.c
 * @brief     driver as608 advance source file
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

#include "driver_as608_advance.h"

static as608_handle_t gs_handle;        /**< as608 handle */
static uint32_t gs_addr = 0;            /**< as608 address */

/**
 * @brief     advance example init
 * @param[in] addr is the chip address
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t as608_advance_init(uint32_t addr)
{
    uint8_t res;

    /* link interface function */
    DRIVER_AS608_LINK_INIT(&gs_handle, as608_handle_t);
    DRIVER_AS608_LINK_UART_INIT(&gs_handle, as608_interface_uart_init);
    DRIVER_AS608_LINK_UART_DEINIT(&gs_handle, as608_interface_uart_deinit);
    DRIVER_AS608_LINK_UART_READ(&gs_handle, as608_interface_uart_read);
    DRIVER_AS608_LINK_UART_WRITE(&gs_handle, as608_interface_uart_write);
    DRIVER_AS608_LINK_UART_FLUSH(&gs_handle, as608_interface_uart_flush);
    DRIVER_AS608_LINK_DELAY_MS(&gs_handle, as608_interface_delay_ms);
    DRIVER_AS608_LINK_DEBUG_PRINT(&gs_handle, as608_interface_debug_print);

    /* as608 init */
    res = as608_init(&gs_handle, addr);
    if (res != 0)
    {
        as608_interface_debug_print("as608: init failed.\n");

        return 1;
    }
#if (AS608_ADVANCE_SEND_CONFIG == 1)
    /* set default port */
    res = as608_set_port(&gs_handle, addr, AS608_ADVANCE_DEFAULT_PORT, &status);
    if (res != 0)
    {
        as608_interface_debug_print("as608: set port failed.\n");
        (void)as608_deinit(&gs_handle);

        return 1;
    }
    if (status != AS608_STATUS_OK)
    {
        as608_interface_debug_print("as608: status not ok.\n");
        (void)as608_deinit(&gs_handle);

        return 1;
    }

    /* set default baud rate */
    res = as608_set_baud_rate(&gs_handle, addr, AS608_ADVANCE_DEFAULT_BAUD_RATE, &status);
    if (res != 0)
    {
        as608_interface_debug_print("as608: set baud rate failed.\n");
        (void)as608_deinit(&gs_handle);

        return 1;
    }
    if (status != AS608_STATUS_OK)
    {
        as608_interface_debug_print("as608: status not ok.\n");
        (void)as608_deinit(&gs_handle);

        return 1;
    }

    /* set default level */
    res = as608_set_level(&gs_handle, addr, AS608_ADVANCE_DEFAULT_LEVEL, &status);
    if (res != 0)
    {
        as608_interface_debug_print("as608: set level failed.\n");
        (void)as608_deinit(&gs_handle);

        return 1;
    }
    if (status != AS608_STATUS_OK)
    {
        as608_interface_debug_print("as608: status not ok.\n");
        (void)as608_deinit(&gs_handle);

        return 1;
    }

    /* set default packet size */
    res = as608_set_packet_size(&gs_handle, addr, AS608_ADVANCE_DEFAULT_PACKET_SIZE, &status);
    if (res != 0)
    {
        as608_interface_debug_print("as608: set packet size failed.\n");
        (void)as608_deinit(&gs_handle);

        return 1;
    }
    if (status != AS608_STATUS_OK)
    {
        as608_interface_debug_print("as608: status not ok.\n");
        (void)as608_deinit(&gs_handle);

        return 1;
    }

    /* set default password */
    res = as608_set_password(&gs_handle, addr, AS608_ADVANCE_DEFAULT_PASSWORD, &status);
    if (res != 0)
    {
        as608_interface_debug_print("as608: set password failed.\n");
        (void)as608_deinit(&gs_handle);

        return 1;
    }
    if (status != AS608_STATUS_OK)
    {
        as608_interface_debug_print("as608: status not ok.\n");
        (void)as608_deinit(&gs_handle);

        return 1;
    }

    /* verify default password */
    res = as608_verify_password(&gs_handle, addr, AS608_ADVANCE_DEFAULT_PASSWORD, &status);
    if (res != 0)
    {
        as608_interface_debug_print("as608: verify password failed.\n");
        (void)as608_deinit(&gs_handle);

        return 1;
    }
    if (status != AS608_STATUS_OK)
    {
        as608_interface_debug_print("as608: status not ok.\n");
        (void)as608_deinit(&gs_handle);

        return 1;
    }

    /* set default chip address */
    res = as608_set_chip_address(&gs_handle, addr, AS608_ADVANCE_DEFAULT_ADDRESS, &status);
    if (res != 0)
    {
        as608_interface_debug_print("as608: set chip address failed.\n");
        (void)as608_deinit(&gs_handle);

        return 1;
    }
    if (status != AS608_STATUS_OK)
    {
        as608_interface_debug_print("as608: status not ok.\n");
        (void)as608_deinit(&gs_handle);

        return 1;
    }
#endif
    /* set address */
    gs_addr = addr;

    return 0;
}

/**
 * @brief      advance example input fingerprint
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
uint8_t as608_advance_input_fingerprint(void (*callback)(int8_t status, const char *const fmt, ...),
                                        uint16_t *score,
                                        uint16_t *page_number,
                                        as608_status_t *status)
{
    uint8_t res;
    uint32_t timeout;

    /* max default time */
    timeout = AS608_ADVANCE_DEFAULT_TIMEOUT;

    /* wait your finger */
    while (timeout != 0)
    {
        /* run the callback */
        if (callback != NULL)
        {
            /* output */
            callback(0, "please put your finger on the sensor.\n");
        }

        /* get image */
        res = as608_get_image(&gs_handle, gs_addr, status);
        if (res != 0)
        {
            return 1;
        }
        if (*status == AS608_STATUS_OK)
        {
            /* generate feature */
            res = as608_generate_feature(&gs_handle, gs_addr, AS608_BUFFER_NUMBER_1, status);
            if (res != 0)
            {
                return 1;
            }
            if (*status == AS608_STATUS_OK)
            {
                /* run the callback */
                if (callback != NULL)
                {
                    /* output */
                    callback(1, "please put your finger on the sensor again.\n");
                }

                /* max default time */
                timeout = AS608_ADVANCE_DEFAULT_TIMEOUT;

                /* wait your finger */
                while (timeout != 0)
                {
                    /* get image */
                    res = as608_get_image(&gs_handle, gs_addr, status);
                    if (res != 0)
                    {
                        return 1;
                    }
                    if (*status == AS608_STATUS_OK)
                    {
                        /* generate feature */
                        res = as608_generate_feature(&gs_handle, gs_addr, AS608_BUFFER_NUMBER_2, status);
                        if (res != 0)
                        {
                            return 1;
                        }
                        if (*status == AS608_STATUS_OK)
                        {
                            /* run the callback */
                            if (callback != NULL)
                            {
                                /* output */
                                callback(2, "generate feature success.\n");
                            }

                            goto next;
                        }
                        else
                        {
                            /* run the callback */
                            if (callback != NULL)
                            {
                                /* output */
                                callback(-1, "error.\n");
                            }
                        }
                    }

                    /* delay 1000ms */
                    as608_interface_delay_ms(1000);

                    /* timeout-- */
                    timeout--;
                }

                /* check timeout */
                if (timeout == 0)
                {
                    return 2;
                }
            }
            else
            {
                /* run the callback */
                if (callback != NULL)
                {
                    /* output */
                    callback(-1, "error.\n");
                }
            }
        }

        /* delay 1000ms */
        as608_interface_delay_ms(1000);

        /* timeout-- */
        timeout--;
    }

    /* check timeout */
    if (timeout == 0)
    {
        return 2;
    }

    next:
    /* match feature */
    res = as608_match_feature(&gs_handle, gs_addr, score, status);
    if (res != 0)
    {
        return 1;
    }
    if (*status != AS608_STATUS_OK)
    {
        /* run the callback */
        if (callback != NULL)
        {
            /* output */
            callback(-1, "error.\n");
        }

        return 1;
    }

    /* combine feature */
    res = as608_combine_feature(&gs_handle, gs_addr, status);
    if (res != 0)
    {
        return 1;
    }
    if (*status != AS608_STATUS_OK)
    {
        /* run the callback */
        if (callback != NULL)
        {
            /* output */
            callback(-1, "error.\n");
        }

        return 1;
    }

    /* get valid template number */
    res = as608_get_valid_template_number(&gs_handle, gs_addr, page_number, status);
    if (res != 0)
    {
        return 1;
    }
    if (*status != AS608_STATUS_OK)
    {
        /* run the callback */
        if (callback != NULL)
        {
            /* output */
            callback(-1, "error.\n");
        }

        return 1;
    }

    /* store feature */
    res = as608_store_feature(&gs_handle, gs_addr, AS608_BUFFER_NUMBER_2, *page_number, status);
    if (res != 0)
    {
        return 1;
    }
    if (*status != AS608_STATUS_OK)
    {
        /* run the callback */
        if (callback != NULL)
        {
            /* output */
            callback(-1, "error.\n");
        }

        return 1;
    }

    return 0;
}

/**
 * @brief      advance example verify
 * @param[out] *found_page points to a found page buffer
 * @param[out] *score points to a score buffer
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 verify failed
 * @note       none
 */
uint8_t as608_advance_verify(uint16_t *found_page, uint16_t *score, as608_status_t *status)
{
    uint8_t res;

    /* get image */
    res = as608_get_image(&gs_handle, gs_addr, status);
    if (res != 0)
    {
        return 1;
    }
    if (*status != AS608_STATUS_OK)
    {
        return 1;
    }

    /* generate feature */
    res = as608_generate_feature(&gs_handle, gs_addr, AS608_ADVANCE_DEFAULT_FEATURE, status);
    if (res != 0)
    {
        return 1;
    }
    if (*status != AS608_STATUS_OK)
    {
        return 1;
    }

    /* search feature */
    res = as608_search_feature(&gs_handle, gs_addr,
                               AS608_ADVANCE_DEFAULT_FEATURE,
                               0, 300, found_page,
                               score, status);
    if (res != 0)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief      advance example high speed verify
 * @param[out] *found_page points to a found page buffer
 * @param[out] *score points to a score buffer
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 high speed verify failed
 * @note       none
 */
uint8_t as608_advance_high_speed_verify(uint16_t *found_page, uint16_t *score, as608_status_t *status)
{
    uint8_t res;

    /* get image */
    res = as608_get_image(&gs_handle, gs_addr, status);
    if (res != 0)
    {
        return 1;
    }
    if (*status != AS608_STATUS_OK)
    {
        return 1;
    }

    /* generate feature */
    res = as608_generate_feature(&gs_handle, gs_addr, AS608_ADVANCE_DEFAULT_FEATURE, status);
    if (res != 0)
    {
        return 1;
    }
    if (*status != AS608_STATUS_OK)
    {
        return 1;
    }

    /* high speed search feature */
    res = as608_high_speed_search(&gs_handle, gs_addr,
                                  AS608_ADVANCE_DEFAULT_FEATURE,
                                  0, 300, found_page,
                                  score, status);
    if (res != 0)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief     advance example print status
 * @param[in] status is the print status
 * @return    status code
 *             - 0 success
 *             - 1 print failed
 * @note       none
 */
uint8_t as608_advance_print_status(as608_status_t status)
{
    uint8_t res;

    /* print status */
    res = as608_print_status(&gs_handle, status);
    if (res != 0)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief      advance example delete fingerprint
 * @param[in]  page_number is the page number
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 delete fingerprint failed
 * @note       none
 */
uint8_t as608_advance_delete_fingerprint(uint16_t page_number, as608_status_t *status)
{
    uint8_t res;

    /* delete feature */
    res = as608_delete_feature(&gs_handle, gs_addr, page_number, 1, status);
    if (res != 0)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief      advance example empty fingerprint
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 empty fingerprint failed
 * @note       none
 */
uint8_t as608_advance_empty_fingerprint(as608_status_t *status)
{
    uint8_t res;

    /* empty all feature */
    res = as608_empty_all_feature(&gs_handle, gs_addr, status);
    if (res != 0)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief      advance example write notepad
 * @param[in]  page_number is the page number
 * @param[in]  *data points to an input buffer
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 write notepad failed
 * @note       none
 */
uint8_t as608_advance_write_notepad(uint8_t page_number, uint8_t data[32], as608_status_t *status)
{
    uint8_t res;

    /* write notepad */
    res = as608_write_notepad(&gs_handle, gs_addr, page_number, data, status);
    if (res != 0)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief      advance example read notepad
 * @param[in]  page_number is the page number
 * @param[out] *data points to an output buffer
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 read notepad failed
 * @note       none
 */
uint8_t as608_advance_read_notepad(uint8_t page_number, uint8_t data[32], as608_status_t *status)
{
    uint8_t res;

    /* read notepad */
    res = as608_read_notepad(&gs_handle, gs_addr, page_number, data, status);
    if (res != 0)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief      advance example get random
 * @param[out] *randn points to a random buffer
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 random failed
 * @note       none
 */
uint8_t as608_advance_random(uint32_t *randn, as608_status_t *status)
{
    uint8_t res;

    /* get random */
    res = as608_get_random(&gs_handle, gs_addr, randn, status);
    if (res != 0)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief         advance example get flash information
 * @param[out]    *output_buffer points to an output buffer
 * @param[in,out] *output_len points to an output length buffer
 * @param[out]    *status points to a status buffer
 * @return        status code
 *                - 0 success
 *                - 1 get flash information failed
 * @note          none
 */
uint8_t as608_advance_flash_information(uint8_t *output_buffer, uint16_t *output_len, as608_status_t *status)
{
    uint8_t res;

    /* get flash information */
    res = as608_get_flash_information(&gs_handle, gs_addr, output_buffer, output_len, status);
    if (res != 0)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief      advance example get params
 * @param[out] *params points to a param structure
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get params failed
 * @note       none
 */
uint8_t as608_advance_params(as608_params_t *params, as608_status_t *status)
{
    uint8_t res;

    /* get params */
    res = as608_get_params(&gs_handle, gs_addr, params, status);
    if (res != 0)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief      advance example enroll
 * @param[out] *page_number points to a page number buffer
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 enroll failed
 * @note       none
 */
uint8_t as608_advance_enroll(uint16_t *page_number, as608_status_t *status)
{
    uint8_t res;

    /* enroll */
    res = as608_enroll(&gs_handle, gs_addr, page_number, status);
    if (res != 0)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief      advance example identify
 * @param[out] *page_number points to a page number buffer
 * @param[out] *score points to a score buffer
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 identify failed
 * @note       none
 */
uint8_t as608_advance_identify(uint16_t *page_number, uint16_t *score, as608_status_t *status)
{
    uint8_t res;

    /* identify */
    res = as608_identify(&gs_handle, gs_addr, page_number, score, status);
    if (res != 0)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief         advance example upload flash feature
 * @param[in]     page_number is the page number
 * @param[out]    *output_buffer points to an output buffer
 * @param[in,out] *output_len points to an output length buffer
 * @param[out]    *status points to a status buffer
 * @return        status code
 *                - 0 success
 *                - 1 upload flash feature failed
 * @note          none
 */
uint8_t as608_advance_upload_flash_feature(uint16_t page_number, uint8_t *output_buffer,
                                           uint16_t *output_len, as608_status_t *status)
{
    uint8_t res;

    /* load feature */
    res = as608_load_feature(&gs_handle, gs_addr, AS608_ADVANCE_DEFAULT_FEATURE,
                             page_number, status);
    if (res != 0)
    {
        return 1;
    }
    if (*status != AS608_STATUS_OK)
    {
        return 1;
    }

    /* upload feature */
    res = as608_upload_feature(&gs_handle, gs_addr, AS608_ADVANCE_DEFAULT_FEATURE,
                               output_buffer, output_len, status);
    if (res != 0)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief         advance example upload image feature
 * @param[out]    *output_buffer points to an output buffer
 * @param[in,out] *output_len points to an output length buffer
 * @param[out]    *status points to a status buffer
 * @return        status code
 *                - 0 success
 *                - 1 upload image feature failed
 * @note          none
 */
uint8_t as608_advance_upload_image_feature(uint8_t *output_buffer, uint16_t *output_len, as608_status_t *status)
{
    uint8_t res;

    /* get image */
    res = as608_get_image(&gs_handle, gs_addr, status);
    if (res != 0)
    {
        return 1;
    }
    if (*status != AS608_STATUS_OK)
    {
        return 1;
    }

    /* generate feature */
    res = as608_generate_feature(&gs_handle, gs_addr, AS608_ADVANCE_DEFAULT_FEATURE, status);
    if (res != 0)
    {
        return 1;
    }
    if (*status != AS608_STATUS_OK)
    {
        return 1;
    }

    /* upload feature */
    res = as608_upload_feature(&gs_handle, gs_addr, AS608_ADVANCE_DEFAULT_FEATURE,
                               output_buffer, output_len, status);
    if (res != 0)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief      advance example download flash feature
 * @param[in]  page_number is the page number
 * @param[in]  *input_buffer points to an input buffer
 * @param[in]  input_len is the input length
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 download flash feature failed
 * @note       none
 */
uint8_t as608_advance_download_flash_feature(uint16_t page_number, uint8_t *input_buffer,
                                             uint16_t input_len, as608_status_t *status)
{
    uint8_t res;

    /* download feature */
    res = as608_download_feature(&gs_handle, gs_addr, AS608_ADVANCE_DEFAULT_FEATURE,
                                 input_buffer, input_len, status);
    if (res != 0)
    {
        return 1;
    }

    /* store feature */
    res = as608_store_feature(&gs_handle, gs_addr, AS608_ADVANCE_DEFAULT_FEATURE, page_number, status);
    if (res != 0)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief         advance example upload image
 * @param[out]    *output_buffer points to an output buffer
 * @param[in,out] *output_len points to an output length buffer
 * @param[out]    *status points to a status buffer
 * @return        status code
 *                - 0 success
 *                - 1 upload image failed
 * @note          none
 */
uint8_t as608_advance_upload_image(uint8_t *output_buffer, uint16_t *output_len, as608_status_t *status)
{
    uint8_t res;

    /* get image */
    res = as608_get_image(&gs_handle, gs_addr, status);
    if (res != 0)
    {
        return 1;
    }
    if (*status != AS608_STATUS_OK)
    {
        return 1;
    }

    /* upload image */
    res = as608_upload_image(&gs_handle, gs_addr, output_buffer, output_len, status);
    if (res != 0)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief      advance example download image
 * @param[in]  page_number is the page number
 * @param[in]  *input_buffer points to an input buffer
 * @param[in]  input_len is the input length
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 download image failed
 * @note       none
 */
uint8_t as608_advance_download_image(uint16_t page_number, uint8_t *input_buffer, uint16_t input_len, as608_status_t *status)
{
    uint8_t res;

    /* download image */
    res = as608_download_image(&gs_handle, gs_addr, input_buffer, input_len, status);
    if (res != 0)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief  advance example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t as608_advance_deinit(void)
{
    /* deinit as608 */
    if (as608_deinit(&gs_handle) != 0)
    {
        return 1;
    }

    return 0;
}
