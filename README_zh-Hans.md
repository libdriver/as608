[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver AS608

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/as608/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

AS608是一个指纹扫描仪和读取器模块，但它的功能远不止指纹注册、图像处理、打印匹配等。它处理数据并通过串口发送处理后的数据到微控制器。该设备使用DSP芯片进行图像处理渲染、特征查找、计算和搜索。它有一个内置的闪存，可以存储指纹的数据，它最多可以存储162个指纹。它可以与控制器或具有TTL串行的任何其他系统通信，提供数据包拍照输出，检测打印、哈希和搜索等功能。该设备具有红色和绿色LED指示灯，可用于错误和正确的打印。

LibDriver AS608是LibDriver推出的AS608全功能驱动，该驱动提供指纹录入、指纹验证等功能并且它符合MISRA标准。

### 目录

  - [说明](#说明)
  - [安装](#安装)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example advance](#example-advance)
  - [文档](#文档)
  - [贡献](#贡献)
  - [版权](#版权)
  - [联系我们](#联系我们)

### 说明

/src目录包含了LibDriver AS608的源文件。

/interface目录包含了LibDriver AS608与平台无关的UART模板。

/test目录包含了LibDriver AS608驱动测试程序，该程序可以简单的测试芯片必要功能。

/example目录包含了LibDriver AS608编程范例。

/doc目录包含了LibDriver AS608离线文档。

/datasheet目录包含了AS608数据手册。

/project目录包含了常用Linux与单片机开发板的工程样例。所有工程均采用shell脚本作为调试方法，详细内容可参考每个工程里面的README.md。

/misra目录包含了LibDriver MISRA代码扫描结果。

### 安装

参考/interface目录下与平台无关的UART模板，完成指定平台的UART驱动。

将/src目录，您使用平台的接口驱动和您开发的驱动加入工程，如果您想要使用默认的范例驱动，可以将/example目录加入您的工程。

### 使用

您可以参考/example目录下的编程范例完成适合您的驱动，如果您想要使用默认的编程范例，以下是它们的使用方法。

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

### 文档

在线文档: [https://www.libdriver.com/docs/as608/index.html](https://www.libdriver.com/docs/as608/index.html)。

离线文档: /doc/html/index.html。

### 贡献

请参考CONTRIBUTING.md。

### 版权

版权 (c) 2015 - 现在 LibDriver 版权所有

MIT 许可证（MIT）

特此免费授予任何获得本软件副本和相关文档文件（下称“软件”）的人不受限制地处置该软件的权利，包括不受限制地使用、复制、修改、合并、发布、分发、转授许可和/或出售该软件副本，以及再授权被配发了本软件的人如上的权利，须在下列条件下：

上述版权声明和本许可声明应包含在该软件的所有副本或实质成分中。

本软件是“如此”提供的，没有任何形式的明示或暗示的保证，包括但不限于对适销性、特定用途的适用性和不侵权的保证。在任何情况下，作者或版权持有人都不对任何索赔、损害或其他责任负责，无论这些追责来自合同、侵权或其它行为中，还是产生于、源于或有关于本软件以及本软件的使用或其它处置。

### 联系我们

请联系lishifenging@outlook.com。