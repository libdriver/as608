[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver AS608

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/as608/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

AS608 は指紋スキャナーおよびリーダー モジュールですが、指紋の登録、画像処理、印刷照合など、それ以上の機能を備えています。 データを処理し、処理されたデータをシリアル経由でマイクロコントローラーに送信します。 このデバイスは、画像のレンダリング、特徴の検出、計算、検索を行う DSP チップを使用します。 指紋データを保存し、新規登録するフラッシュ メモリが内蔵されており、最大 300 個の指紋を保存できます。 TTL シリアルを使用してコントローラまたはその他のシステムと接続し、データのパケットを送信して写真を撮り、印刷を検出し、ハッシュし、検索します。 このデバイスには、間違ったプリントと正しいプリントを示す赤と緑の LED ライトが付いています。

LibDriver AS608 は、LibDriver によって起動された AS608 のフル機能ドライバーであり、指紋登録、指紋照合などの機能を提供します。 LibDriver は MISRA に準拠しています。

### 目次

  - [説明](#説明)
  - [インストール](#インストール)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example advance](#example-advance)
  - [ドキュメント](#ドキュメント)
  - [貢献](#貢献)
  - [著作権](#著作権)
  - [連絡して](#連絡して)

### 説明

/ srcディレクトリには、LibDriver AS608 のソースファイルが含まれています。

/ interfaceディレクトリには、LibDriver AS608 用のプラットフォームに依存しないUARTバステンプレートが含まれています。

/ testディレクトリには、チップの必要な機能を簡単にテストできるLibDriver AS608 ドライバーテストプログラムが含まれています。

/ exampleディレクトリには、LibDriver AS608 プログラミング例が含まれています。

/ docディレクトリには、LibDriver AS608 オフラインドキュメントが含まれています。

/ datasheetディレクトリには、AS608 データシートが含まれています。

/ projectディレクトリには、一般的に使用されるLinuxおよびマイクロコントローラー開発ボードのプロジェクトサンプルが含まれています。 すべてのプロジェクトは、デバッグ方法としてシェルスクリプトを使用しています。詳細については、各プロジェクトのREADME.mdを参照してください。

/ misraはLibDriver misraコードスキャン結果を含む。

### インストール

/ interfaceディレクトリにあるプラットフォームに依存しないUARTバステンプレートを参照して、指定したプラットフォームのUARTバスドライバを完成させます。

/src ディレクトリ、プラットフォームのインターフェイス ドライバー、および独自のドライバーをプロジェクトに追加します。デフォルトのサンプル ドライバーを使用する場合は、/example ディレクトリをプロジェクトに追加します。

### 使用

/example ディレクトリ内のサンプルを参照して、独自のドライバーを完成させることができます。 デフォルトのプログラミング例を使用したい場合の使用方法は次のとおりです。

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

```c
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

### ドキュメント

オンラインドキュメント: [https://www.libdriver.com/docs/as608/index.html](https://www.libdriver.com/docs/as608/index.html)。

オフラインドキュメント: /doc/html/index.html。

### 貢献

CONTRIBUTING.mdを参照してください。

### 著作権

著作権（c）2015-今 LibDriver 全著作権所有

MITライセンス（MIT）

このソフトウェアおよび関連するドキュメントファイル（「ソフトウェア」）のコピーを取得した人は、無制限の使用、複製、変更、組み込み、公開、配布、サブライセンスを含む、ソフトウェアを処分する権利を制限なく付与されます。ソフトウェアのライセンスおよび/またはコピーの販売、および上記のようにソフトウェアが配布された人の権利のサブライセンスは、次の条件に従うものとします。

上記の著作権表示およびこの許可通知は、このソフトウェアのすべてのコピーまたは実体に含まれるものとします。

このソフトウェアは「現状有姿」で提供され、商品性、特定目的への適合性、および非侵害の保証を含むがこれらに限定されない、明示または黙示を問わず、いかなる種類の保証もありません。 いかなる場合も、作者または著作権所有者は、契約、不法行為、またはその他の方法で、本ソフトウェアおよび本ソフトウェアの使用またはその他の廃棄に起因または関連して、請求、損害、またはその他の責任を負わないものとします。

### 連絡して

お問い合わせくださいlishifenging@outlook.com。