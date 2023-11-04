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
 * @file      driver_as608.c
 * @brief     driver as608 source file
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

#include "driver_as608.h"
#include <math.h>

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "Synochip AS608"        /**< chip name */
#define MANUFACTURER_NAME         "Synochip"              /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        3.0f                    /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        3.6f                    /**< chip max supply voltage */
#define MAX_CURRENT               60.0f                   /**< chip max current */
#define TEMPERATURE_MIN           -40.0f                  /**< chip min operating temperature */
#define TEMPERATURE_MAX           85.0f                   /**< chip max operating temperature */
#define DRIVER_VERSION            1000                    /**< driver version */

/**
 * @brief chip command definition
 */
#define AS608_COMMAND_GET_IMAGE             0x01        /**< get image command */
#define AS608_COMMAND_GEN_CHAR              0x02        /**< generate char command */
#define AS608_COMMAND_MATCH                 0x03        /**< match command */
#define AS608_COMMAND_SEARCH                0x04        /**< search command */
#define AS608_COMMAND_REG_MODEL             0x05        /**< reg model command */
#define AS608_COMMAND_STORE_CHAR            0x06        /**< store char command */
#define AS608_COMMAND_LOAD_CHAR             0x07        /**< load char command */
#define AS608_COMMAND_UP_CHAR               0x08        /**< up char command */
#define AS608_COMMAND_DOWN_CHAR             0x09        /**< down char command */
#define AS608_COMMAND_UP_IMAGE              0x0A        /**< up image command */
#define AS608_COMMAND_DOWN_IMAGE            0x0B        /**< down image command */
#define AS608_COMMAND_DELETE_CHAR           0x0C        /**< delete char command */
#define AS608_COMMAND_EMPTY                 0x0D        /**< empty command */
#define AS608_COMMAND_WRITE_REG             0x0E        /**< write reg command */
#define AS608_COMMAND_READ_SYS_PARA         0x0F        /**< read sys para command */
#define AS608_COMMAND_ENROLL                0x10        /**< enroll command */
#define AS608_COMMAND_IDENTIFY              0x11        /**< identify command */
#define AS608_COMMAND_SET_PWD               0x12        /**< set password command */
#define AS608_COMMAND_VFY_PWD               0x13        /**< verify password command */
#define AS608_COMMAND_GET_RANDOM_CODE       0x14        /**< get random code command */
#define AS608_COMMAND_SET_CHIP_ADDR         0x15        /**< set chip addr command */
#define AS608_COMMAND_READ_INFO_PAGE        0x16        /**< read info page command */
#define AS608_COMMAND_PORT_CONTROL          0x17        /**< port control command */
#define AS608_COMMAND_WRITE_NOTEPAD         0x18        /**< write notepad command */
#define AS608_COMMAND_READ_NOTEPAD          0x19        /**< read notepad command */
#define AS608_COMMAND_BURN_CODE             0x1A        /**< burn code command */
#define AS608_COMMAND_HIGH_SPEED_SEARCH     0x1B        /**< high speed search command */
#define AS608_COMMAND_GEN_BIN_IMAGE         0x1C        /**< generate bin image command */
#define AS608_COMMAND_VALID_TEMPLATE_NUM    0x1D        /**< valid template num command */
#define AS608_COMMAND_USER_GPIO             0x1E        /**< user gpio command */
#define AS608_COMMAND_READ_INDEX_TABLE      0x1F        /**< read index table command */

/**
 * @brief chip type definition
 */
#define AS608_TYPE_COMMAND         0x01        /**< command type */
#define AS608_TYPE_DATA            0x02        /**< data type */
#define AS608_TYPE_RESPONSE        0x07        /**< response type */
#define AS608_TYPE_END             0x08        /**< end type */

/**
 * @brief     uart write data
 * @param[in] *handle points to an as608 handle structure
 * @param[in] addr is the set address
 * @param[in] type is the set type
 * @param[in] *buf points to an input buffer
 * @param[in] len is the buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 len is over
 * @note      none
 */
static uint8_t a_as608_uart_write(as608_handle_t *handle, uint32_t addr, uint8_t type, 
                                  uint8_t *buf, uint16_t len)
{
    uint16_t i;
    uint16_t sum;
    uint16_t l;
    
    if ((len + 11) > 384)                                   /* check length */
    {
        return 2;                                           /* return error */
    }
    handle->buf[0] = 0xEF;                                  /* header 0 */
    handle->buf[1] = 0x01;                                  /* header 1 */
    handle->buf[2] = (addr >> 24) & 0xFF;                   /* set addr 3 */
    handle->buf[3] = (addr >> 16) & 0xFF;                   /* set addr 2 */
    handle->buf[4] = (addr >> 8) & 0xFF;                    /* set addr 1 */
    handle->buf[5] = (addr >> 0) & 0xFF;                    /* set addr 0 */
    handle->buf[6] = type;                                  /* set type */
    l = len + 2;                                            /* add check crc */
    handle->buf[7] = (l >> 8) & 0xFF;                       /* set l msb */
    handle->buf[8] = (l >> 0) & 0xFF;                       /* set l lsb */
    memcpy(handle->buf + 9, buf, len);                      /* copy data */
    l = len + 9;                                            /* add 9 */
    sum = 0;                                                /* init 0 */
    for(i = 6; i < l; i++)                                  /* loop */
    {
        sum += handle->buf[i];                              /* sum all */
    }
    handle->buf[l] = (sum >> 8) & 0xFF;                     /* set sum msb */
    handle->buf[l + 1] = (sum >> 0) & 0xFF;                 /* set sum lsb */
    
    if (handle->uart_flush() != 0)                          /* uart flush */
    {
        return 1;                                           /* return error */
    }
    if (handle->uart_write(handle->buf, l + 2) != 0)        /* write data */
    {
        return 1;                                           /* return error */
    }
    
    return 0;                                               /* success return 0 */
}

/**
 * @brief         uart decode
 * @param[in]     *handle points to an as608 handle structure
 * @param[in]     ms is the delay in ms
 * @param[out]    *addr points to an address buffer
 * @param[out]    *buf points to an output buffer
 * @param[in,out] *len points to a buffer length buffer
 * @return        status code
 *                - 0 success
 *                - 1 decode failed
 *                - 2 no response
 *                - 3 sum check error
 *                - 4 len is invalid
 *                - 5 header is invalid
 *                - 6 type is invalid
 * @note          none
 */
static uint8_t a_as608_uart_decode(as608_handle_t *handle, uint32_t ms, uint32_t *addr,
                                   uint8_t *buf, uint16_t *len)
{
    uint16_t i;
    uint16_t l;
    uint16_t ll;
    uint16_t sum;
    uint16_t sum_check;
    uint16_t read_len;
    
    handle->delay_ms(ms);                                                                      /* delay */
    l = handle->uart_read(handle->buf, 384);                                                   /* read data */
    if (l != 0)                                                                                /* check length */
    {
        if (l < 12)                                                                            /* check min length */
        {
            handle->debug_print("as608: decode failed.\n");                                    /* decode failed */
            
            return 2;                                                                          /* return error */
        }
        ll = l - 2;                                                                            /* no check sum */
        sum = 0;                                                                               /* init 0 */
        for(i = 6; i < ll; i++)                                                                /* loop */
        {
            sum += handle->buf[i];                                                             /* sum all */
        }
        sum_check = (uint16_t)((uint16_t)handle->buf[l - 2] << 8) | handle->buf[l - 1];        /* sum check */
        if (sum != sum_check)                                                                  /* check sum */
        {
            handle->debug_print("as608: sum check error.\n");                                  /* sum check error */
            
            return 3;                                                                          /* return error */
        }
        ll = (uint16_t)((uint16_t)handle->buf[7] << 8) | handle->buf[8];                       /* get length */
        if (ll + 9 != l)                                                                       /* check length */
        {
            handle->debug_print("as608: len is invalid.\n");                                   /* len is invalid */
            
            return 4;                                                                          /* return error */
        }
        if ((handle->buf[0] != 0xEF) && (handle->buf[1] != 0x01))                              /* check header */
        {
            handle->debug_print("as608: header is invalid.\n");                                /* header is invalid */
            
            return 5;                                                                          /* return error */
        }
        *addr = (uint32_t)((uint32_t)handle->buf[2] << 24) | 
                (uint32_t)((uint32_t)handle->buf[3] << 16) |
                (uint32_t)((uint32_t)handle->buf[4] << 8)  |
                (uint32_t)((uint32_t)handle->buf[5] << 0);                                     /* set address */
        if (handle->buf[6] != AS608_TYPE_RESPONSE)                                             /* check type */
        {
            handle->debug_print("as608: type is invalid.\n");                                  /* type is invalid */
            
            return 6;                                                                          /* return error */
        }
        read_len = ((*len) < (ll - 2)) ? (*len) : (ll - 2);                                    /* set length */
        memcpy(buf, &handle->buf[9], read_len);                                                /* copy data */
        *len = read_len;                                                                       /* set addr */
        
        return 0;                                                                              /* success return 0 */
    }
    else
    {
        handle->debug_print("as608: no response.\n");                                          /* no response */
        
        return 2;                                                                              /* return error */
    }
}

/**
 * @brief         uart decode with length
 * @param[in]     *handle points to an as608 handle structure
 * @param[in]     input_len is the input length
 * @param[in]     ms is the delay in ms
 * @param[out]    *addr points to an address buffer
 * @param[out]    *buf points to an output buffer
 * @param[in,out] *len points to a buffer length buffer
 * @return        status code
 *                - 0 success
 *                - 1 decode failed
 *                - 2 no response
 *                - 3 sum check error
 *                - 4 len is invalid
 *                - 5 header is invalid
 *                - 6 type is invalid
 * @note          none
 */
static uint8_t a_as608_uart_decode_with_length(as608_handle_t *handle, uint16_t input_len,
                                               uint32_t ms, uint32_t *addr,
                                               uint8_t *buf, uint16_t *len)
{
    uint16_t i;
    uint16_t l;
    uint16_t ll;
    uint16_t sum;
    uint16_t sum_check;
    uint16_t read_len;
    
    handle->delay_ms(ms);                                                                      /* delay */
    l = handle->uart_read(handle->buf, input_len);                                             /* read data */
    if (l != 0)                                                                                /* check length */
    {
        if (l < 12)                                                                            /* check min length */
        {
            handle->debug_print("as608: decode failed.\n");                                    /* decode failed */
            
            return 2;                                                                          /* return error */
        }
        if (l != input_len)                                                                    /* check min length */
        {
            handle->debug_print("as608: decode failed.\n");                                    /* decode failed */
            
            return 2;                                                                          /* return error */
        }
        ll = l - 2;                                                                            /* no check sum */
        sum = 0;                                                                               /* init 0 */
        for(i = 6; i < ll; i++)                                                                /* loop */
        {
            sum += handle->buf[i];                                                             /* sum all */
        }
        sum_check = (uint16_t)((uint16_t)handle->buf[l - 2] << 8) | handle->buf[l - 1];        /* sum check */
        if (sum != sum_check)                                                                  /* check sum */
        {
            handle->debug_print("as608: sum check error.\n");                                  /* sum check error */
            
            return 3;                                                                          /* return error */
        }
        ll = (uint16_t)((uint16_t)handle->buf[7] << 8) | handle->buf[8];                       /* get length */
        if (ll + 9 != l)                                                                       /* check length */
        {
            handle->debug_print("as608: len is invalid.\n");                                   /* len is invalid */
            
            return 4;                                                                          /* return error */
        }
        if ((handle->buf[0] != 0xEF) && (handle->buf[1] != 0x01))                              /* check header */
        {
            handle->debug_print("as608: header is invalid.\n");                                /* header is invalid */
            
            return 5;                                                                          /* return error */
        }
        *addr = (uint32_t)((uint32_t)handle->buf[2] << 24) | 
                (uint32_t)((uint32_t)handle->buf[3] << 16) |
                (uint32_t)((uint32_t)handle->buf[4] << 8)  |
                (uint32_t)((uint32_t)handle->buf[5] << 0);                                     /* set address */
        if (handle->buf[6] != AS608_TYPE_RESPONSE)                                             /* check type */
        {
            handle->debug_print("as608: type is invalid.\n");                                  /* type is invalid */
            
            return 6;                                                                          /* return error */
        }
        read_len = ((*len) < (ll - 2)) ? (*len) : (ll - 2);                                    /* set length */
        memcpy(buf, &handle->buf[9], read_len);                                                /* copy data */
        *len = read_len;                                                                       /* set addr */
        
        return 0;                                                                              /* success return 0 */
    }
    else
    {
        handle->debug_print("as608: no response.\n");                                          /* no response */
        
        return 2;                                                                              /* return error */
    }
}

