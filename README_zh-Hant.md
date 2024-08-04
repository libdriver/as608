[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver AS608

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/as608/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

AS608是一個指紋掃描儀和讀取器模塊，但它的功能遠不止指紋注册、圖像處理、列印匹配等。 它處理數據並通過串口發送處理後的數據到微控制器。 該設備使用DSP晶片進行圖像處理渲染、特徵查找、計算和蒐索。 它有一個內寘的閃存，可以存儲指紋的數據，它最多可以存儲162個指紋。 它可以與控制器或具有TTL串列的任何其他系統通信，提供數据包拍照輸出，檢測列印、雜湊和蒐索等功能。 該設備具有紅色和綠色LED指示燈，可用於錯誤和正確的列印。

LibDriver AS608是LibDriver推出的AS608全功能驅動，該驅動提供指紋錄入、指紋驗證等功能並且它符合MISRA標準。

### 目錄

  - [說明](#說明)
  - [安裝](#安裝)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example advance](#example-advance)
  - [文檔](#文檔)
  - [貢獻](#貢獻)
  - [版權](#版權)
  - [聯繫我們](#聯繫我們)

### 說明

/src目錄包含了LibDriver AS608的源文件。

/interface目錄包含了LibDriver AS608與平台無關的UART模板。

/test目錄包含了LibDriver AS608驅動測試程序，該程序可以簡單的測試芯片必要功能。

/example目錄包含了LibDriver AS608編程範例。

/doc目錄包含了LibDriver AS608離線文檔。

/datasheet目錄包含了AS608數據手冊。

/project目錄包含了常用Linux與單片機開發板的工程樣例。所有工程均採用shell腳本作為調試方法，詳細內容可參考每個工程裡面的README.md。

/misra目錄包含了LibDriver MISRA程式碼掃描結果。

### 安裝

參考/interface目錄下與平台無關的UART模板，完成指定平台的UART驅動。

將/src目錄，您使用平臺的介面驅動和您開發的驅動加入工程，如果您想要使用默認的範例驅動，可以將/example目錄加入您的工程。

### 使用

您可以參考/example目錄下的程式設計範例完成適合您的驅動，如果您想要使用默認的程式設計範例，以下是它們的使用方法。

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

### 文檔

在線文檔: [https://www.libdriver.com/docs/as608/index.html](https://www.libdriver.com/docs/as608/index.html)。

離線文檔: /doc/html/index.html。

### 貢獻

請參攷CONTRIBUTING.md。

### 版權

版權 (c) 2015 - 現在 LibDriver 版權所有

MIT 許可證（MIT）

特此免費授予任何獲得本軟件副本和相關文檔文件（下稱“軟件”）的人不受限制地處置該軟件的權利，包括不受限制地使用、複製、修改、合併、發布、分發、轉授許可和/或出售該軟件副本，以及再授權被配發了本軟件的人如上的權利，須在下列條件下：

上述版權聲明和本許可聲明應包含在該軟件的所有副本或實質成分中。

本軟件是“如此”提供的，沒有任何形式的明示或暗示的保證，包括但不限於對適銷性、特定用途的適用性和不侵權的保證。在任何情況下，作者或版權持有人都不對任何索賠、損害或其他責任負責，無論這些追責來自合同、侵權或其它行為中，還是產生於、源於或有關於本軟件以及本軟件的使用或其它處置。

### 聯繫我們

請聯繫lishifenging@outlook.com。