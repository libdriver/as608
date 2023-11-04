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
 * @file      driver_as608_interface_template.c
 * @brief     driver as608 interface template source file
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

#include "driver_as608_interface.h"

/**
 * @brief  interface uart init
 * @return status code
 *         - 0 success
 *         - 1 uart init failed
 * @note   none
 */
uint8_t as608_interface_uart_init(void)
{
    return 0;
}

/**
 * @brief  interface uart deinit
 * @return status code
 *         - 0 success
 *         - 1 uart deinit failed
 * @note   none
 */
uint8_t as608_interface_uart_deinit(void)
{
    return 0;
}

/**
 * @brief      interface uart read
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint16_t as608_interface_uart_read(uint8_t *buf, uint16_t len)
{
    return 0;
}

/**
 * @brief  interface uart flush
 * @return status code
 *         - 0 success
 *         - 1 uart flush failed
 * @note   none
 */
uint8_t as608_interface_uart_flush(void)
{
    return 0;
}

/**
 * @brief     interface uart write
 * @param[in] *buf points to a data buffer
 * @param[in] len is the length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t as608_interface_uart_write(uint8_t *buf, uint16_t len)
{
    return 0;
}

/**
 * @brief     interface delay ms
 * @param[in] ms
 * @note      none
 */
void as608_interface_delay_ms(uint32_t ms)
{

}

/**
 * @brief     interface print format data
 * @param[in] fmt is the format data
 * @note      none
 */
void as608_interface_debug_print(const char *const fmt, ...)
{
    
}
