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
 * @file      main.c
 * @brief     main source file
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

#include "driver_as608_basic.h"
#include "driver_as608_advance.h"
#include "driver_as608_fingerprint_test.h"
#include "driver_as608_register_test.h"
#include "ff.h"
#include "shell.h"
#include "clock.h"
#include "delay.h"
#include "uart.h"
#include "getopt.h"
#include <ctype.h>
#include <math.h>
#include <stdlib.h>

/**
 * @brief fs var definition
 */
FATFS g_fs;                            /**< fatfs handle */
FIL g_file;                            /**< fs handle */

/**
 * @brief global var definition
 */
uint8_t g_buf[256];                    /**< uart buffer */
uint8_t g_rx_buffer[36 * 1024];        /**< rx uart buffer length */
volatile uint16_t g_len;               /**< uart buffer length */

/**
 * @brief     input fingerprint callback
 * @param[in] status is the current status
 * @param[in] fmt is the format data
 * @note      none
 */
static void a_callback(int8_t status, const char *const fmt, ...)
{
    switch (status)
    {
        case -1 :
        {
            /* output */
            as608_interface_debug_print("%s", fmt);
        }
        case 0 :
        {
            /* delay 2000ms */
            as608_interface_delay_ms(2000);

            /* output */
            as608_interface_debug_print("%s", fmt);
        }
        case 1 :
        {
            /* delay 2000ms */
            as608_interface_delay_ms(2000);

            /* output */
            as608_interface_debug_print("%s", fmt);
        }
        case 2 :
        {
            /* output */
            as608_interface_debug_print("%s", fmt);
        }
        default :
        {
            break;
        }
    }
}

