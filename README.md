[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver AS608

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/as608/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

AS608 is a fingerprint scanner and reader module but does more than that like fingerprint enrollment, image processing, print matching and many more. It processes the data and sends processed data to the microcontroller through serial. The device uses a DSP chip that does image rendering, feature finding, calculation and searching. It has an in-built flash memory that stores the data of fingerprint and enrolls new – up to 300 fingerprints can be stored. It interfaces with the controller or any other system with TTL serial and sends packets of data to take photos, detects print, hash, and search. The device has a red and green LED light indication for the wrong and right prints. 

LibDriver AS608 is the full function driver of AS608 launched by LibDriver.It provides fingerprint registration, fingerprint verification and other functions. LibDriver is MISRA compliant.

### Table of Contents

  - [Instruction](#Instruction)
  - [Install](#Install)
  - [Usage](#Usage)
    - [example basic](#example-basic)
    - [example advance](#example-advance)
  - [Document](#Document)
  - [Contributing](#Contributing)
  - [License](#License)
  - [Contact Us](#Contact-Us)

### Instruction

/src includes LibDriver AS608 source files.

/interface includes LibDriver AS608 UART platform independent template.

/test includes LibDriver AS608 driver test code and this code can test the chip necessary function simply.

/example includes LibDriver AS608 sample code.

/doc includes LibDriver AS608 offline document.

/datasheet includes AS608 datasheet.

/project includes the common Linux and MCU development board sample code. All projects use the shell script to debug the driver and the detail instruction can be found in each project's README.md.

/misra includes the LibDriver MISRA code scanning results.

### Install

Reference /interface UART platform independent template and finish your platform UART driver.

Add the /src directory, the interface driver for your platform, and your own drivers to your project, if you want to use the default example drivers, add the /example directory to your project.

### Usage

You can refer to the examples in the /example directory to complete your own driver. If you want to use the default programming examples, here's how to use them.

#### example basic

```C
#include "driver_as608_basic.h"

uint8_t res;
uint16_t score;
uint16_t page_number;
uint16_t found_page;
as608_status_t status;
uint32_t num = 0;
uint32_t addr = 0xFFFFFFFF;

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

...

/* basic init */
res = as608_basic_init(addr);
if (res != 0)
{
    return 1;
}

...
    
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

...
    
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

...

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

...
    
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

...
    
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

...
    
/* basic deinit */
(void)as608_basic_deinit();

return 0;
```

#### example advance

```C
#include "driver_as608_advance.h"

uint8_t res;
uint16_t score;
uint16_t page_number;
uint16_t found_page;
uint16_t i;
uint16_t output_len;
as608_status_t status;
as608_params_t params;
uint32_t num = 0;
uint32_t random;
uint32_t addr = 0xFFFFFFFF;
uint8_t data[32] = {0};
uint8_t g_rx_buffer[36 * 1024];

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

...
    
/* advance init */
res = as608_advance_init(addr);
if (res != 0)
{
    return 1;
}

...
    
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

...
    
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

...
    
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

...
    
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

...
    
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

...
    
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

...
    
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

...
    
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

...
    
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

...
    
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

...
    
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

...
    
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

...
    
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
    ...
}
else
{
    /* print error */
    as608_advance_print_status(status);
}

...
    
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
    ...
}
else
{
    /* print error */
    as608_advance_print_status(status);
}

...
    
/* download flash feature */
output_len = 768;
res = as608_advance_download_flash_feature(num, g_rx_buffer, output_len, &status);
if (res != 0)
{
    (void)as608_advance_deinit();

    return 1;
}

if (status == AS608_STATUS_OK)
{
    ...
}
else
{
    /* print error */
    as608_advance_print_status(status);
}

...
    
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
    ...
}
else
{
    /* print error */
    as608_advance_print_status(status);
}

...
    
/* download image */
output_len = 32768;
res = as608_advance_download_image(num, g_rx_buffer, output_len, &status);
if (res != 0)
{
    (void)as608_advance_deinit();

    return 1;
}

if (status == AS608_STATUS_OK)
{
    ...
}
else
{
    /* print error */
    as608_advance_print_status(status);
}

...
    
/* advance deinit */
(void)as608_advance_deinit();

return 0;
```

### Document

Online documents: [https://www.libdriver.com/docs/as608/index.html](https://www.libdriver.com/docs/as608/index.html).

Offline documents: /doc/html/index.html.

### Contributing

Please refer to CONTRIBUTING.md.

### License

Copyright (c) 2015 - present LibDriver All rights reserved



The MIT License (MIT) 



Permission is hereby granted, free of charge, to any person obtaining a copy

of this software and associated documentation files (the "Software"), to deal

in the Software without restriction, including without limitation the rights

to use, copy, modify, merge, publish, distribute, sublicense, and/or sell

copies of the Software, and to permit persons to whom the Software is

furnished to do so, subject to the following conditions: 



The above copyright notice and this permission notice shall be included in all

copies or substantial portions of the Software. 



THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR

IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,

FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE

AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER

LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,

OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE

SOFTWARE. 

### Contact Us

Please send an e-mail to lishifenging@outlook.com.