/**
 * @brief         uart parse data
 * @param[in]     *handle points to an as608 handle structure
 * @param[in]     ms is the delay in ms
 * @param[out]    *addr points to an address buffer
 * @param[out]    *buf points to an output buffer
 * @param[in,out] *len points to a buffer length buffer
 * @param[out]    *end_enable points to an end bool buffer
 * @param[out]    *full_enable points to a full bool buffer
 * @return        status code
 *                - 0 success
 *                - 1 parse data failed
 *                - 2 no response
 *                - 3 sum check error
 *                - 4 len is invalid
 *                - 5 header is invalid
 *                - 6 type is invalid
 * @note          none
 */
static uint8_t a_as608_uart_parse_data(as608_handle_t *handle, uint32_t ms,uint32_t *addr, uint8_t *buf, 
                                       uint16_t *len, as608_bool_t *end_enable, as608_bool_t *full_enable)
{
    uint16_t i;
    uint16_t l;
    uint16_t ll;
    uint16_t sum;
    uint16_t sum_check;
    uint16_t read_len;
    
    handle->delay_ms(ms);                                                                      /* delay */
    l = handle->uart_read(handle->buf, 9);                                                     /* read data */
    if (l != 0)                                                                                /* check length */
    {
        if (l != 9)                                                                            /* check min length */
        {
            handle->debug_print("as608: parse failed.\n");                                     /* parse failed */
            
            return 2;                                                                          /* return error */
        }
        l = (uint16_t)(((uint16_t)handle->buf[7]) << 8) | handle->buf[8];                      /* get length */
        if (l > 256)
        {
            handle->debug_print("as608: len is invalid.\n");                                   /* len is invalid */
            
            return 4;                                                                          /* return error */
        }
        ll = handle->uart_read(&handle->buf[9], l);                                            /* read data */
        if (ll != l)                                                                           /* check the read length */
        {
            handle->debug_print("as608: parse failed.\n");                                     /* parse failed */
            
            return 2;                                                                          /* return error */
        }
        l += 9;                                                                                /* add header length */
        ll = l - 2;                                                                            /* no check sum */
        sum = 0;                                                                               /* init 0 */
        for(i = 6; i < ll; i++)                                                                /* loop */
        {
            sum += handle->buf[i];                                                             /* sum all */
        }
        sum_check = (uint16_t)((uint16_t)handle->buf[l - 2] << 8) | handle->buf[l - 1];        /* sum check */
        if (sum != sum_check)                                                                  /* check sum */
        {
            handle->debug_print("as608: sum check error.\n");                                  /* sum check error */
            
            return 3;                                                                          /* return error */
        }
        ll = (uint16_t)((uint16_t)handle->buf[7] << 8) | handle->buf[8];                       /* get length */
        if (ll + 9 != l)                                                                       /* check length */
        {
            handle->debug_print("as608: len is invalid.\n");                                   /* len is invalid */
            
            return 4;                                                                          /* return error */
        }
        if ((handle->buf[0] != 0xEF) && (handle->buf[1] != 0x01))                              /* check header */
        {
            handle->debug_print("as608: header is invalid.\n");                                /* header is invalid */
            
            return 5;                                                                          /* return error */
        }
        *addr = (uint32_t)((uint32_t)handle->buf[2] << 24) | 
                (uint32_t)((uint32_t)handle->buf[3] << 16) |
                (uint32_t)((uint32_t)handle->buf[4] << 8)  |
                (uint32_t)((uint32_t)handle->buf[5] << 0);                                     /* set address */
        if (handle->buf[6] == AS608_TYPE_DATA)                                                 /* if data type */
        {
            *end_enable = AS608_BOOL_FALSE;                                                    /* disable end */
        }
        else if (handle->buf[6] == AS608_TYPE_END)                                             /* if end */
        {
            *end_enable = AS608_BOOL_TRUE;                                                     /* enable end */
        }
        else
        {
            handle->debug_print("as608: type is invalid.\n");                                  /* type is invalid */
            
            return 6;                                                                          /* return error */
        }
        read_len = ((*len) < (ll - 2)) ? (*len) : (ll - 2);                                    /* set length */
        memcpy(buf, &handle->buf[9], read_len);                                                /* copy data */
        *len = read_len;                                                                       /* set addr */
        if ((ll - 2) > (*len))                                                                 /* check buffer full */
        {
            *full_enable = AS608_BOOL_TRUE;                                                    /* full */
        }
        else
        {
            *full_enable = AS608_BOOL_FALSE;                                                   /* not full */
        }
        
        return 0;                                                                              /* success return 0 */
    }
    else
    {
        handle->debug_print("as608: no response.\n");                                          /* no response */
        
        return 2;                                                                              /* return error */
    }
}

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
uint8_t as608_init(as608_handle_t *handle, uint32_t addr)
{
    uint8_t res;
    uint8_t buf[17];
    uint16_t len;
    uint32_t addr_check;
    
    if (handle == NULL)                                              /* check handle */
    {
        return 2;                                                    /* return error */
    }
    if (handle->debug_print == NULL)                                 /* check debug_print */
    {
        return 3;                                                    /* return error */
    }
    if (handle->uart_init == NULL)                                   /* check uart_init */
    {
        handle->debug_print("as608: uart_init is null.\n");          /* uart_init is null */
        
        return 3;                                                    /* return error */
    }
    if (handle->uart_deinit == NULL)                                 /* check uart_deinit */
    {
        handle->debug_print("as608: uart_deinit is null.\n");        /* uart_deinit is null */
        
        return 3;                                                    /* return error */
    }
    if (handle->uart_read == NULL)                                   /* check uart_read */
    {
        handle->debug_print("as608: uart_read is null.\n");          /* uart_read is null */
        
        return 3;                                                    /* return error */
    }
    if (handle->uart_write == NULL)                                  /* check uart_write */
    {
        handle->debug_print("as608: uart_write is null.\n");         /* uart_write is null */
        
        return 3;                                                    /* return error */
    } 
    if (handle->uart_flush == NULL)                                  /* check uart_flush */
    {
        handle->debug_print("as608: uart_flush is null.\n");         /* uart_flush is null */
        
        return 3;                                                    /* return error */
    }
    if (handle->delay_ms == NULL)                                    /* check delay_ms */
    {
        handle->debug_print("as608: delay_ms is null.\n");           /* delay_ms is null */
        
        return 3;                                                    /* return error */
    }

    if (handle->uart_init() != 0)                                    /* uart init */
    {
        handle->debug_print("as608: uart init failed.\n");           /* uart init failed */
        
        return 1;                                                    /* return error */
    }
    handle->delay_ms(100);                                           /* delay 100ms */
    
    buf[0] = AS608_COMMAND_READ_SYS_PARA;                                      /* read sys para */
    res = a_as608_uart_write(handle, addr, AS608_TYPE_COMMAND, buf, 1);        /* write data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("as608: uart write failed.\n");                    /* uart write failed */
        (void)handle->uart_deinit();                                           /* uart deinit */
        
        return 4;                                                              /* return error */
    }
    len = 17;                                                                  /* len 17 */
    res = a_as608_uart_decode(handle, 300, &addr_check, buf, &len);            /* decode */
    if (res != 0)                                                              /* check result */
    {
        (void)handle->uart_deinit();                                           /* uart deinit */
        
        return 4;                                                              /* return error */
    }
    if (addr_check != addr)                                                    /* check addr */
    {
        handle->debug_print("as608: addr is invalid.\n");                      /* addr is invalid */
        (void)handle->uart_deinit();                                           /* uart deinit */
        
        return 4;                                                              /* return error */
    }
    if (len != 17)                                                             /* check length */
    {
        handle->debug_print("as608: len is invalid.\n");                       /* len is invalid */
        (void)handle->uart_deinit();                                           /* uart deinit */
        
        return 4;                                                              /* return error */
    }
    handle->status = buf[0];                                                   /* save status */
    handle->packet_size = (uint16_t)(32.0f * powf(2.0f, (float)((uint16_t)
                          ((uint16_t)buf[13] << 8) | buf[14])));               /* packet size */
    
    handle->inited = 1;                                                        /* flag finish initialization */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as608_deinit(as608_handle_t *handle)
{
    if (handle == NULL)                                             /* check handle */
    {
        return 2;                                                   /* return error */
    }
    if (handle->inited != 1)                                        /* check handle initialization */
    {
        return 3;                                                   /* return error */
    }
    
    if (handle->uart_deinit() != 0)                                 /* uart deinit */
    {
        handle->debug_print("as608: uart deinit failed.\n");        /* uart deinit failed */
        
        return 1;                                                   /* return error */
    }         
    handle->inited = 0;                                             /* flag close */
    
    return 0;                                                       /* success return 0 */ 
}

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
uint8_t as608_get_last_status(as608_handle_t *handle, as608_status_t *status)
{
    if (handle == NULL)                              /* check handle */
    {
        return 2;                                    /* return error */
    }
    if (handle->inited != 1)                         /* check handle initialization */
    {
        return 3;                                    /* return error */
    }
    
    *status = (as608_status_t)handle->status;        /* get status */
    
    return 0;                                        /* success return 0 */ 
}

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
uint8_t as608_get_image(as608_handle_t *handle, uint32_t addr, as608_status_t *status)
{
    uint8_t res;
    uint8_t buf[1];
    uint16_t len;
    uint32_t addr_check;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    buf[0] = AS608_COMMAND_GET_IMAGE;                                          /* get image */
    res = a_as608_uart_write(handle, addr, AS608_TYPE_COMMAND, buf, 1);        /* write data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("as608: uart write failed.\n");                    /* uart write failed */
        
        return 1;                                                              /* return error */
    }
    len = 1;                                                                   /* len 1 */
    res = a_as608_uart_decode(handle, 500, &addr_check, buf, &len);            /* decode */
    if (res != 0)                                                              /* check result */
    {
        return 4;                                                              /* return error */
    }
    if (addr_check != addr)                                                    /* check addr */
    {
        handle->debug_print("as608: addr is invalid.\n");                      /* addr is invalid */
        
        return 5;                                                              /* return error */
    }
    if (len != 1)                                                              /* check length */
    {
        handle->debug_print("as608: len is invalid.\n");                       /* len is invalid */
        
        return 6;                                                              /* return error */
    }
    handle->status = buf[0];                                                   /* save status */
    *status = (as608_status_t)handle->status;                                  /* set status */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as608_generate_feature(as608_handle_t *handle, uint32_t addr, as608_buffer_number_t num, as608_status_t *status)
{
    uint8_t res;
    uint8_t buf[2];
    uint16_t len;
    uint32_t addr_check;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    buf[0] = AS608_COMMAND_GEN_CHAR;                                           /* generate char */
    buf[1] = num;                                                              /* set buffer number */
    res = a_as608_uart_write(handle, addr, AS608_TYPE_COMMAND, buf, 2);        /* write data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("as608: uart write failed.\n");                    /* uart write failed */
        
        return 1;                                                              /* return error */
    }
    len = 1;                                                                   /* len 1 */
    res = a_as608_uart_decode(handle, 400, &addr_check, buf, &len);            /* decode */
    if (res != 0)                                                              /* check result */
    {
        return 4;                                                              /* return error */
    }
    if (addr_check != addr)                                                    /* check addr */
    {
        handle->debug_print("as608: addr is invalid.\n");                      /* addr is invalid */
        
        return 5;                                                              /* return error */
    }
    if (len != 1)                                                              /* check length */
    {
        handle->debug_print("as608: len is invalid.\n");                       /* len is invalid */
        
        return 6;                                                              /* return error */
    }
    handle->status = buf[0];                                                   /* save status */
    *status = (as608_status_t)handle->status;                                  /* set status */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as608_match_feature(as608_handle_t *handle, uint32_t addr, uint16_t *score, as608_status_t *status)
{
    uint8_t res;
    uint8_t buf[3];
    uint16_t len;
    uint32_t addr_check;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    buf[0] = AS608_COMMAND_MATCH;                                              /* match feature */
    res = a_as608_uart_write(handle, addr, AS608_TYPE_COMMAND, buf, 1);        /* write data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("as608: uart write failed.\n");                    /* uart write failed */
        
        return 1;                                                              /* return error */
    }
    len = 3;                                                                   /* len 3 */
    res = a_as608_uart_decode(handle, 300, &addr_check, buf, &len);            /* decode */
    if (res != 0)                                                              /* check result */
    {
        return 4;                                                              /* return error */
    }
    if (addr_check != addr)                                                    /* check addr */
    {
        handle->debug_print("as608: addr is invalid.\n");                      /* addr is invalid */
        
        return 5;                                                              /* return error */
    }
    if (len != 3)                                                              /* check length */
    {
        handle->debug_print("as608: len is invalid.\n");                       /* len is invalid */
        
        return 6;                                                              /* return error */
    }
    handle->status = buf[0];                                                   /* save status */
    *status = (as608_status_t)handle->status;                                  /* set status */
    *score = (uint16_t)((uint16_t)buf[1] << 8) | buf[2];                       /* set the score */
    
    return 0;                                                                  /* success return 0 */
}

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
                             uint16_t *score, as608_status_t *status)
{
    uint8_t res;
    uint8_t buf[6];
    uint16_t len;
    uint32_t addr_check;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    buf[0] = AS608_COMMAND_SEARCH;                                             /* search char */
    buf[1] = num;                                                              /* set buffer number */
    buf[2] = (start_page >> 8) & 0xFF;                                         /* start page msb */
    buf[3] = (start_page >> 0) & 0xFF;                                         /* start page lsb */
    buf[4] = (page_number >> 8) & 0xFF;                                        /* page number msb */
    buf[5] = (page_number >> 0) & 0xFF;                                        /* page number lsb */
    res = a_as608_uart_write(handle, addr, AS608_TYPE_COMMAND, buf, 6);        /* write data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("as608: uart write failed.\n");                    /* uart write failed */
        
        return 1;                                                              /* return error */
    }
    len = 5;                                                                   /* len 5 */
    res = a_as608_uart_decode(handle, 300, &addr_check, buf, &len);            /* decode */
    if (res != 0)                                                              /* check result */
    {
        return 4;                                                              /* return error */
    }
    if (addr_check != addr)                                                    /* check addr */
    {
        handle->debug_print("as608: addr is invalid.\n");                      /* addr is invalid */
        
        return 5;                                                              /* return error */
    }
    if (len != 5)                                                              /* check length */
    {
        handle->debug_print("as608: len is invalid.\n");                       /* len is invalid */
        
        return 6;                                                              /* return error */
    }
    handle->status = buf[0];                                                   /* save status */
    *status = (as608_status_t)handle->status;                                  /* set status */
    *found_page = (uint16_t)((uint16_t)buf[1] << 8) | buf[2];                  /* set the found page */
    *score = (uint16_t)((uint16_t)buf[3] << 8) | buf[4];                       /* set the score */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as608_combine_feature(as608_handle_t *handle, uint32_t addr, as608_status_t *status)
{
    uint8_t res;
    uint8_t buf[1];
    uint16_t len;
    uint32_t addr_check;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    buf[0] = AS608_COMMAND_REG_MODEL;                                          /* reg model */
    res = a_as608_uart_write(handle, addr, AS608_TYPE_COMMAND, buf, 1);        /* write data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("as608: uart write failed.\n");                    /* uart write failed */
        
        return 1;                                                              /* return error */
    }
    len = 1;                                                                   /* len 1 */
    res = a_as608_uart_decode(handle, 300, &addr_check, buf, &len);            /* decode */
    if (res != 0)                                                              /* check result */
    {
        return 4;                                                              /* return error */
    }
    if (addr_check != addr)                                                    /* check addr */
    {
        handle->debug_print("as608: addr is invalid.\n");                      /* addr is invalid */
        
        return 5;                                                              /* return error */
    }
    if (len != 1)                                                              /* check length */
    {
        handle->debug_print("as608: len is invalid.\n");                       /* len is invalid */
        
        return 6;                                                              /* return error */
    }
    handle->status = buf[0];                                                   /* save status */
    *status = (as608_status_t)handle->status;                                  /* set status */
    
    return 0;                                                                  /* success return 0 */
}

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
                            uint16_t page_number, as608_status_t *status)
{
    uint8_t res;
    uint8_t buf[4];
    uint16_t len;
    uint32_t addr_check;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    buf[0] = AS608_COMMAND_STORE_CHAR;                                         /* store char */
    buf[1] = num;                                                              /* set buffer number */
    buf[2] = (page_number >> 8) & 0xFF;                                        /* page number msb */
    buf[3] = (page_number >> 0) & 0xFF;                                        /* page number lsb */
    res = a_as608_uart_write(handle, addr, AS608_TYPE_COMMAND, buf, 4);        /* write data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("as608: uart write failed.\n");                    /* uart write failed */
        
        return 1;                                                              /* return error */
    }
    len = 1;                                                                   /* len 1 */
    res = a_as608_uart_decode(handle, 300, &addr_check, buf, &len);            /* decode */
    if (res != 0)                                                              /* check result */
    {
        return 4;                                                              /* return error */
    }
    if (addr_check != addr)                                                    /* check addr */
    {
        handle->debug_print("as608: addr is invalid.\n");                      /* addr is invalid */
        
        return 5;                                                              /* return error */
    }
    if (len != 1)                                                              /* check length */
    {
        handle->debug_print("as608: len is invalid.\n");                       /* len is invalid */
        
        return 6;                                                              /* return error */
    }
    handle->status = buf[0];                                                   /* save status */
    *status = (as608_status_t)handle->status;                                  /* set status */
    
    return 0;                                                                  /* success return 0 */
}

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
                           uint16_t page_number, as608_status_t *status)
{
    uint8_t res;
    uint8_t buf[4];
    uint16_t len;
    uint32_t addr_check;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    buf[0] = AS608_COMMAND_LOAD_CHAR;                                          /* load char */
    buf[1] = num;                                                              /* set buffer number */
    buf[2] = (page_number >> 8) & 0xFF;                                        /* page number msb */
    buf[3] = (page_number >> 0) & 0xFF;                                        /* page number lsb */
    res = a_as608_uart_write(handle, addr, AS608_TYPE_COMMAND, buf, 4);        /* write data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("as608: uart write failed.\n");                    /* uart write failed */
        
        return 1;                                                              /* return error */
    }
    len = 1;                                                                   /* len 1 */
    res = a_as608_uart_decode(handle, 300, &addr_check, buf, &len);            /* decode */
    if (res != 0)                                                              /* check result */
    {
        return 4;                                                              /* return error */
    }
    if (addr_check != addr)                                                    /* check addr */
    {
        handle->debug_print("as608: addr is invalid.\n");                      /* addr is invalid */
        
        return 5;                                                              /* return error */
    }
    if (len != 1)                                                              /* check length */
    {
        handle->debug_print("as608: len is invalid.\n");                       /* len is invalid */
        
        return 6;                                                              /* return error */
    }
    handle->status = buf[0];                                                   /* save status */
    *status = (as608_status_t)handle->status;                                  /* set status */
    
    return 0;                                                                  /* success return 0 */
}

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
                             uint8_t *output_buffer, uint16_t *output_len, as608_status_t *status)
{
    uint8_t res;
    uint8_t buf[2];
    uint16_t l;
    uint16_t point;
    uint16_t len;
    uint32_t addr_check;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    buf[0] = AS608_COMMAND_UP_CHAR;                                            /* upload char */
    buf[1] = num;                                                              /* set buffer number */
    res = a_as608_uart_write(handle, addr, AS608_TYPE_COMMAND, buf, 2);        /* write data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("as608: uart write failed.\n");                    /* uart write failed */
        
        return 1;                                                              /* return error */
    }
    len = 1;                                                                   /* len 1 */
    res = a_as608_uart_decode_with_length(handle, 12, 100, &addr_check, 
                                          buf, &len);                          /* decode */
    if (res != 0)                                                              /* check result */
    {
        return 4;                                                              /* return error */
    }
    if (addr_check != addr)                                                    /* check addr */
    {
        handle->debug_print("as608: addr is invalid.\n");                      /* addr is invalid */
        
        return 5;                                                              /* return error */
    }
    if (len != 1)                                                              /* check length */
    {
        handle->debug_print("as608: len is invalid.\n");                       /* len is invalid */
        
        return 6;                                                              /* return error */
    }
    handle->status = buf[0];                                                   /* save status */
    *status = (as608_status_t)handle->status;                                  /* set status */
    point = 0;                                                                 /* init 0 */
    len = *output_len;                                                         /* get the length */
    l = *output_len;                                                           /* get the length */
    while (1)                                                                  /* loop */
    {
        as608_bool_t end_enable;
        as608_bool_t full_enable;
        
        l = len;                                                               /* set the remain max buffer size */
        res = a_as608_uart_parse_data(handle, 100, &addr_check, 
                                      output_buffer + point, 
                                      &l, &end_enable, &full_enable);          /* parse the data */
        if (res != 0)
        {
            return 4;                                                          /* return error */
        }
        if (addr_check != addr)                                                /* check addr */
        {
            handle->debug_print("as608: addr is invalid.\n");                  /* addr is invalid */
            
            return 5;                                                          /* return error */
        }
        len -= l;                                                              /* remain length - read length */
        point += l;                                                            /* current point adds read length */
        if (full_enable == AS608_BOOL_TRUE)                                    /* if buffer is full */
        {
            handle->debug_print("as608: buffer is full.\n");                   /* buffer is full */
            
            return 7;                                                          /* return error */
        }
        if (end_enable == AS608_BOOL_TRUE)                                     /* if the last packet */
        {
            break;                                                             /* break */
        }
    }
    *output_len = point;                                                       /* save the length */
    
    return 0;                                                                  /* success return 0 */
}

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
                               uint8_t *input_buffer, uint16_t input_len, as608_status_t *status)
{
    uint8_t res;
    uint8_t buf[2];
    uint16_t i;
    uint16_t m;
    uint16_t n;
    uint16_t len;
    uint32_t addr_check;
    
    if (handle == NULL)                                                                        /* check handle */
    {
        return 2;                                                                              /* return error */
    }
    if (handle->inited != 1)                                                                   /* check handle initialization */
    {
        return 3;                                                                              /* return error */
    }
    
    buf[0] = AS608_COMMAND_DOWN_CHAR;                                                          /* download char */
    buf[1] = num;                                                                              /* set buffer number */
    res = a_as608_uart_write(handle, addr, AS608_TYPE_COMMAND, buf, 2);                        /* write data */
    if (res != 0)                                                                              /* check result */
    {
        handle->debug_print("as608: uart write failed.\n");                                    /* uart write failed */
        
        return 1;                                                                              /* return error */
    }
    len = 1;                                                                                   /* len 1 */
    res = a_as608_uart_decode_with_length(handle, 12, 100, &addr_check, buf, &len);            /* decode */
    if (res != 0)                                                                              /* check result */
    {
        return 4;                                                                              /* return error */
    }
    if (addr_check != addr)                                                                    /* check addr */
    {
        handle->debug_print("as608: addr is invalid.\n");                                      /* addr is invalid */
        
        return 5;                                                                              /* return error */
    }
    if (len != 1)                                                                              /* check length */
    {
        handle->debug_print("as608: len is invalid.\n");                                       /* len is invalid */
        
        return 6;                                                                              /* return error */
    }
    handle->status = buf[0];                                                                   /* save status */
    *status = (as608_status_t)handle->status;                                                  /* set status */
    
    m = input_len / handle->packet_size;                                                       /* set m */
    n = input_len % handle->packet_size;                                                       /* set n */
    if (n != 0)                                                                                /* check n */
    {
        for (i = 0; i < m; i++)                                                                /* m times */
        {
            res = a_as608_uart_write(handle, addr, AS608_TYPE_DATA, 
                                     input_buffer + i * handle->packet_size, 
                                     handle->packet_size);                                     /* write data */
            if (res != 0)                                                                      /* check result */
            {
                handle->debug_print("as608: uart write failed.\n");                            /* uart write failed */
                
                return 1;                                                                      /* return error */
            }
            handle->delay_ms(100);                                                             /* delay 100ms */
        }
        res = a_as608_uart_write(handle, addr, AS608_TYPE_END, 
                                 input_buffer + m * handle->packet_size, n);                   /* write data */
        if (res != 0)                                                                          /* check result */
        {
            handle->debug_print("as608: uart write failed.\n");                                /* uart write failed */
            
            return 1;                                                                          /* return error */
        }
        handle->delay_ms(100);                                                                 /* delay 100ms */
    }
    else
    {
        for (i = 0; i < (m - 1); i++)                                                          /* m - 1 times */
        {
            res = a_as608_uart_write(handle, addr, AS608_TYPE_DATA, 
                                     input_buffer + i * handle->packet_size, 
                                     handle->packet_size);                                     /* write data */
            if (res != 0)                                                                      /* check result */
            {
                handle->debug_print("as608: uart write failed.\n");                            /* uart write failed */
                
                return 1;                                                                      /* return error */
            }
            handle->delay_ms(100);                                                             /* delay 100ms */
        }
        res = a_as608_uart_write(handle, addr, AS608_TYPE_END, 
                                 input_buffer + (m - 1) * handle->packet_size, 
                                 handle->packet_size);                                         /* write data */
        if (res != 0)                                                                          /* check result */
        {
            handle->debug_print("as608: uart write failed.\n");                                /* uart write failed */
            
            return 1;                                                                          /* return error */
        }
        handle->delay_ms(100);                                                                 /* delay 100ms */
    }
    
    return 0;                                                                                  /* success return 0 */
}

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
                           uint16_t *output_len, as608_status_t *status)
{
    uint8_t res;
    uint8_t buf[1];
    uint16_t l;
    uint16_t point;
    uint16_t len;
    uint32_t addr_check;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    buf[0] = AS608_COMMAND_UP_IMAGE;                                           /* upload image */
    res = a_as608_uart_write(handle, addr, AS608_TYPE_COMMAND, buf, 1);        /* write data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("as608: uart write failed.\n");                    /* uart write failed */
        
        return 1;                                                              /* return error */
    }
    len = 1;                                                                   /* len 1 */
    res = a_as608_uart_decode_with_length(handle, 12, 100, 
                                          &addr_check, buf, &len);             /* decode */
    if (res != 0)                                                              /* check result */
    {
        return 4;                                                              /* return error */
    }
    if (addr_check != addr)                                                    /* check addr */
    {
        handle->debug_print("as608: addr is invalid.\n");                      /* addr is invalid */
        
        return 5;                                                              /* return error */
    }
    if (len != 1)                                                              /* check length */
    {
        handle->debug_print("as608: len is invalid.\n");                       /* len is invalid */
        
        return 6;                                                              /* return error */
    }
    handle->status = buf[0];                                                   /* save status */
    *status = (as608_status_t)handle->status;                                  /* set status */
    point = 0;                                                                 /* init 0 */
    len = *output_len;                                                         /* get the length */
    l = *output_len;                                                           /* get the length */
    while (1)                                                                  /* loop */
    {
        as608_bool_t end_enable;
        as608_bool_t full_enable;
        
        l = len;                                                               /* set the remain max buffer size */
        res = a_as608_uart_parse_data(handle, 100, &addr_check, 
                                      output_buffer + point, 
                                      &l, &end_enable, &full_enable);          /* parse the data */
        if (res != 0)
        {
            return 4;                                                          /* return error */
        }
        if (addr_check != addr)                                                /* check addr */
        {
            handle->debug_print("as608: addr is invalid.\n");                  /* addr is invalid */
            
            return 5;                                                          /* return error */
        }
        len -= l;                                                              /* remain length - read length */
        point += l;                                                            /* current point adds read length */
        if (full_enable == AS608_BOOL_TRUE)                                    /* if buffer is full */
        {
            handle->debug_print("as608: buffer is full.\n");                   /* buffer is full */
            
            return 7;                                                          /* return error */
        }
        if (end_enable == AS608_BOOL_TRUE)                                     /* if the last packet */
        {
            break;                                                             /* break */
        }
    }
    *output_len = point;                                                       /* save the length */
    
    return 0;                                                                  /* success return 0 */
}

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
                             uint16_t input_len, as608_status_t *status)
{
    uint8_t res;
    uint8_t buf[1];
    uint16_t i;
    uint16_t m;
    uint16_t n;
    uint16_t len;
    uint32_t addr_check;
    
    if (handle == NULL)                                                                        /* check handle */
    {
        return 2;                                                                              /* return error */
    }
    if (handle->inited != 1)                                                                   /* check handle initialization */
    {
        return 3;                                                                              /* return error */
    }
    
    buf[0] = AS608_COMMAND_DOWN_IMAGE;                                                         /* download char */
    res = a_as608_uart_write(handle, addr, AS608_TYPE_COMMAND, buf, 1);                        /* write data */
    if (res != 0)                                                                              /* check result */
    {
        handle->debug_print("as608: uart write failed.\n");                                    /* uart write failed */
        
        return 1;                                                                              /* return error */
    }
    len = 1;                                                                                   /* len 1 */
    res = a_as608_uart_decode_with_length(handle, 12, 100, &addr_check, buf, &len);            /* decode */
    if (res != 0)                                                                              /* check result */
    {
        return 4;                                                                              /* return error */
    }
    if (addr_check != addr)                                                                    /* check addr */
    {
        handle->debug_print("as608: addr is invalid.\n");                                      /* addr is invalid */
        
        return 5;                                                                              /* return error */
    }
    if (len != 1)                                                                              /* check length */
    {
        handle->debug_print("as608: len is invalid.\n");                                       /* len is invalid */
        
        return 6;                                                                              /* return error */
    }
    handle->status = buf[0];                                                                   /* save status */
    *status = (as608_status_t)handle->status;                                                  /* set status */
    
    m = input_len / handle->packet_size;                                                       /* set m */
    n = input_len % handle->packet_size;                                                       /* set n */
    if (n != 0)                                                                                /* check n */
    {
        for (i = 0; i < m; i++)                                                                /* m times */
        {
            res = a_as608_uart_write(handle, addr, AS608_TYPE_DATA, 
                                     input_buffer + i * handle->packet_size, 
                                     handle->packet_size);                                     /* write data */
            if (res != 0)                                                                      /* check result */
            {
                handle->debug_print("as608: uart write failed.\n");                            /* uart write failed */
                
                return 1;                                                                      /* return error */
            }
            handle->delay_ms(100);                                                             /* delay 100ms */
        }
        res = a_as608_uart_write(handle, addr, AS608_TYPE_END, 
                                 input_buffer + m * handle->packet_size, n);                   /* write data */
        if (res != 0)                                                                          /* check result */
        {
            handle->debug_print("as608: uart write failed.\n");                                /* uart write failed */
            
            return 1;                                                                          /* return error */
        }
        handle->delay_ms(100);                                                                 /* delay 100ms */
    }
    else
    {
        for (i = 0; i < (m - 1); i++)                                                          /* m - 1 times */
        {
            res = a_as608_uart_write(handle, addr, AS608_TYPE_DATA, 
                                     input_buffer + i * handle->packet_size, 
                                     handle->packet_size);                                     /* write data */
            if (res != 0)                                                                      /* check result */
            {
                handle->debug_print("as608: uart write failed.\n");                            /* uart write failed */
                
                return 1;                                                                      /* return error */
            }
            handle->delay_ms(100);                                                             /* delay 100ms */
        }
        res = a_as608_uart_write(handle, addr, AS608_TYPE_END, 
                                 input_buffer + (m - 1) * handle->packet_size, 
                                 handle->packet_size);                                         /* write data */
        if (res != 0)                                                                          /* check result */
        {
            handle->debug_print("as608: uart write failed.\n");                                /* uart write failed */
            
            return 1;                                                                          /* return error */
        }
        handle->delay_ms(100);                                                                 /* delay 100ms */
    }
    
    return 0;                                                                                  /* success return 0 */
}

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
uint8_t as608_delete_feature(as608_handle_t *handle, uint32_t addr, uint16_t page_number, uint16_t number, as608_status_t *status)
{
    uint8_t res;
    uint8_t buf[5];
    uint16_t len;
    uint32_t addr_check;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    buf[0] = AS608_COMMAND_DELETE_CHAR;                                        /* delete char feature */
    buf[1] = (page_number >> 8) & 0xFF;                                        /* page number msb */
    buf[2] = (page_number >> 0) & 0xFF;                                        /* page number lsb */
    buf[3] = (number >> 8) & 0xFF;                                             /* number msb */
    buf[4] = (number >> 0) & 0xFF;                                             /* number lsb */
    res = a_as608_uart_write(handle, addr, AS608_TYPE_COMMAND, buf, 5);        /* write data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("as608: uart write failed.\n");                    /* uart write failed */
        
        return 1;                                                              /* return error */
    }
    len = 1;                                                                   /* len 1 */
    res = a_as608_uart_decode(handle, 300, &addr_check, buf, &len);            /* decode */
    if (res != 0)                                                              /* check result */
    {
        return 4;                                                              /* return error */
    }
    if (addr_check != addr)                                                    /* check addr */
    {
        handle->debug_print("as608: addr is invalid.\n");                      /* addr is invalid */
        
        return 5;                                                              /* return error */
    }
    if (len != 1)                                                              /* check length */
    {
        handle->debug_print("as608: len is invalid.\n");                       /* len is invalid */
        
        return 6;                                                              /* return error */
    }
    handle->status = buf[0];                                                   /* save status */
    *status = (as608_status_t)handle->status;                                  /* set status */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as608_empty_all_feature(as608_handle_t *handle, uint32_t addr, as608_status_t *status)
{
    uint8_t res;
    uint8_t buf[1];
    uint16_t len;
    uint32_t addr_check;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    buf[0] = AS608_COMMAND_EMPTY;                                              /* empty all char feature */
    res = a_as608_uart_write(handle, addr, AS608_TYPE_COMMAND, buf, 1);        /* write data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("as608: uart write failed.\n");                    /* uart write failed */
        
        return 1;                                                              /* return error */
    }
    len = 1;                                                                   /* len 1 */
    res = a_as608_uart_decode(handle, 300, &addr_check, buf, &len);            /* decode */
    if (res != 0)                                                              /* check result */
    {
        return 4;                                                              /* return error */
    }
    if (addr_check != addr)                                                    /* check addr */
    {
        handle->debug_print("as608: addr is invalid.\n");                      /* addr is invalid */
        
        return 5;                                                              /* return error */
    }
    if (len != 1)                                                              /* check length */
    {
        handle->debug_print("as608: len is invalid.\n");                       /* len is invalid */
        
        return 6;                                                              /* return error */
    }
    handle->status = buf[0];                                                   /* save status */
    *status = (as608_status_t)handle->status;                                  /* set status */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as608_set_baud_rate(as608_handle_t *handle, uint32_t addr, uint8_t n_9600, as608_status_t *status)
{
    uint8_t res;
    uint8_t buf[3];
    uint16_t len;
    uint32_t addr_check;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    buf[0] = AS608_COMMAND_WRITE_REG;                                          /* write reg */
    buf[1] = 0x04;                                                             /* baud rate */
    buf[2] = n_9600;                                                           /* n x 9600 */
    res = a_as608_uart_write(handle, addr, AS608_TYPE_COMMAND, buf, 3);        /* write data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("as608: uart write failed.\n");                    /* uart write failed */
        
        return 1;                                                              /* return error */
    }
    len = 1;                                                                   /* len 1 */
    res = a_as608_uart_decode(handle, 300, &addr_check, buf, &len);            /* decode */
    if (res != 0)                                                              /* check result */
    {
        return 4;                                                              /* return error */
    }
    if (addr_check != addr)                                                    /* check addr */
    {
        handle->debug_print("as608: addr is invalid.\n");                      /* addr is invalid */
        
        return 5;                                                              /* return error */
    }
    if (len != 1)                                                              /* check length */
    {
        handle->debug_print("as608: len is invalid.\n");                       /* len is invalid */
        
        return 6;                                                              /* return error */
    }
    handle->status = buf[0];                                                   /* save status */
    *status = (as608_status_t)handle->status;                                  /* set status */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as608_set_level(as608_handle_t *handle, uint32_t addr, as608_level_t level, as608_status_t *status)
{
    uint8_t res;
    uint8_t buf[3];
    uint16_t len;
    uint32_t addr_check;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    buf[0] = AS608_COMMAND_WRITE_REG;                                          /* write reg */
    buf[1] = 0x05;                                                             /* level settings */
    buf[2] = level;                                                            /* set level */
    res = a_as608_uart_write(handle, addr, AS608_TYPE_COMMAND, buf, 3);        /* write data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("as608: uart write failed.\n");                    /* uart write failed */
        
        return 1;                                                              /* return error */
    }
    len = 1;                                                                   /* len 1 */
    res = a_as608_uart_decode(handle, 300, &addr_check, buf, &len);            /* decode */
    if (res != 0)                                                              /* check result */
    {
        return 4;                                                              /* return error */
    }
    if (addr_check != addr)                                                    /* check addr */
    {
        handle->debug_print("as608: addr is invalid.\n");                      /* addr is invalid */
        
        return 5;                                                              /* return error */
    }
    if (len != 1)                                                              /* check length */
    {
        handle->debug_print("as608: len is invalid.\n");                       /* len is invalid */
        
        return 6;                                                              /* return error */
    }
    handle->status = buf[0];                                                   /* save status */
    *status = (as608_status_t)handle->status;                                  /* set status */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as608_set_packet_size(as608_handle_t *handle, uint32_t addr, as608_packet_size_t size, as608_status_t *status)
{
    uint8_t res;
    uint8_t buf[3];
    uint16_t len;
    uint32_t addr_check;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    buf[0] = AS608_COMMAND_WRITE_REG;                                          /* write reg */
    buf[1] = 0x06;                                                             /* size settings */
    buf[2] = size;                                                             /* set size */
    res = a_as608_uart_write(handle, addr, AS608_TYPE_COMMAND, buf, 3);        /* write data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("as608: uart write failed.\n");                    /* uart write failed */
        
        return 1;                                                              /* return error */
    }
    len = 1;                                                                   /* len 1 */
    res = a_as608_uart_decode(handle, 300, &addr_check, buf, &len);            /* decode */
    if (res != 0)                                                              /* check result */
    {
        return 4;                                                              /* return error */
    }
    if (addr_check != addr)                                                    /* check addr */
    {
        handle->debug_print("as608: addr is invalid.\n");                      /* addr is invalid */
        
        return 5;                                                              /* return error */
    }
    if (len != 1)                                                              /* check length */
    {
        handle->debug_print("as608: len is invalid.\n");                       /* len is invalid */
        
        return 6;                                                              /* return error */
    }
    handle->status = buf[0];                                                   /* save status */
    *status = (as608_status_t)handle->status;                                  /* set status */
    handle->packet_size = (uint16_t)(32.0f * powf(2.0f, 
                          (float)((uint16_t)size)));                           /* packet size */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as608_get_params(as608_handle_t *handle, uint32_t addr, as608_params_t *param, as608_status_t *status)
{
    uint8_t res;
    uint8_t buf[17];
    uint16_t len;
    uint32_t addr_check;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    buf[0] = AS608_COMMAND_READ_SYS_PARA;                                      /* read sys para */
    res = a_as608_uart_write(handle, addr, AS608_TYPE_COMMAND, buf, 1);        /* write data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("as608: uart write failed.\n");                    /* uart write failed */
        
        return 1;                                                              /* return error */
    }
    len = 17;                                                                  /* len 17 */
    res = a_as608_uart_decode(handle, 300, &addr_check, buf, &len);            /* decode */
    if (res != 0)                                                              /* check result */
    {
        return 4;                                                              /* return error */
    }
    if (addr_check != addr)                                                    /* check addr */
    {
        handle->debug_print("as608: addr is invalid.\n");                      /* addr is invalid */
        
        return 5;                                                              /* return error */
    }
    if (len != 17)                                                             /* check length */
    {
        handle->debug_print("as608: len is invalid.\n");                       /* len is invalid */
        
        return 6;                                                              /* return error */
    }
    handle->status = buf[0];                                                   /* save status */
    *status = (as608_status_t)handle->status;                                  /* set status */
    param->status = (uint16_t)((uint16_t)buf[1] << 8) | buf[2];                /* set current status */
    param->sensor_type = (as608_sensor_type_t)
                         ((uint16_t)((uint16_t)buf[3] << 8) | buf[4]);         /* set sensor type */
    param->fingerprint_size = (uint16_t)((uint16_t)buf[5] << 8) | buf[6];      /* set fingerprint size */
    param->level = (as608_level_t)
                   ((uint16_t)((uint16_t)buf[7] << 8) | buf[8]);               /* set level */
    param->address = (uint32_t)((uint32_t)buf[9] << 24) |
                     (uint32_t)((uint32_t)buf[10] << 16) |
                     (uint32_t)((uint32_t)buf[11] << 8) |
                     (uint32_t)((uint32_t)buf[12] << 0);                       /* set address */
    param->packet_size = (as608_packet_size_t)
                         ((uint16_t)((uint16_t)buf[13] << 8) | buf[14]);       /* set packet size */
    param->n_9600 = (uint16_t)((uint16_t)buf[15] << 8) | buf[16];              /* set n_9600 */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as608_enroll(as608_handle_t *handle, uint32_t addr, uint16_t *page_number, as608_status_t *status)
{
    uint8_t res;
    uint8_t buf[3];
    uint16_t len;
    uint32_t addr_check;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    buf[0] = AS608_COMMAND_ENROLL;                                             /* enroll */
    res = a_as608_uart_write(handle, addr, AS608_TYPE_COMMAND, buf, 1);        /* write data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("as608: uart write failed.\n");                    /* uart write failed */
        
        return 1;                                                              /* return error */
    }
    len = 3;                                                                   /* len 3 */
    res = a_as608_uart_decode(handle, 1000, &addr_check, buf, &len);           /* decode */
    if (res != 0)                                                              /* check result */
    {
        return 4;                                                              /* return error */
    }
    if (addr_check != addr)                                                    /* check addr */
    {
        handle->debug_print("as608: addr is invalid.\n");                      /* addr is invalid */
        
        return 5;                                                              /* return error */
    }
    if (len != 3)                                                              /* check length */
    {
        handle->debug_print("as608: len is invalid.\n");                       /* len is invalid */
        
        return 6;                                                              /* return error */
    }
    handle->status = buf[0];                                                   /* save status */
    *status = (as608_status_t)handle->status;                                  /* set status */
    *page_number = (uint16_t)((uint16_t)buf[1]) | buf[2];                      /* set page number */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as608_identify(as608_handle_t *handle, uint32_t addr, uint16_t *page_number, uint16_t *score, as608_status_t *status)
{
    uint8_t res;
    uint8_t buf[5];
    uint16_t len;
    uint32_t addr_check;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    buf[0] = AS608_COMMAND_IDENTIFY;                                           /* identify */
    res = a_as608_uart_write(handle, addr, AS608_TYPE_COMMAND, buf, 1);        /* write data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("as608: uart write failed.\n");                    /* uart write failed */
        
        return 1;                                                              /* return error */
    }
    len = 5;                                                                   /* len 5 */
    res = a_as608_uart_decode(handle, 1000, &addr_check, buf, &len);           /* decode */
    if (res != 0)                                                              /* check result */
    {
        return 4;                                                              /* return error */
    }
    if (addr_check != addr)                                                    /* check addr */
    {
        handle->debug_print("as608: addr is invalid.\n");                      /* addr is invalid */
        
        return 5;                                                              /* return error */
    }
    if (len != 5)                                                              /* check length */
    {
        handle->debug_print("as608: len is invalid.\n");                       /* len is invalid */
        
        return 6;                                                              /* return error */
    }
    handle->status = buf[0];                                                   /* save status */
    *status = (as608_status_t)handle->status;                                  /* set status */
    *page_number = (uint16_t)((uint16_t)buf[1]) | buf[2];                      /* set page number */
    *score = (uint16_t)((uint16_t)buf[3]) | buf[4];                            /* set score */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as608_set_password(as608_handle_t *handle, uint32_t addr, uint32_t password, as608_status_t *status)
{
    uint8_t res;
    uint8_t buf[5];
    uint16_t len;
    uint32_t addr_check;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    buf[0] = AS608_COMMAND_SET_PWD;                                            /* set password */
    buf[1] = (password >> 24) & 0xFF;                                          /* set password */
    buf[2] = (password >> 16) & 0xFF;                                          /* set password */
    buf[3] = (password >> 8) & 0xFF;                                           /* set password */
    buf[4] = (password >> 0) & 0xFF;                                           /* set password */
    res = a_as608_uart_write(handle, addr, AS608_TYPE_COMMAND, buf, 5);        /* write data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("as608: uart write failed.\n");                    /* uart write failed */
        
        return 1;                                                              /* return error */
    }
    len = 1;                                                                   /* len 1 */
    res = a_as608_uart_decode(handle, 300, &addr_check, buf, &len);            /* decode */
    if (res != 0)                                                              /* check result */
    {
        return 4;                                                              /* return error */
    }
    if (addr_check != addr)                                                    /* check addr */
    {
        handle->debug_print("as608: addr is invalid.\n");                      /* addr is invalid */
        
        return 5;                                                              /* return error */
    }
    if (len != 1)                                                              /* check length */
    {
        handle->debug_print("as608: len is invalid.\n");                       /* len is invalid */
        
        return 6;                                                              /* return error */
    }
    handle->status = buf[0];                                                   /* save status */
    *status = (as608_status_t)handle->status;                                  /* set status */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as608_verify_password(as608_handle_t *handle, uint32_t addr, uint32_t password, as608_status_t *status)
{
    uint8_t res;
    uint8_t buf[5];
    uint16_t len;
    uint32_t addr_check;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    buf[0] = AS608_COMMAND_VFY_PWD;                                            /* verify password */
    buf[1] = (password >> 24) & 0xFF;                                          /* set password */
    buf[2] = (password >> 16) & 0xFF;                                          /* set password */
    buf[3] = (password >> 8) & 0xFF;                                           /* set password */
    buf[4] = (password >> 0) & 0xFF;                                           /* set password */
    res = a_as608_uart_write(handle, addr, AS608_TYPE_COMMAND, buf, 5);        /* write data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("as608: uart write failed.\n");                    /* uart write failed */
        
        return 1;                                                              /* return error */
    }
    len = 1;                                                                   /* len 1 */
    res = a_as608_uart_decode(handle, 300, &addr_check, buf, &len);            /* decode */
    if (res != 0)                                                              /* check result */
    {
        return 4;                                                              /* return error */
    }
    if (addr_check != addr)                                                    /* check addr */
    {
        handle->debug_print("as608: addr is invalid.\n");                      /* addr is invalid */
        
        return 5;                                                              /* return error */
    }
    if (len != 1)                                                              /* check length */
    {
        handle->debug_print("as608: len is invalid.\n");                       /* len is invalid */
        
        return 6;                                                              /* return error */
    }
    handle->status = buf[0];                                                   /* save status */
    *status = (as608_status_t)handle->status;                                  /* set status */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as608_get_random(as608_handle_t *handle, uint32_t addr, uint32_t *randn, as608_status_t *status)
{
    uint8_t res;
    uint8_t buf[5];
    uint16_t len;
    uint32_t addr_check;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    buf[0] = AS608_COMMAND_GET_RANDOM_CODE;                                    /* get random code */
    res = a_as608_uart_write(handle, addr, AS608_TYPE_COMMAND, buf, 1);        /* write data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("as608: uart write failed.\n");                    /* uart write failed */
        
        return 1;                                                              /* return error */
    }
    len = 5;                                                                   /* len 5 */
    res = a_as608_uart_decode(handle, 300, &addr_check, buf, &len);            /* decode */
    if (res != 0)                                                              /* check result */
    {
        return 4;                                                              /* return error */
    }
    if (addr_check != addr)                                                    /* check addr */
    {
        handle->debug_print("as608: addr is invalid.\n");                      /* addr is invalid */
        
        return 5;                                                              /* return error */
    }
    if (len != 5)                                                              /* check length */
    {
        handle->debug_print("as608: len is invalid.\n");                       /* len is invalid */
        
        return 6;                                                              /* return error */
    }
    handle->status = buf[0];                                                   /* save status */
    *status = (as608_status_t)handle->status;                                  /* set status */
    *randn = (uint32_t)((uint32_t)buf[1] << 24) |
              (uint32_t)((uint32_t)buf[2] << 16) |
              (uint32_t)((uint32_t)buf[3] << 8) |
              (uint32_t)((uint32_t)buf[4] << 0);                               /* get the random */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as608_set_chip_address(as608_handle_t *handle, uint32_t addr, uint32_t new_addr, as608_status_t *status)
{
    uint8_t res;
    uint8_t buf[5];
    uint16_t len;
    uint32_t addr_check;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    buf[0] = AS608_COMMAND_SET_CHIP_ADDR;                                      /* set chip addr */
    buf[1] = (new_addr >> 24) & 0xFF;                                          /* set chip addr */
    buf[2] = (new_addr >> 16) & 0xFF;                                          /* set chip addr*/
    buf[3] = (new_addr >> 8) & 0xFF;                                           /* set chip addr */
    buf[4] = (new_addr >> 0) & 0xFF;                                           /* set chip addr */
    res = a_as608_uart_write(handle, addr, AS608_TYPE_COMMAND, buf, 5);        /* write data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("as608: uart write failed.\n");                    /* uart write failed */
        
        return 1;                                                              /* return error */
    }
    len = 1;                                                                   /* len 1 */
    res = a_as608_uart_decode(handle, 300, &addr_check, buf, &len);            /* decode */
    if (res != 0)                                                              /* check result */
    {
        return 4;                                                              /* return error */
    }
    if (addr_check != new_addr)                                                /* check addr */
    {
        handle->debug_print("as608: addr is invalid.\n");                      /* addr is invalid */
        
        return 5;                                                              /* return error */
    }
    if (len != 1)                                                              /* check length */
    {
        handle->debug_print("as608: len is invalid.\n");                       /* len is invalid */
        
        return 6;                                                              /* return error */
    }
    handle->status = buf[0];                                                   /* save status */
    *status = (as608_status_t)handle->status;                                  /* set status */
    
    return 0;                                                                  /* success return 0 */
}

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
                                    uint8_t *output_buffer, uint16_t *output_len, as608_status_t *status)
{
    uint8_t res;
    uint8_t buf[1];
    uint16_t l;
    uint16_t point;
    uint16_t len;
    uint32_t addr_check;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    buf[0] = AS608_COMMAND_READ_INFO_PAGE;                                     /* read info page */
    res = a_as608_uart_write(handle, addr, AS608_TYPE_COMMAND, buf, 1);        /* write data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("as608: uart write failed.\n");                    /* uart write failed */
        
        return 1;                                                              /* return error */
    }
    len = 1;                                                                   /* len 1 */
    res = a_as608_uart_decode_with_length(handle, 12, 100, &addr_check, 
                                          buf, &len);                          /* decode */
    if (res != 0)                                                              /* check result */
    {
        return 4;                                                              /* return error */
    }
    if (addr_check != addr)                                                    /* check addr */
    {
        handle->debug_print("as608: addr is invalid.\n");                      /* addr is invalid */
        
        return 5;                                                              /* return error */
    }
    if (len != 1)                                                              /* check length */
    {
        handle->debug_print("as608: len is invalid.\n");                       /* len is invalid */
        
        return 6;                                                              /* return error */
    }
    handle->status = buf[0];                                                   /* save status */
    *status = (as608_status_t)handle->status;                                  /* set status */
    point = 0;                                                                 /* init 0 */
    len = *output_len;                                                         /* get the length */
    l = *output_len;                                                           /* get the length */
    while (1)                                                                  /* loop */
    {
        as608_bool_t end_enable;
        as608_bool_t full_enable;
        
        l = len;                                                               /* set the remain max buffer size */
        res = a_as608_uart_parse_data(handle, 100, &addr_check, 
                                      output_buffer + point, 
                                      &l, &end_enable, &full_enable);          /* parse the data */
        if (res != 0)
        {
            return 4;                                                          /* return error */
        }
        if (addr_check != addr)                                                /* check addr */
        {
            handle->debug_print("as608: addr is invalid.\n");                  /* addr is invalid */
            
            return 5;                                                          /* return error */
        }
        len -= l;                                                              /* remain length - read length */
        point += l;                                                            /* current point adds read length */
        if (full_enable == AS608_BOOL_TRUE)                                    /* if buffer is full */
        {
            handle->debug_print("as608: buffer is full.\n");                   /* buffer is full */
            
            return 7;                                                          /* return error */
        }
        if (end_enable == AS608_BOOL_TRUE)                                     /* if the last packet */
        {
            break;                                                             /* break */
        }
    }
    *output_len = point;                                                       /* save the length */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as608_set_port(as608_handle_t *handle, uint32_t addr, as608_bool_t enable, as608_status_t *status)
{
    uint8_t res;
    uint8_t buf[2];
    uint16_t len;
    uint32_t addr_check;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    buf[0] = AS608_COMMAND_PORT_CONTROL;                                       /* port control */
    buf[1] = enable;                                                           /* set chip addr */
    res = a_as608_uart_write(handle, addr, AS608_TYPE_COMMAND, buf, 2);        /* write data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("as608: uart write failed.\n");                    /* uart write failed */
        
        return 1;                                                              /* return error */
    }
    len = 1;                                                                   /* len 1 */
    res = a_as608_uart_decode(handle, 300, &addr_check, buf, &len);            /* decode */
    if (res != 0)                                                              /* check result */
    {
        return 4;                                                              /* return error */
    }
    if (addr_check != addr)                                                    /* check addr */
    {
        handle->debug_print("as608: addr is invalid.\n");                      /* addr is invalid */
        
        return 5;                                                              /* return error */
    }
    if (len != 1)                                                              /* check length */
    {
        handle->debug_print("as608: len is invalid.\n");                       /* len is invalid */
        
        return 6;                                                              /* return error */
    }
    handle->status = buf[0];                                                   /* save status */
    *status = (as608_status_t)handle->status;                                  /* set status */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as608_write_notepad(as608_handle_t *handle, uint32_t addr, uint8_t page_number, uint8_t data[32], as608_status_t *status)
{
    uint8_t res;
    uint8_t buf[34];
    uint16_t len;
    uint32_t addr_check;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    if (page_number > 16)                                                      /* check page number */
    {
        handle->debug_print("as608: page_number > 16.\n");                     /* page_number > 16 */
        
        return 7;                                                              /* return error */
    }
    
    buf[0] = AS608_COMMAND_WRITE_NOTEPAD;                                      /* write notepad */
    buf[1] = page_number;                                                      /* set page number */
    memcpy(&buf[2], data, 32);                                                 /* copy data */
    res = a_as608_uart_write(handle, addr, AS608_TYPE_COMMAND, buf, 34);       /* write data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("as608: uart write failed.\n");                    /* uart write failed */
        
        return 1;                                                              /* return error */
    }
    len = 1;                                                                   /* len 1 */
    res = a_as608_uart_decode(handle, 300, &addr_check, buf, &len);            /* decode */
    if (res != 0)                                                              /* check result */
    {
        return 4;                                                              /* return error */
    }
    if (addr_check != addr)                                                    /* check addr */
    {
        handle->debug_print("as608: addr is invalid.\n");                      /* addr is invalid */
        
        return 5;                                                              /* return error */
    }
    if (len != 1)                                                              /* check length */
    {
        handle->debug_print("as608: len is invalid.\n");                       /* len is invalid */
        
        return 6;                                                              /* return error */
    }
    handle->status = buf[0];                                                   /* save status */
    *status = (as608_status_t)handle->status;                                  /* set status */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as608_read_notepad(as608_handle_t *handle, uint32_t addr, uint8_t page_number, uint8_t data[32], as608_status_t *status)
{
    uint8_t res;
    uint8_t buf[33];
    uint16_t len;
    uint32_t addr_check;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    if (page_number > 16)                                                      /* check page number */
    {
        handle->debug_print("as608: page_number > 16.\n");                     /* page_number > 16 */
        
        return 7;                                                              /* return error */
    }
    
    buf[0] = AS608_COMMAND_READ_NOTEPAD;                                       /* read notepad */
    buf[1] = page_number;                                                      /* set page number */
    res = a_as608_uart_write(handle, addr, AS608_TYPE_COMMAND, buf, 2);        /* write data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("as608: uart write failed.\n");                    /* uart write failed */
        
        return 1;                                                              /* return error */
    }
    len = 33;                                                                  /* len 33 */
    res = a_as608_uart_decode(handle, 300, &addr_check, buf, &len);            /* decode */
    if (res != 0)                                                              /* check result */
    {
        return 4;                                                              /* return error */
    }
    if (addr_check != addr)                                                    /* check addr */
    {
        handle->debug_print("as608: addr is invalid.\n");                      /* addr is invalid */
        
        return 5;                                                              /* return error */
    }
    if (len != 33)                                                             /* check length */
    {
        handle->debug_print("as608: len is invalid.\n");                       /* len is invalid */
        
        return 6;                                                              /* return error */
    }
    handle->status = buf[0];                                                   /* save status */
    *status = (as608_status_t)handle->status;                                  /* set status */
    memcpy(data, &buf[1], 32);                                                 /* copy data */
    
    return 0;                                                                  /* success return 0 */
}

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
                        uint8_t *input_buffer, uint16_t input_len, as608_status_t *status)
{
    uint8_t res;
    uint8_t buf[2];
    uint16_t i;
    uint16_t m;
    uint16_t n;
    uint16_t len;
    uint32_t addr_check;
    
    if (handle == NULL)                                                                        /* check handle */
    {
        return 2;                                                                              /* return error */
    }
    if (handle->inited != 1)                                                                   /* check handle initialization */
    {
        return 3;                                                                              /* return error */
    }
    
    buf[0] = AS608_COMMAND_BURN_CODE;                                                          /* burn code */
    buf[1] = mode;                                                                             /* set burn code */
    res = a_as608_uart_write(handle, addr, AS608_TYPE_COMMAND, buf, 2);                        /* write data */
    if (res != 0)                                                                              /* check result */
    {
        handle->debug_print("as608: uart write failed.\n");                                    /* uart write failed */
        
        return 1;                                                                              /* return error */
    }
    len = 1;                                                                                   /* len 1 */
    res = a_as608_uart_decode_with_length(handle, 12, 100, &addr_check, buf, &len);            /* decode */
    if (res != 0)                                                                              /* check result */
    {
        return 4;                                                                              /* return error */
    }
    if (addr_check != addr)                                                                    /* check addr */
    {
        handle->debug_print("as608: addr is invalid.\n");                                      /* addr is invalid */
        
        return 5;                                                                              /* return error */
    }
    if (len != 1)                                                                              /* check length */
    {
        handle->debug_print("as608: len is invalid.\n");                                       /* len is invalid */
        
        return 6;                                                                              /* return error */
    }
    handle->status = buf[0];                                                                   /* save status */
    *status = (as608_status_t)handle->status;                                                  /* set status */
    
    m = input_len / handle->packet_size;                                                       /* set m */
    n = input_len % handle->packet_size;                                                       /* set n */
    if (n != 0)                                                                                /* check n */
    {
        for (i = 0; i < m; i++)                                                                /* m times */
        {
            res = a_as608_uart_write(handle, addr, AS608_TYPE_DATA, 
                                     input_buffer + i * handle->packet_size, 
                                     handle->packet_size);                                     /* write data */
            if (res != 0)                                                                      /* check result */
            {
                handle->debug_print("as608: uart write failed.\n");                            /* uart write failed */
                
                return 1;                                                                      /* return error */
            }
            handle->delay_ms(100);                                                             /* delay 100ms */
        }
        res = a_as608_uart_write(handle, addr, AS608_TYPE_END, 
                                 input_buffer + m * handle->packet_size, n);                   /* write data */
        if (res != 0)                                                                          /* check result */
        {
            handle->debug_print("as608: uart write failed.\n");                                /* uart write failed */
            
            return 1;                                                                          /* return error */
        }
        handle->delay_ms(100);                                                                 /* delay 100ms */
    }
    else
    {
        for (i = 0; i < (m - 1); i++)                                                          /* m - 1 times */
        {
            res = a_as608_uart_write(handle, addr, AS608_TYPE_DATA, 
                                     input_buffer + i * handle->packet_size, 
                                     handle->packet_size);                                     /* write data */
            if (res != 0)                                                                      /* check result */
            {
                handle->debug_print("as608: uart write failed.\n");                            /* uart write failed */
                
                return 1;                                                                      /* return error */
            }
            handle->delay_ms(100);                                                             /* delay 100ms */
        }
        res = a_as608_uart_write(handle, addr, AS608_TYPE_END, 
                                 input_buffer + (m - 1) * handle->packet_size, 
                                 handle->packet_size);                                         /* write data */
        if (res != 0)                                                                          /* check result */
        {
            handle->debug_print("as608: uart write failed.\n");                                /* uart write failed */
            
            return 1;                                                                          /* return error */
        }
        handle->delay_ms(100);                                                                 /* delay 100ms */
    }
    
    return 0;                                                                                  /* success return 0 */
}

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
                                as608_status_t *status)
{
    uint8_t res;
    uint8_t buf[6];
    uint16_t len;
    uint32_t addr_check;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    buf[0] = AS608_COMMAND_HIGH_SPEED_SEARCH;                                  /* high speed search */
    buf[1] = buffer_number;                                                    /* set buffer number */
    buf[2] = (start_page >> 8) & 0xFF;                                         /* start page msb */
    buf[3] = (start_page >> 0) & 0xFF;                                         /* start page lsb */
    buf[4] = (page_number >> 8) & 0xFF;                                        /* page number msb */
    buf[5] = (page_number >> 0) & 0xFF;                                        /* page number lsb */
    res = a_as608_uart_write(handle, addr, AS608_TYPE_COMMAND, buf, 6);        /* write data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("as608: uart write failed.\n");                    /* uart write failed */
        
        return 1;                                                              /* return error */
    }
    len = 5;                                                                   /* len 5 */
    res = a_as608_uart_decode(handle, 300, &addr_check, buf, &len);            /* decode */
    if (res != 0)                                                              /* check result */
    {
        return 4;                                                              /* return error */
    }
    if (addr_check != addr)                                                    /* check addr */
    {
        handle->debug_print("as608: addr is invalid.\n");                      /* addr is invalid */
        
        return 5;                                                              /* return error */
    }
    if (len != 5)                                                              /* check length */
    {
        handle->debug_print("as608: len is invalid.\n");                       /* len is invalid */
        
        return 6;                                                              /* return error */
    }
    handle->status = buf[0];                                                   /* save status */
    *status = (as608_status_t)handle->status;                                  /* set status */
    *found_page = (uint16_t)((uint16_t)buf[1] << 8) | buf[2];                  /* set found page */
    *score = (uint16_t)((uint16_t)buf[3] << 8) | buf[4];                       /* set score */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as608_generate_bin_image(as608_handle_t *handle, uint32_t addr, as608_image_t image, as608_status_t *status)
{
    uint8_t res;
    uint8_t buf[2];
    uint16_t len;
    uint32_t addr_check;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    buf[0] = AS608_COMMAND_GEN_BIN_IMAGE;                                      /* generate bin image */
    buf[1] = image;                                                            /* set image */
    res = a_as608_uart_write(handle, addr, AS608_TYPE_COMMAND, buf, 2);        /* write data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("as608: uart write failed.\n");                    /* uart write failed */
        
        return 1;                                                              /* return error */
    }
    len = 1;                                                                   /* len 1 */
    res = a_as608_uart_decode(handle, 300, &addr_check, buf, &len);            /* decode */
    if (res != 0)                                                              /* check result */
    {
        return 4;                                                              /* return error */
    }
    if (addr_check != addr)                                                    /* check addr */
    {
        handle->debug_print("as608: addr is invalid.\n");                      /* addr is invalid */
        
        return 5;                                                              /* return error */
    }
    if (len != 1)                                                              /* check length */
    {
        handle->debug_print("as608: len is invalid.\n");                       /* len is invalid */
        
        return 6;                                                              /* return error */
    }
    handle->status = buf[0];                                                   /* save status */
    *status = (as608_status_t)handle->status;                                  /* set status */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as608_get_valid_template_number(as608_handle_t *handle, uint32_t addr, uint16_t *num, as608_status_t *status)
{
    uint8_t res;
    uint8_t buf[3];
    uint16_t len;
    uint32_t addr_check;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    buf[0] = AS608_COMMAND_VALID_TEMPLATE_NUM;                                 /* valid template num */
    res = a_as608_uart_write(handle, addr, AS608_TYPE_COMMAND, buf, 1);        /* write data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("as608: uart write failed.\n");                    /* uart write failed */
        
        return 1;                                                              /* return error */
    }
    len = 3;                                                                   /* len 3 */
    res = a_as608_uart_decode(handle, 300, &addr_check, buf, &len);            /* decode */
    if (res != 0)                                                              /* check result */
    {
        return 4;                                                              /* return error */
    }
    if (addr_check != addr)                                                    /* check addr */
    {
        handle->debug_print("as608: addr is invalid.\n");                      /* addr is invalid */
        
        return 5;                                                              /* return error */
    }
    if (len != 3)                                                              /* check length */
    {
        handle->debug_print("as608: len is invalid.\n");                       /* len is invalid */
        
        return 6;                                                              /* return error */
    }
    handle->status = buf[0];                                                   /* save status */
    *status = (as608_status_t)handle->status;                                  /* set status */
    *num = (uint16_t)((uint16_t)buf[1] << 8) | buf[2];                         /* get the number */
    
    return 0;                                                                  /* success return 0 */
}

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
                             as608_gpio_level_t input_level, as608_gpio_level_t *output_level, as608_status_t *status)
{
    uint8_t res;
    uint8_t buf[3];
    uint16_t len;
    uint32_t addr_check;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    buf[0] = AS608_COMMAND_USER_GPIO;                                          /* user gpio */
    buf[1] = gpio;                                                             /* set gpio number */
    buf[2] = input_level;                                                      /* set gpio input level */
    res = a_as608_uart_write(handle, addr, AS608_TYPE_COMMAND, buf, 3);        /* write data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("as608: uart write failed.\n");                    /* uart write failed */
        
        return 1;                                                              /* return error */
    }
    len = 2;                                                                   /* len 2 */
    res = a_as608_uart_decode(handle, 300, &addr_check, buf, &len);            /* decode */
    if (res != 0)                                                              /* check result */
    {
        return 4;                                                              /* return error */
    }
    if (addr_check != addr)                                                    /* check addr */
    {
        handle->debug_print("as608: addr is invalid.\n");                      /* addr is invalid */
        
        return 5;                                                              /* return error */
    }
    if (len != 2)                                                              /* check length */
    {
        handle->debug_print("as608: len is invalid.\n");                       /* len is invalid */
        
        return 6;                                                              /* return error */
    }
    handle->status = buf[0];                                                   /* save status */
    *status = (as608_status_t)handle->status;                                  /* set status */
    *output_level = (as608_gpio_level_t)(buf[1]);                              /* get gpio level */
    
    return 0;                                                                  /* success return 0 */
}

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
                              uint8_t table[32], as608_status_t *status)
{
    uint8_t res;
    uint8_t buf[33];
    uint16_t len;
    uint32_t addr_check;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    if (num > 3)                                                               /* check num */
    {
        handle->debug_print("as608: num > 3.\n");                              /* num > 3 */
        
        return 7;                                                              /* return error */
    }
    
    buf[0] = AS608_COMMAND_READ_INDEX_TABLE;                                   /* read index table */
    buf[1] = num;                                                              /* set index table number */
    res = a_as608_uart_write(handle, addr, AS608_TYPE_COMMAND, buf, 2);        /* write data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("as608: uart write failed.\n");                    /* uart write failed */
        
        return 1;                                                              /* return error */
    }
    len = 33;                                                                  /* len 33 */
    res = a_as608_uart_decode(handle, 300, &addr_check, buf, &len);            /* decode */
    if (res != 0)                                                              /* check result */
    {
        return 4;                                                              /* return error */
    }
    if (addr_check != addr)                                                    /* check addr */
    {
        handle->debug_print("as608: addr is invalid.\n");                      /* addr is invalid */
        
        return 5;                                                              /* return error */
    }
    if (len != 33)                                                             /* check length */
    {
        handle->debug_print("as608: len is invalid.\n");                       /* len is invalid */
        
        return 6;                                                              /* return error */
    }
    handle->status = buf[0];                                                   /* save status */
    *status = (as608_status_t)handle->status;                                  /* set status */
    memcpy(table, &buf[1], 32);                                                /* copy data */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as608_print_status(as608_handle_t *handle, as608_status_t status)
{
    if (handle == NULL)                                                          /* check handle */
    {
        return 2;                                                                /* return error */
    }
    if (handle->inited != 1)                                                     /* check handle initialization */
    {
        return 3;                                                                /* return error */
    }
    
    switch (status)                                                              /* switch status */
    {
        case AS608_STATUS_OK :                                                   /* status case */
        {
            handle->debug_print("as608: ok.\n");                                 /* print message */
            
            break;                                                               /* break */
        }
        case AS608_STATUS_FRAME_ERROR :                                          /* status case */
        {
            handle->debug_print("as608: frame error.\n");                        /* print message */
            
            break;                                                               /* break */
        }
        case AS608_STATUS_NO_FINGERPRINT :                                       /* status case */
        {
            handle->debug_print("as608: no fingerprint.\n");                     /* print message */
            
            break;                                                               /* break */
        }
        case AS608_STATUS_INPUT_ERROR :                                          /* status case */
        {
            handle->debug_print("as608: fingerprint image error.\n");            /* print message */
            
            break;                                                               /* break */
        }
        case AS608_STATUS_IMAGE_TOO_DRY :                                        /* status case */
        {
            handle->debug_print("as608: fingerprint image too dry.\n");          /* print message */
            
            break;                                                               /* break */
        }
        case AS608_STATUS_IMAGE_TOO_WET :                                        /* status case */
        {
            handle->debug_print("as608: fingerprint image too wet.\n");          /* print message */
            
            break;                                                               /* break */
        }
        case AS608_STATUS_IMAGE_TOO_CLUTTER :                                    /* status case */
        {
            handle->debug_print("as608: fingerprint too clutter.\n");            /* print message */
            
            break;                                                               /* break */
        }
        case AS608_STATUS_IMAGE_TOO_FEW_FEATURE :                                /* status case */
        {
            handle->debug_print("as608: fingerprint feature too few.\n");        /* print message */
            
            break;                                                               /* break */
        }
        case AS608_STATUS_NOT_MATCH :                                            /* status case */
        {
            handle->debug_print("as608: not match.\n");                          /* print message */
            
            break;                                                               /* break */
        }
        case AS608_STATUS_NOT_FOUND :                                            /* status case */
        {
            handle->debug_print("as608: not found.\n");                          /* print message */
            
            break;                                                               /* break */
        }
        case AS608_STATUS_FEATURE_COMBINE_ERROR :                                /* status case */
        {
            handle->debug_print("as608: feature combine error.\n");              /* print message */
            
            break;                                                               /* break */
        }
        case AS608_STATUS_LIB_ADDR_OVER :                                        /* status case */
        {
            handle->debug_print("as608: fingerprint lib addr is over.\n");       /* print message */
            
            break;                                                               /* break */
        }
        case AS608_STATUS_LIB_READ_ERROR :                                       /* status case */
        {
            handle->debug_print("as608: fingerprint lib read error.\n");         /* print message */
            
            break;                                                               /* break */
        }
        case AS608_STATUS_UPLOAD_FEATURE_ERROR :                                 /* status case */
        {
            handle->debug_print("as608: upload feature error.\n");               /* print message */
            
            break;                                                               /* break */
        }
        case AS608_STATUS_NO_FRAME :                                             /* status case */
        {
            handle->debug_print("as608: no frame.\n");                           /* print message */
            
            break;                                                               /* break */
        }
        case AS608_STATUS_UPLOAD_IMAGE_ERROR :                                   /* status case */
        {
            handle->debug_print("as608: upload image error.\n");                 /* print message */
            
            break;                                                               /* break */
        }
        case AS608_STATUS_LIB_DELETE_ERROR :                                     /* status case */
        {
            handle->debug_print("as608: delete lib error.\n");                   /* print message */
            
            break;                                                               /* break */
        }
        case AS608_STATUS_LIB_CLEAR_ERROR :                                      /* status case */
        {
            handle->debug_print("as608: clear lib error.\n");                    /* print message */
            
            break;                                                               /* break */
        }
        case AS608_STATUS_ENTER_LOW_POWER_ERROR :                                /* status case */
        {
            handle->debug_print("as608: enter low power error.\n");              /* print message */
            
            break;                                                               /* break */
        }
        case AS608_STATUS_COMMAND_INVALID :                                      /* status case */
        {
            handle->debug_print("as608: command invalid.\n");                    /* print message */
            
            break;                                                               /* break */
        }
        case AS608_STATUS_RESET_ERROR :                                          /* status case */
        {
            handle->debug_print("as608: reset error.\n");                        /* print message */
            
            break;                                                               /* break */
        }
        case AS608_STATUS_BUFFER_INVALID :                                       /* status case */
        {
            handle->debug_print("as608: buffer invalid.\n");                     /* print message */
            
            break;                                                               /* break */
        }
        case AS608_STATUS_UPDATE_ERROR :                                         /* status case */
        {
            handle->debug_print("as608: update error.\n");                       /* print message */
            
            break;                                                               /* break */
        }
        case AS608_STATUS_NO_MOVE :                                              /* status case */
        {
            handle->debug_print("as608: no move.\n");                            /* print message */
            
            break;                                                               /* break */
        }
        case AS608_STATUS_FLASH_ERROR :                                          /* status case */
        {
            handle->debug_print("as608: flash error.\n");                        /* print message */
            
            break;                                                               /* break */
        }
        case AS608_STATUS_F0_RESPONSE :                                          /* status case */
        {
            handle->debug_print("as608: f0 response.\n");                        /* print message */
            
            break;                                                               /* break */
        }
        case AS608_STATUS_F1_RESPONSE :                                          /* status case */
        {
            handle->debug_print("as608: f1 response.\n");                        /* print message */
            
            break;                                                               /* break */
        }
        case AS608_STATUS_FLASH_WRITE_SUM_ERROR :                                /* status case */
        {
            handle->debug_print("as608: flash sum error.\n");                    /* print message */
            
            break;                                                               /* break */
        }
        case AS608_STATUS_FLASH_WRITE_HEADER_ERROR :                             /* status case */
        {
            handle->debug_print("as608: flash header error.\n");                 /* print message */
            
            break;                                                               /* break */
        }
        case AS608_STATUS_FLASH_WRITE_LENGTH_ERROR :                             /* status case */
        {
            handle->debug_print("as608: flash length error.\n");                 /* print message */
            
            break;                                                               /* break */
        }
        case AS608_STATUS_FLASH_WRITE_LENGTH_TOO_LONG :                          /* status case */
        {
            handle->debug_print("as608: flash length too long.\n");              /* print message */
            
            break;                                                               /* break */
        }
        case AS608_STATUS_FLASH_WRITE_ERROR :                                    /* status case */
        {
            handle->debug_print("as608: flash write error.\n");                  /* print message */
            
            break;                                                               /* break */
        }
        case AS608_STATUS_UNKNOWN :                                              /* status case */
        {
            handle->debug_print("as608: unknown.\n");                            /* print message */
            
            break;                                                               /* break */
        }
        case AS608_STATUS_REG_INVALID :                                          /* status case */
        {
            handle->debug_print("as608: reg invalid.\n");                        /* print message */
            
            break;                                                               /* break */
        }
        case AS608_STATUS_DATA_INVALID :                                         /* status case */
        {
            handle->debug_print("as608: data invalid.\n");                       /* print message */
            
            break;                                                               /* break */
        }
        case AS608_STATUS_NOTE_PAGE_INVALID :                                    /* status case */
        {
            handle->debug_print("as608: note page invalid.\n");                  /* print message */
            
            break;                                                               /* break */
        }
        case AS608_STATUS_PORT_INVALID :                                         /* status case */
        {
            handle->debug_print("as608: port invalid.\n");                       /* print message */
            
            break;                                                               /* break */
        }
        case AS608_STATUS_ENROOL_ERROR :                                         /* status case */
        {
            handle->debug_print("as608: enrool error.\n");                       /* print message */
            
            break;                                                               /* break */
        }
        case AS608_STATUS_LIB_FULL :                                             /* status case */
        {
            handle->debug_print("as608: lib full.\n");                           /* print message */
            
            break;                                                               /* break */
        }
        default :                                                                /* default */
        {
            handle->debug_print("as608: invalid type.\n");                       /* print message */
            
            break;                                                               /* break */
        }
    }
    
    return 0;                                                                    /* success return 0 */ 
}

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
                                 uint16_t ms, uint8_t *output_buffer, uint16_t *output_len)
{
    uint8_t res;
    uint32_t addr_check;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_as608_uart_write(handle, addr, type, input_buffer, input_len);                /* write data */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("as608: uart write failed.\n");                               /* uart write failed */
        
        return 1;                                                                         /* return error */
    }
    res = a_as608_uart_decode(handle, ms, &addr_check, output_buffer, output_len);        /* decode*/
    if (res != 0)                                                                         /* check result */
    {
        return 4;                                                                         /* return error */
    }
    if (addr_check != addr)                                                               /* check addr */
    {
        handle->debug_print("as608: addr is invalid.\n");                                 /* addr is invalid */
        
        return 5;                                                                         /* return error */
    }
    
    return 0;                                                                             /* success return 0 */ 
}

/**
 * @brief      get chip's information
 * @param[out] *info points to an as608 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t as608_info(as608_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(as608_info_t));                          /* initialize as608 info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "UART", 8);                            /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}