/**
 * @brief     as608 full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t as608(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"addr", required_argument, NULL, 1},
        {"data", required_argument, NULL, 2},
        {"num", required_argument, NULL, 3},
        {"path", required_argument, NULL, 4},
        {NULL, 0, NULL, 0},
    };
    char type[37] = "unknown";
    char path[33] = "tmp.data";
    uint8_t data[32] = {0};
    uint32_t num = 0;
    uint32_t addr = 0xFFFFFFFF;

    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }

    /* init 0 */
    optind = 0;

    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);

        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 37);
                snprintf(type, 36, "h");

                break;
            }

            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 37);
                snprintf(type, 36, "i");

                break;
            }

            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 37);
                snprintf(type, 36, "p");

                break;
            }

            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 37);
                snprintf(type, 36, "e_%s", optarg);

                break;
            }

            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 37);
                snprintf(type, 36, "t_%s", optarg);

                break;
            }

            /* addr */
            case 1 :
            {
                char *p;
                uint16_t l;
                uint16_t i;
                uint64_t hex_data;

                /* set the data */
                l = strlen(optarg);

                /* check the header */
                if (l >= 2)
                {
                    if (strncmp(optarg, "0x", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                    }
                    else if (strncmp(optarg, "0X", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                    }
                    else
                    {
                        p = optarg;
                    }
                }
                else
                {
                    p = optarg;
                }

                /* init 0 */
                hex_data = 0;

                /* loop */
                for (i = 0; i < l; i++)
                {
                    if ((p[i] <= '9') && (p[i] >= '0'))
                    {
                        hex_data += (p[i] - '0') * (uint32_t)pow(16, l - i - 1);
                    }
                    else if ((p[i] <= 'F') && (p[i] >= 'A'))
                    {
                        hex_data += ((p[i] - 'A') + 10) * (uint32_t)pow(16, l - i - 1);
                    }
                    else if ((p[i] <= 'f') && (p[i] >= 'a'))
                    {
                        hex_data += ((p[i] - 'a') + 10) * (uint32_t)pow(16, l - i - 1);
                    }
                    else
                    {
                        return 5;
                    }
                }

                /* set the data */
                addr = hex_data & 0xFFFFFFFF;

                break;
            }

            /* data */
            case 2 :
            {
                char *p;
                uint16_t l;
                uint16_t i;

                /* set the data */
                l = strlen(optarg);

                /* check the header */
                if (l >= 2)
                {
                    if (strncmp(optarg, "0x", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                    }
                    else if (strncmp(optarg, "0X", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                    }
                    else
                    {
                        p = optarg;
                    }
                }
                else
                {
                    p = optarg;
                }

                /* check length */
                if (l != 64)
                {
                    return 5;
                }

                /* loop */
                for (i = 0; i < l; i++)
                {
                    if ((p[i] <= '9') && (p[i] >= '0'))
                    {
                        data[i / 2] += (p[i] - '0') * (uint32_t)pow(16, (l - i - 1) % 2);
                    }
                    else if ((p[i] <= 'F') && (p[i] >= 'A'))
                    {
                        data[i / 2] += ((p[i] - 'A') + 10) * (uint32_t)pow(16, (l - i - 1) % 2);
                    }
                    else if ((p[i] <= 'f') && (p[i] >= 'a'))
                    {
                        data[i / 2] += ((p[i] - 'a') + 10) * (uint32_t)pow(16, (l - i - 1) % 2);
                    }
                    else
                    {
                        return 5;
                    }
                }

                break;
            }

            /* num */
            case 3 :
            {
                /* set the number */
                num = atoi(optarg);

                break;
            }

            /* path */
            case 4 :
            {
                /* set the path */
                memset(path, 0, sizeof(char) * 32);
                strncpy(path, optarg, 32);

                break;
            }

            /* the end */
            case -1 :
            {
                break;
            }

            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if (strcmp("t_reg", type) == 0)
    {
        /* run reg test */
        if (as608_register_test(addr) != 0)
        {
            return 1;
        }

        return 0;
    }
    else if (strcmp("t_fingerprint", type) == 0)
    {
        /* run fingerprint test */
        if (as608_fingerprint_test(addr) != 0)
        {
            return 1;
        }

        return 0;
    }
    else if (strcmp("e_basic-fingerprint", type) == 0)
    {
        uint8_t res;
        uint16_t score;
        uint16_t page_number;
        as608_status_t status;

        /* basic init */
        res = as608_basic_init(addr);
        if (res != 0)
        {
            return 1;
        }

        /* input fingerprint */
        res = as608_basic_input_fingerprint(a_callback, &score, &page_number, &status);
        if (res != 0)
        {
            (void)as608_basic_deinit();

            return 1;
        }

        if (status == AS608_STATUS_OK)
        {
            /* output */
            as608_interface_debug_print("input fingerprint success with score %d in page number %d.\n", score, page_number);
        }
        else
        {
            /* print error */
            as608_basic_print_status(status);
        }

        /* basic deinit */
        (void)as608_basic_deinit();

        return 0;
    }
    else if (strcmp("e_basic-verify", type) == 0)
    {
        uint8_t res;
        uint16_t score;
        uint16_t found_page;
        as608_status_t status;

        /* basic init */
        res = as608_basic_init(addr);
        if (res != 0)
        {
            return 1;
        }

        /* verify */
        res = as608_basic_verify(&found_page, &score, &status);
        if (res != 0)
        {
            (void)as608_basic_deinit();

            return 1;
        }

        if (status == AS608_STATUS_OK)
        {
            /* output */
            as608_interface_debug_print("verify success with score %d in found page %d.\n", score, found_page);
        }
        else
        {
            /* print error */
            as608_basic_print_status(status);
        }

        /* basic deinit */
        (void)as608_basic_deinit();

        return 0;
    }
    else if (strcmp("e_basic-fast-verify", type) == 0)
    {
        uint8_t res;
        uint16_t score;
        uint16_t found_page;
        as608_status_t status;

        /* basic init */
        res = as608_basic_init(addr);
        if (res != 0)
        {
            return 1;
        }

        /* high speed verify */
        res = as608_basic_high_speed_verify(&found_page, &score, &status);
        if (res != 0)
        {
            (void)as608_basic_deinit();

            return 1;
        }

        if (status == AS608_STATUS_OK)
        {
            /* output */
            as608_interface_debug_print("verify success with score %d in found page %d.\n", score, found_page);
        }
        else
        {
            /* print error */
            as608_basic_print_status(status);
        }

        /* basic deinit */
        (void)as608_basic_deinit();

        return 0;
    }
    else if (strcmp("e_basic-delete", type) == 0)
    {
        uint8_t res;
        as608_status_t status;

        /* basic init */
        res = as608_basic_init(addr);
        if (res != 0)
        {
            return 1;
        }

        /* delete */
        res = as608_basic_delete_fingerprint(num, &status);
        if (res != 0)
        {
            (void)as608_basic_deinit();

            return 1;
        }

        if (status == AS608_STATUS_OK)
        {
            /* output */
            as608_interface_debug_print("delate page number %d success.\n", num);
        }
        else
        {
            /* print error */
            as608_basic_print_status(status);
        }

        /* basic deinit */
        (void)as608_basic_deinit();

        return 0;
    }
    else if (strcmp("e_basic-empty", type) == 0)
    {
        uint8_t res;
        as608_status_t status;

        /* basic init */
        res = as608_basic_init(addr);
        if (res != 0)
        {
            return 1;
        }

        /* empty */
        res = as608_basic_empty_fingerprint(&status);
        if (res != 0)
        {
            (void)as608_basic_deinit();

            return 1;
        }

        if (status == AS608_STATUS_OK)
        {
            /* output */
            as608_interface_debug_print("empty success.\n");
        }
        else
        {
            /* print error */
            as608_basic_print_status(status);
        }

        /* basic deinit */
        (void)as608_basic_deinit();

        return 0;
    }
    else if (strcmp("e_advance-fingerprint", type) == 0)
    {
        uint8_t res;
        uint16_t score;
        uint16_t page_number;
        as608_status_t status;

        /* advance init */
        res = as608_advance_init(addr);
        if (res != 0)
        {
            return 1;
        }

        /* input fingerprint */
        res = as608_advance_input_fingerprint(a_callback, &score, &page_number, &status);
        if (res != 0)
        {
            (void)as608_advance_deinit();

            return 1;
        }

        if (status == AS608_STATUS_OK)
        {
            /* output */
            as608_interface_debug_print("input fingerprint success with score %d in page number %d.\n", score, page_number);
        }
        else
        {
            /* print error */
            as608_advance_print_status(status);
        }

        /* advance deinit */
        (void)as608_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-verify", type) == 0)
    {
        uint8_t res;
        uint16_t score;
        uint16_t found_page;
        as608_status_t status;

        /* advance init */
        res = as608_advance_init(addr);
        if (res != 0)
        {
            return 1;
        }

        /* verify */
        res = as608_advance_verify(&found_page, &score, &status);
        if (res != 0)
        {
            (void)as608_advance_deinit();

            return 1;
        }

        if (status == AS608_STATUS_OK)
        {
            /* output */
            as608_interface_debug_print("verify success with score %d in found page %d.\n", score, found_page);
        }
        else
        {
            /* print error */
            as608_advance_print_status(status);
        }

        /* advance deinit */
        (void)as608_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-fast-verify", type) == 0)
    {
        uint8_t res;
        uint16_t score;
        uint16_t found_page;
        as608_status_t status;

        /* advance init */
        res = as608_advance_init(addr);
        if (res != 0)
        {
            return 1;
        }

        /* high speed verify */
        res = as608_advance_high_speed_verify(&found_page, &score, &status);
        if (res != 0)
        {
            (void)as608_advance_deinit();

            return 1;
        }

        if (status == AS608_STATUS_OK)
        {
            /* output */
            as608_interface_debug_print("verify success with score %d in found page %d.\n", score, found_page);
        }
        else
        {
            /* print error */
            as608_advance_print_status(status);
        }

        /* advance deinit */
        (void)as608_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-delete", type) == 0)
    {
        uint8_t res;
        as608_status_t status;

        /* advance init */
        res = as608_advance_init(addr);
        if (res != 0)
        {
            return 1;
        }

        /* delete */
        res = as608_advance_delete_fingerprint(num, &status);
        if (res != 0)
        {
            (void)as608_advance_deinit();

            return 1;
        }

        if (status == AS608_STATUS_OK)
        {
            /* output */
            as608_interface_debug_print("delate page number %d success.\n", num);
        }
        else
        {
            /* print error */
            as608_advance_print_status(status);
        }

        /* advance deinit */
        (void)as608_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-empty", type) == 0)
    {
        uint8_t res;
        as608_status_t status;

        /* advance init */
        res = as608_advance_init(addr);
        if (res != 0)
        {
            return 1;
        }

        /* empty */
        res = as608_advance_empty_fingerprint(&status);
        if (res != 0)
        {
            (void)as608_advance_deinit();

            return 1;
        }

        if (status == AS608_STATUS_OK)
        {
            /* output */
            as608_interface_debug_print("empty success.\n");
        }
        else
        {
            /* print error */
            as608_advance_print_status(status);
        }

        /* advance deinit */
        (void)as608_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-write", type) == 0)
    {
        uint8_t res;
        as608_status_t status;

        /* advance init */
        res = as608_advance_init(addr);
        if (res != 0)
        {
            return 1;
        }

        /* write notepad */
        res = as608_advance_write_notepad(num, data, &status);
        if (res != 0)
        {
            (void)as608_advance_deinit();

            return 1;
        }

        if (status == AS608_STATUS_OK)
        {
            /* output */
            as608_interface_debug_print("write success.\n");
        }
        else
        {
            /* print error */
            as608_advance_print_status(status);
        }

        /* advance deinit */
        (void)as608_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-read", type) == 0)
    {
        uint8_t res;
        uint8_t i;
        as608_status_t status;

        /* advance init */
        res = as608_advance_init(addr);
        if (res != 0)
        {
            return 1;
        }

        /* read notepad */
        res = as608_advance_read_notepad(num, data, &status);
        if (res != 0)
        {
            (void)as608_advance_deinit();

            return 1;
        }

        if (status == AS608_STATUS_OK)
        {
            /* output */
            for (i = 0; i < 32; i++)
            {
                as608_interface_debug_print("%02X ", data[i]);
            }
            as608_interface_debug_print("\n");
        }
        else
        {
            /* print error */
            as608_advance_print_status(status);
        }

        /* advance deinit */
        (void)as608_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-random", type) == 0)
    {
        uint8_t res;
        uint32_t random;
        as608_status_t status;

        /* advance init */
        res = as608_advance_init(addr);
        if (res != 0)
        {
            return 1;
        }

        /* random */
        res = as608_advance_random(&random,  &status);
        if (res != 0)
        {
            (void)as608_advance_deinit();

            return 1;
        }

        if (status == AS608_STATUS_OK)
        {
            /* output */
            as608_interface_debug_print("random is 0x%08X.\n", random);
        }
        else
        {
            /* print error */
            as608_advance_print_status(status);
        }

        /* advance deinit */
        (void)as608_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-flash", type) == 0)
    {
        uint8_t res;
        uint16_t i;
        uint16_t output_len;
        as608_status_t status;

        /* advance init */
        res = as608_advance_init(addr);
        if (res != 0)
        {
            return 1;
        }

        /* read flash */
        output_len = 36 * 1024;
        res = as608_advance_flash_information(g_rx_buffer, &output_len, &status);
        if (res != 0)
        {
            (void)as608_advance_deinit();

            return 1;
        }

        if (status == AS608_STATUS_OK)
        {
            /* output */
            for (i = 0; i < output_len; i++)
            {
                as608_interface_debug_print("%02X ", data[i]);
            }
            as608_interface_debug_print("\n");
        }
        else
        {
            /* print error */
            as608_advance_print_status(status);
        }

        /* advance deinit */
        (void)as608_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-param", type) == 0)
    {
        uint8_t res;
        as608_params_t params;
        as608_status_t status;

        /* advance init */
        res = as608_advance_init(addr);
        if (res != 0)
        {
            return 1;
        }

        /* read params */
        res = as608_advance_params(&params, &status);
        if (res != 0)
        {
            (void)as608_advance_deinit();

            return 1;
        }

        if (status == AS608_STATUS_OK)
        {
            /* output */
            as608_interface_debug_print("status is 0x%02X.\n", params.status);
            as608_interface_debug_print("sensor type is 0x%02X.\n", params.sensor_type);
            as608_interface_debug_print("fingerprint size is %d.\n", params.fingerprint_size);
            as608_interface_debug_print("level is 0x%02X.\n", params.level);
            as608_interface_debug_print("address is 0x%08X.\n", params.address);
            as608_interface_debug_print("packet size is %d.\n", params.packet_size);
            as608_interface_debug_print("baud rate is %d.\n", params.n_9600 * 9600);
        }
        else
        {
            /* print error */
            as608_advance_print_status(status);
        }

        /* advance deinit */
        (void)as608_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-enroll", type) == 0)
    {
        uint8_t res;
        uint16_t page_number;
        as608_status_t status;

        /* advance init */
        res = as608_advance_init(addr);
        if (res != 0)
        {
            return 1;
        }

        /* enroll */
        res = as608_advance_enroll(&page_number, &status);
        if (res != 0)
        {
            (void)as608_advance_deinit();

            return 1;
        }

        if (status == AS608_STATUS_OK)
        {
            /* output */
            as608_interface_debug_print("enroll ok and page number is %d.\n", page_number);
        }
        else
        {
            /* print error */
            as608_advance_print_status(status);
        }

        /* advance deinit */
        (void)as608_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-identify", type) == 0)
    {
        uint8_t res;
        uint16_t score;
        uint16_t page_number;
        as608_status_t status;

        /* advance init */
        res = as608_advance_init(addr);
        if (res != 0)
        {
            return 1;
        }

        /* identify */
        res = as608_advance_identify(&page_number, &score, &status);
        if (res != 0)
        {
            (void)as608_advance_deinit();

            return 1;
        }

        if (status == AS608_STATUS_OK)
        {
            /* output */
            as608_interface_debug_print("identify ok and page number is %d, score is %d.\n", page_number, score);
        }
        else
        {
            /* print error */
            as608_advance_print_status(status);
        }

        /* advance deinit */
        (void)as608_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-upload-flash-feature", type) == 0)
    {
        uint8_t res;
        uint16_t output_len;
        as608_status_t status;

        /* advance init */
        res = as608_advance_init(addr);
        if (res != 0)
        {
            return 1;
        }

        /* upload flash feature */
        output_len = 36 * 1024;
        res = as608_advance_upload_flash_feature(num, g_rx_buffer, &output_len, &status);
        if (res != 0)
        {
            (void)as608_advance_deinit();

            return 1;
        }

        if (status == AS608_STATUS_OK)
        {
            FRESULT ret;
            UINT bw;

            /* mount */
            ret = f_mount(&g_fs, "0:", 1);
            if (ret != FR_OK)
            {
                return 1;
            }

            /* open */
            ret = f_open(&g_file, path, FA_WRITE | FA_CREATE_ALWAYS);
            if (ret != FR_OK)
            {
                (void)f_unmount("0:");

                return 1;
            }

            /* write */
            ret = f_write(&g_file, g_rx_buffer, output_len, &bw);
            if (ret != FR_OK)
            {
                (void)f_unmount("0:");

                return 1;
            }

            /* close */
            ret = f_close(&g_file);
            if (ret != FR_OK)
            {
                (void)f_unmount("0:");

                return 1;
            }

            /* unmount */
            (void)f_unmount("0:");

            /* output */
            as608_interface_debug_print("upload flash feature ok.\n");
        }
        else
        {
            /* print error */
            as608_advance_print_status(status);
        }

        /* advance deinit */
        (void)as608_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-upload-image-feature", type) == 0)
    {
        uint8_t res;
        uint16_t output_len;
        as608_status_t status;

        /* advance init */
        res = as608_advance_init(addr);
        if (res != 0)
        {
            return 1;
        }

        /* upload image feature */
        output_len = 36 * 1024;
        res = as608_advance_upload_image_feature(g_rx_buffer, &output_len, &status);
        if (res != 0)
        {
            (void)as608_advance_deinit();

            return 1;
        }

        if (status == AS608_STATUS_OK)
        {
            FRESULT ret;
            UINT bw;

            /* mount */
            ret = f_mount(&g_fs, "0:", 1);
            if (ret != FR_OK)
            {
                return 1;
            }

            /* open */
            ret = f_open(&g_file, path, FA_WRITE | FA_CREATE_ALWAYS);
            if (ret != FR_OK)
            {
                (void)f_unmount("0:");

                return 1;
            }

            /* write */
            ret = f_write(&g_file, g_rx_buffer, output_len, &bw);
            if (ret != FR_OK)
            {
                (void)f_unmount("0:");

                return 1;
            }

            /* close */
            ret = f_close(&g_file);
            if (ret != FR_OK)
            {
                (void)f_unmount("0:");

                return 1;
            }

            /* unmount */
            (void)f_unmount("0:");

            /* output */
            as608_interface_debug_print("upload image feature ok.\n");
        }
        else
        {
            /* print error */
            as608_advance_print_status(status);
        }

        /* advance deinit */
        (void)as608_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-download-flash-feature", type) == 0)
    {
        uint8_t res;
        uint16_t output_len;
        as608_status_t status;
        FRESULT ret;
        UINT bw;

        /* advance init */
        res = as608_advance_init(addr);
        if (res != 0)
        {
            return 1;
        }

        /* mount */
        ret = f_mount(&g_fs, "0:", 1);
        if (ret != FR_OK)
        {
            return 1;
        }

        /* open */
        ret = f_open(&g_file, path, FA_READ);
        if (ret != FR_OK)
        {
            (void)f_unmount("0:");

            return 1;
        }

        /* read */
        output_len = 36 * 1024;
        ret = f_read(&g_file, g_rx_buffer, output_len, &bw);
        if (ret != FR_OK)
        {
            (void)f_unmount("0:");

            return 1;
        }

        /* close */
        ret = f_close(&g_file);
        if (ret != FR_OK)
        {
            (void)f_unmount("0:");

            return 1;
        }

        /* unmount */
        (void)f_unmount("0:");

        /* download flash feature */
        output_len = bw;
        res = as608_advance_download_flash_feature(num, g_rx_buffer, output_len, &status);
        if (res != 0)
        {
            (void)as608_advance_deinit();

            return 1;
        }

        if (status == AS608_STATUS_OK)
        {
            /* output */
            as608_interface_debug_print("download flash feature ok.\n");
        }
        else
        {
            /* print error */
            as608_advance_print_status(status);
        }

        /* advance deinit */
        (void)as608_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-upload-image", type) == 0)
    {
        uint8_t res;
        uint16_t output_len;
        as608_status_t status;

        /* advance init */
        res = as608_advance_init(addr);
        if (res != 0)
        {
            return 1;
        }

        /* upload image */
        output_len = 36 * 1024;
        res = as608_advance_upload_image(g_rx_buffer, &output_len, &status);
        if (res != 0)
        {
            (void)as608_advance_deinit();

            return 1;
        }

        if (status == AS608_STATUS_OK)
        {
            FRESULT ret;
            UINT bw;

            /* mount */
            ret = f_mount(&g_fs, "0:", 1);
            if (ret != FR_OK)
            {
                return 1;
            }

            /* open */
            ret = f_open(&g_file, path, FA_WRITE | FA_CREATE_ALWAYS);
            if (ret != FR_OK)
            {
                (void)f_unmount("0:");

                return 1;
            }

            /* write */
            ret = f_write(&g_file, g_rx_buffer, output_len, &bw);
            if (ret != FR_OK)
            {
                (void)f_unmount("0:");

                return 1;
            }

            /* close */
            ret = f_close(&g_file);
            if (ret != FR_OK)
            {
                (void)f_unmount("0:");

                return 1;
            }

            /* unmount */
            (void)f_unmount("0:");

            /* output */
            as608_interface_debug_print("upload image ok.\n");
        }
        else
        {
            /* print error */
            as608_advance_print_status(status);
        }

        /* advance deinit */
        (void)as608_advance_deinit();

        return 0;
    }
    else if (strcmp("e_advance-download-image", type) == 0)
    {
        uint8_t res;
        uint16_t output_len;
        as608_status_t status;
        FRESULT ret;
        UINT bw;

        /* advance init */
        res = as608_advance_init(addr);
        if (res != 0)
        {
            return 1;
        }

        /* mount */
        ret = f_mount(&g_fs, "0:", 1);
        if (ret != FR_OK)
        {
            return 1;
        }

        /* open */
        ret = f_open(&g_file, path, FA_READ);
        if (ret != FR_OK)
        {
            (void)f_unmount("0:");

            return 1;
        }

        /* read */
        output_len = 36 * 1024;
        ret = f_read(&g_file, g_rx_buffer, output_len, &bw);
        if (ret != FR_OK)
        {
            (void)f_unmount("0:");

            return 1;
        }

        /* close */
        ret = f_close(&g_file);
        if (ret != FR_OK)
        {
            (void)f_unmount("0:");

            return 1;
        }

        /* unmount */
        (void)f_unmount("0:");

        /* download image */
        output_len = bw;
        res = as608_advance_download_image(num, g_rx_buffer, output_len, &status);
        if (res != 0)
        {
            (void)as608_advance_deinit();

            return 1;
        }

        if (status == AS608_STATUS_OK)
        {
            /* output */
            as608_interface_debug_print("download image ok.\n");
        }
        else
        {
            /* print error */
            as608_advance_print_status(status);
        }

        /* advance deinit */
        (void)as608_advance_deinit();

        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        as608_interface_debug_print("Usage:\n");
        as608_interface_debug_print("  as608 (-i | --information)\n");
        as608_interface_debug_print("  as608 (-h | --help)\n");
        as608_interface_debug_print("  as608 (-p | --port)\n");
        as608_interface_debug_print("  as608 (-t reg | --test=reg)\n");
        as608_interface_debug_print("  as608 (-t fingerprint | --test=fingerprint)\n");
        as608_interface_debug_print("  as608 (-e basic-fingerprint | --example=basic-fingerprint) [--addr=<address>]\n");
        as608_interface_debug_print("  as608 (-e basic-verify | --example=basic-verify) [--addr=<address>]\n");
        as608_interface_debug_print("  as608 (-e basic-fast-verify | --example=basic-fast-verify) [--addr=<address>]\n");
        as608_interface_debug_print("  as608 (-e basic-delete | --example=basic-delete) [--addr=<address>] [--num=<page>]\n");
        as608_interface_debug_print("  as608 (-e basic-empty | --example=basic-empty) [--addr=<address>]\n");
        as608_interface_debug_print("  as608 (-e advance-fingerprint | --example=advance-fingerprint) [--addr=<address>]\n");
        as608_interface_debug_print("  as608 (-e advance-verify | --example=advance-verify) [--addr=<address>]\n");
        as608_interface_debug_print("  as608 (-e advance-fast-verify | --example=advance-fast-verify) [--addr=<address>]\n");
        as608_interface_debug_print("  as608 (-e advance-delete | --example=advance-delete) [--addr=<address>] [--num=<page>]\n");
        as608_interface_debug_print("  as608 (-e advance-empty | --example=advance-empty) [--addr=<address>]\n");
        as608_interface_debug_print("  as608 (-e advance-write | --example=advance-write) [--addr=<address>] [--num=<page>] [--data=<data>]\n");
        as608_interface_debug_print("  as608 (-e advance-read | --example=advance-read) [--addr=<address>] [--num=<page>]\n");
        as608_interface_debug_print("  as608 (-e advance-random | --example=advance-random) [--addr=<address>]\n");
        as608_interface_debug_print("  as608 (-e advance-flash | --example=advance-flash) [--addr=<address>]\n");
        as608_interface_debug_print("  as608 (-e advance-param | --example=advance-param) [--addr=<address>]\n");
        as608_interface_debug_print("  as608 (-e advance-enroll | --example=advance-enroll) [--addr=<address>]\n");
        as608_interface_debug_print("  as608 (-e advance-identify | --example=advance-identify) [--addr=<address>]\n");
        as608_interface_debug_print("  as608 (-e advance-upload-flash-feature | --example=advance-upload-flash-feature) [--addr=<address>] [--num=<page>] [--path=<path>]\n");
        as608_interface_debug_print("  as608 (-e advance-upload-image-feature | --example=advance-upload-image-feature) [--addr=<address>] [--path=<path>]\n");
        as608_interface_debug_print("  as608 (-e advance-download-flash-feature | --example=advance-download-flash-feature) [--addr=<address>] [--num=<page>] [--path=<path>]\n");
        as608_interface_debug_print("  as608 (-e advance-upload-image | --example=advance-upload-image) [--addr=<address>] [--path=<path>]\n");
        as608_interface_debug_print("  as608 (-e advance-download-image | --example=advance-download-image) [--addr=<address>] [--path=<path>]\n");
        as608_interface_debug_print("\n");
        as608_interface_debug_print("Options:\n");
        as608_interface_debug_print("      --addr=<address>    Set chip address and it is hexadecimal.([default: 0xFFFFFFFF])\n");
        as608_interface_debug_print("      --data=<data>       Set notepad data and it is hexadecimal with 32 bytes.([default: random])\n");
        as608_interface_debug_print("  -e <basic-fingerprint | basic-verify | basic-fast-verify | basic-delete | basic-empty\n");
        as608_interface_debug_print("     | advance-fingerprint | advance-verify | advance-fast-verify | advance-delete | advance-empty\n");
        as608_interface_debug_print("     | advance-write | advance-read | advance-random | advance-flash | advance-param | advance-enroll\n");
        as608_interface_debug_print("     | advance-identify | advance-upload-flash-feature | advance-upload-image-feature |\n");
        as608_interface_debug_print("     | advance-download-flash-feature | advance-upload-image | advance-download-image>, --example=<basic-fingerprint\n");
        as608_interface_debug_print("     | basic-verify | basic-fast-verify | basic-delete | basic-empty\n");
        as608_interface_debug_print("     | advance-fingerprint | advance-verify | advance-fast-verify | advance-delete | advance-empty\n");
        as608_interface_debug_print("     | advance-write | advance-read | advance-random | advance-flash | advance-param | advance-enroll\n");
        as608_interface_debug_print("     | advance-identify | advance-upload-flash-feature | advance-upload-image-feature |\n");
        as608_interface_debug_print("     | advance-download-flash-feature | advance-upload-image | advance-download-image>\n");
        as608_interface_debug_print("                          Run the driver example.\n");
        as608_interface_debug_print("  -h, --help              Show the help.\n");
        as608_interface_debug_print("  -i, --information       Show the chip information.\n");
        as608_interface_debug_print("      --num=<page>        Set flash page.([default: 0])\n");
        as608_interface_debug_print("  -p, --port              Display the pin connections of the current board.\n");
        as608_interface_debug_print("      --path=<path>       Set file path.([default: \"tmp.data\"])\n");
        as608_interface_debug_print("  -t <reg | fingerprint>, --test=<reg | fingerprint>\n");
        as608_interface_debug_print("                          Run the driver test.\n");

        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        as608_info_t info;

        /* print as608 info */
        as608_info(&info);
        as608_interface_debug_print("as608: chip is %s.\n", info.chip_name);
        as608_interface_debug_print("as608: manufacturer is %s.\n", info.manufacturer_name);
        as608_interface_debug_print("as608: interface is %s.\n", info.interface);
        as608_interface_debug_print("as608: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        as608_interface_debug_print("as608: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        as608_interface_debug_print("as608: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        as608_interface_debug_print("as608: max current is %0.2fmA.\n", info.max_current_ma);
        as608_interface_debug_print("as608: max temperature is %0.1fC.\n", info.temperature_max);
        as608_interface_debug_print("as608: min temperature is %0.1fC.\n", info.temperature_min);

        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        as608_interface_debug_print("as608: TX connected to GPIOA PIN3.\n");
        as608_interface_debug_print("as608: RX connected to GPIOA PIN2.\n");

        return 0;
    }
    else
    {
        return 5;
    }
}

/**
 * @brief main function
 * @note  none
 */
int main(void)
{
    uint8_t res;

    /* stm32f407 clock init and hal init */
    clock_init();

    /* delay init */
    delay_init();

    /* uart init */
    uart_init(115200);

    /* shell init && register as608 function */
    shell_init();
    shell_register("as608", as608);
    uart_print("as608: welcome to libdriver as608.\n");

    while (1)
    {
        /* read uart */
        g_len = uart_read(g_buf, 256);
        if (g_len != 0)
        {
            /* run shell */
            res = shell_parse((char *)g_buf, g_len);
            if (res == 0)
            {
                /* run success */
            }
            else if (res == 1)
            {
                uart_print("as608: run failed.\n");
            }
            else if (res == 2)
            {
                uart_print("as608: unknown command.\n");
            }
            else if (res == 3)
            {
                uart_print("as608: length is too long.\n");
            }
            else if (res == 4)
            {
                uart_print("as608: pretreat failed.\n");
            }
            else if (res == 5)
            {
                uart_print("as608: param is invalid.\n");
            }
            else
            {
                uart_print("as608: unknown status code.\n");
            }
            uart_flush();
        }
        delay_ms(100);
    }
}
