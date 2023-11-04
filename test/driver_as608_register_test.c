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
 * @file      driver_as608_register_test.c
 * @brief     driver as608 register test source file
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

#include "driver_as608_register_test.h"
#include <stdlib.h>

static as608_handle_t gs_handle;            /**< as608 handle */
static uint8_t gs_output_buffer[512];       /**< test buffer */

/**
 * @brief     register test
 * @param[in] addr is the chip address
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t as608_register_test(uint32_t addr)
{
    uint8_t res;
    uint16_t i;
    uint16_t j;
    uint16_t output_len;
    uint32_t randn;
    uint8_t data[32];
    uint8_t data_check[32];
    as608_info_t info;
    as608_status_t status;
    as608_params_t param;
    as608_gpio_level_t output_level;
    
    /* link interface function */
    DRIVER_AS608_LINK_INIT(&gs_handle, as608_handle_t);
    DRIVER_AS608_LINK_UART_INIT(&gs_handle, as608_interface_uart_init);
    DRIVER_AS608_LINK_UART_DEINIT(&gs_handle, as608_interface_uart_deinit);
    DRIVER_AS608_LINK_UART_READ(&gs_handle, as608_interface_uart_read);
    DRIVER_AS608_LINK_UART_WRITE(&gs_handle, as608_interface_uart_write);
    DRIVER_AS608_LINK_UART_FLUSH(&gs_handle, as608_interface_uart_flush);
    DRIVER_AS608_LINK_DELAY_MS(&gs_handle, as608_interface_delay_ms);
    DRIVER_AS608_LINK_DEBUG_PRINT(&gs_handle, as608_interface_debug_print);
    
    /* get as608 information */
    res = as608_info(&info);
    if (res != 0)
    {
        as608_interface_debug_print("as608: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip information */
        as608_interface_debug_print("as608: chip is %s.\n", info.chip_name);
        as608_interface_debug_print("as608: manufacturer is %s.\n", info.manufacturer_name);
        as608_interface_debug_print("as608: interface is %s.\n", info.interface);
        as608_interface_debug_print("as608: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        as608_interface_debug_print("as608: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        as608_interface_debug_print("as608: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        as608_interface_debug_print("as608: max current is %0.2fmA.\n", info.max_current_ma);
        as608_interface_debug_print("as608: max temperature is %0.1fC.\n", info.temperature_max);
        as608_interface_debug_print("as608: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start register test */
    as608_interface_debug_print("as608: start register test.\n");
    
    /* as608 init */
    res = as608_init(&gs_handle, addr);
    if (res != 0)
    {
        as608_interface_debug_print("as608: init failed.\n");

        return 1;
    }
    
    /* as608_get_last_status test */
    as608_interface_debug_print("as608: as608_get_last_status test.\n");
    
    /* get last status */
    res = as608_get_last_status(&gs_handle, &status);
    if (res != 0)
    {
        as608_interface_debug_print("as608: get last status failed.\n");
        (void)as608_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    as608_interface_debug_print("as608: last status is 0x%02X.\n", status);
    
    /* as608_print_status test */
    as608_interface_debug_print("as608: as608_print_status test.\n");
    
    /* print status */
    res = as608_print_status(&gs_handle, status);
    if (res != 0)
    {
        as608_interface_debug_print("as608: print status failed.\n");
        (void)as608_deinit(&gs_handle);
        
        return 1;
    }
    
    /* as608_set_baud_rate test */
    as608_interface_debug_print("as608: as608_set_baud_rate test.\n");
    
    /* 57600bps */
    res = as608_set_baud_rate(&gs_handle, addr, 6, &status);
    if (res != 0)
    {
        as608_interface_debug_print("as608: set baud rate failed.\n");
        (void)as608_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    as608_interface_debug_print("as608: set baud rate %d.\n", 57600);
    
    /* as608_set_level test */
    as608_interface_debug_print("as608: as608_set_level test.\n");
    
    /* set level 3 */
    res = as608_set_level(&gs_handle, addr, AS608_LEVEL_3, &status);
    if (res != 0)
    {
        as608_interface_debug_print("as608: set level failed.\n");
        (void)as608_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    as608_interface_debug_print("as608: set level 3.\n");
    
    /* as608_set_packet_size test */
    as608_interface_debug_print("as608: as608_set_packet_size test.\n");
    
    /* 128bytes */
    res = as608_set_packet_size(&gs_handle, addr, AS608_PACKET_SIZE_128_BYTES, &status);
    if (res != 0)
    {
        as608_interface_debug_print("as608: set packet size failed.\n");
        (void)as608_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    as608_interface_debug_print("as608: set packet size 128bytes.\n");
    
    /* as608_get_params test */
    as608_interface_debug_print("as608: as608_get_params test.\n");
    
    /* get params */
    res = as608_get_params(&gs_handle, addr, &param, &status);
    if (res != 0)
    {
        as608_interface_debug_print("as608: get params failed.\n");
        (void)as608_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    as608_interface_debug_print("as608: status is 0x%02X.\n", param.status);
    as608_interface_debug_print("as608: sensor type is 0x%02X.\n", param.sensor_type);
    as608_interface_debug_print("as608: fingerprint size is %d.\n", param.fingerprint_size);
    as608_interface_debug_print("as608: level is 0x%02X.\n", param.level);
    as608_interface_debug_print("as608: address is 0x%04X.\n", param.address);
    as608_interface_debug_print("as608: packet size is 0x%02X.\n", param.packet_size);
    as608_interface_debug_print("as608: baud rate is %d.\n", param.n_9600 * 9600);
    
    /* as608_set_password test */
    as608_interface_debug_print("as608: as608_set_password test.\n");
    
    /* 0x00000000 */
    res = as608_set_password(&gs_handle, addr, 0x00000000, &status);
    if (res != 0)
    {
        as608_interface_debug_print("as608: set password failed.\n");
        (void)as608_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    as608_interface_debug_print("as608: set password 0x%08X.\n", 0x00000000);
    
    /* as608_verify_password test */
    as608_interface_debug_print("as608: as608_verify_password test.\n");
    
    /* verify password */
    res = as608_verify_password(&gs_handle, addr, 0x00000000, &status);
    if (res != 0)
    {
        as608_interface_debug_print("as608: verify password failed.\n");
        (void)as608_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    as608_interface_debug_print("as608: verify password %s.\n", status == AS608_STATUS_OK ? "ok" : "error");
    
    /* as608_get_random test */
    as608_interface_debug_print("as608: as608_get_random test.\n");
    
    /* get random */
    res = as608_get_random(&gs_handle, addr, &randn, &status);
    if (res != 0)
    {
        as608_interface_debug_print("as608: get random failed.\n");
        (void)as608_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    as608_interface_debug_print("as608: random is 0x%08X.\n", randn);
    
    /* as608_set_chip_address test */
    as608_interface_debug_print("as608: as608_set_chip_address test.\n");
    
    /* set chip address */
    res = as608_set_chip_address(&gs_handle, addr, 0xFFFFFFFFU, &status);
    if (res != 0)
    {
        as608_interface_debug_print("as608: set chip address failed.\n");
        (void)as608_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    as608_interface_debug_print("as608: set chip address 0x%08X.\n", 0xFFFFFFFFU);
    
    /* as608_get_flash_information test */
    as608_interface_debug_print("as608: as608_get_flash_information test.\n");
    
    /* get flash information */
    output_len = 512;
    res = as608_get_flash_information(&gs_handle, addr, gs_output_buffer, &output_len, &status);
    if (res != 0)
    {
        as608_interface_debug_print("as608: get flash information failed.\n");
        (void)as608_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    as608_interface_debug_print("as608: flash information is\n");
    for (i = 0; i < output_len; i++)
    {
        as608_interface_debug_print("0x%02X ", gs_output_buffer[i]);
    }
    as608_interface_debug_print("\n");
    
    /* as608_set_port test */
    as608_interface_debug_print("as608: as608_set_port test.\n");
    
    /* enable port */
    res = as608_set_port(&gs_handle, addr, AS608_BOOL_TRUE, &status);
    if (res != 0)
    {
        as608_interface_debug_print("as608: set port failed.\n");
        (void)as608_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    as608_interface_debug_print("as608: enable port.\n");
    
    /* as608_write_notepad/as608_read_notepad test */
    as608_interface_debug_print("as608: as608_write_notepad/as608_read_notepad test test.\n");
    
    for (i = 0; i < 16; i++)
    {
        /* random */
        as608_interface_debug_print("as608: random.\n");
        for (j = 0; j < 32; j++)
        {
            data[j] = rand() % 0xFF;
            as608_interface_debug_print("0x%02X ", data[j]);
        }
        as608_interface_debug_print("\n");
        
        /* write notepad */
        res = as608_write_notepad(&gs_handle, addr, (uint8_t)i, data, &status);
        if (res != 0)
        {
            as608_interface_debug_print("as608: write notepad failed.\n");
            (void)as608_deinit(&gs_handle);
            
            return 1;
        }
        
        /* read notepad */
        res = as608_read_notepad(&gs_handle, addr, (uint8_t)i, data_check, &status);
        if (res != 0)
        {
            as608_interface_debug_print("as608: read notepad failed.\n");
            (void)as608_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        as608_interface_debug_print("as608: check page number %d %s.\n", i, (memcmp(data, data_check, 32) == 0) ? "ok" : "error");
    }
    
    /* as608_set_gpio_level test */
    as608_interface_debug_print("as608: as608_set_gpio_level test.\n");
    
    /* set level low */
    res = as608_set_gpio_level(&gs_handle, addr, AS608_GPIO_NUMBER_0, AS608_GPIO_LEVEL_LOW, &output_level, &status);
    if (res != 0)
    {
        as608_interface_debug_print("as608: set gpio level failed.\n");
        (void)as608_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    as608_interface_debug_print("as608: set gpio%d level %s.\n", 0, (output_level == AS608_GPIO_LEVEL_LOW) ? "low" : "high");
    
    /* set level high */
    res = as608_set_gpio_level(&gs_handle, addr, AS608_GPIO_NUMBER_0, AS608_GPIO_LEVEL_HIGH, &output_level, &status);
    if (res != 0)
    {
        as608_interface_debug_print("as608: set gpio level failed.\n");
        (void)as608_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    as608_interface_debug_print("as608: set gpio%d level %s.\n", 0, (output_level == AS608_GPIO_LEVEL_LOW) ? "low" : "high");
    
    /* set level low */
    res = as608_set_gpio_level(&gs_handle, addr, AS608_GPIO_NUMBER_1, AS608_GPIO_LEVEL_LOW, &output_level, &status);
    if (res != 0)
    {
        as608_interface_debug_print("as608: set gpio level failed.\n");
        (void)as608_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    as608_interface_debug_print("as608: set gpio%d level %s.\n", 1, (output_level == AS608_GPIO_LEVEL_LOW) ? "low" : "high");
    
    /* set level high */
    res = as608_set_gpio_level(&gs_handle, addr, AS608_GPIO_NUMBER_1, AS608_GPIO_LEVEL_HIGH, &output_level, &status);
    if (res != 0)
    {
        as608_interface_debug_print("as608: set gpio level failed.\n");
        (void)as608_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    as608_interface_debug_print("as608: set gpio%d level %s.\n", 1, (output_level == AS608_GPIO_LEVEL_LOW) ? "low" : "high");
    
    /* finish register test */
    as608_interface_debug_print("as608: finish register test.\n");
    (void)as608_deinit(&gs_handle);

    return 0;
}
