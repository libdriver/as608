[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver AS608

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/as608/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

AS608은 지문 스캐너 및 판독기 모듈이지만 지문 등록, 이미지 처리, 인쇄 일치 등과 같은 기능 이상의 기능을 수행합니다. 데이터를 처리하고 처리된 데이터를 직렬을 통해 마이크로 컨트롤러로 보냅니다. 이 장치는 이미지 렌더링, 특징 찾기, 계산 및 검색을 수행하는 DSP 칩을 사용합니다. 지문 데이터를 저장하고 새로 등록할 수 있는 플래시 메모리가 내장되어 있으며 최대 300개의 지문을 저장할 수 있습니다. TTL 직렬을 사용하여 컨트롤러 또는 기타 시스템과 인터페이스하고 데이터 패킷을 전송하여 사진을 찍고 인쇄, 해시 및 검색을 감지합니다. 장치에는 잘못된 인쇄와 올바른 인쇄를 나타내는 빨간색과 녹색 LED 표시등이 있습니다.

LibDriver AS608은 LibDriver가 출시한 AS608의 전체 기능 드라이버입니다. 지문 등록, 지문 확인 및 기타 기능을 제공합니다. LibDriver는 MISRA를 준수합니다.

### 콘텐츠

  - [설명](#설명)
  - [설치](#설치)
  - [사용](#사용)
    - [example basic](#example-basic)
    - [example advance](#example-advance)
  - [문서](#문서)
  - [기고](#기고)
  - [저작권](#저작권)
  - [문의하기](#문의하기)

### 설명

/src 디렉토리에는 LibDriver AS608의 소스 파일이 포함되어 있습니다.

/interface 디렉토리에는 LibDriver AS608용 플랫폼 독립적인 UART버스 템플릿이 포함되어 있습니다.

/test 디렉토리에는 LibDriver AS608드라이버 테스트 프로그램이 포함되어 있어 칩의 필요한 기능을 간단히 테스트할 수 있습니다.

/example 디렉토리에는 LibDriver AS608프로그래밍 예제가 포함되어 있습니다.

/doc 디렉토리에는 LibDriver AS608오프라인 문서가 포함되어 있습니다.

/datasheet 디렉토리에는 AS608데이터시트가 있습니다.

/project 디렉토리에는 일반적으로 사용되는 Linux 및 마이크로컨트롤러 개발 보드의 프로젝트 샘플이 포함되어 있습니다. 모든 프로젝트는 디버깅 방법으로 셸 스크립트를 사용하며, 자세한 내용은 각 프로젝트의 README.md를 참조하십시오.

/misra 에는 LibDriver misra 코드 검색 결과가 포함됩니다.

### 설치

/interface 디렉토리에서 플랫폼 독립적인UART버스 템플릿을 참조하여 지정된 플랫폼에 대한 UART버스 드라이버를 완성하십시오.

/src 디렉터리, 플랫폼용 인터페이스 드라이버 및 자체 드라이버를 프로젝트에 추가합니다. 기본 예제 드라이버를 사용하려면 /example 디렉터리를 프로젝트에 추가합니다.

### 사용

/example 디렉터리의 예제를 참조하여 자신만의 드라이버를 완성할 수 있습니다. 기본 프로그래밍 예제를 사용하려는 경우 사용 방법은 다음과 같습니다.

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

### 문서

온라인 문서: [https://www.libdriver.com/docs/as608/index.html](https://www.libdriver.com/docs/as608/index.html).

오프라인 문서: /doc/html/index.html.

### 기고

CONTRIBUTING.md 를 참조하십시오.

### 저작권

저작권 (c) 2015 - 지금 LibDriver 판권 소유

MIT 라이선스(MIT)

이 소프트웨어 및 관련 문서 파일("소프트웨어")의 사본을 얻은 모든 사람은 이에 따라 무제한 사용, 복제, 수정, 통합, 출판, 배포, 2차 라이선스를 포함하여 소프트웨어를 처분할 수 있는 권리가 부여됩니다. 소프트웨어의 사본에 대한 라이선스 및/또는 판매, 그리고 소프트웨어가 위와 같이 배포된 사람의 권리에 대한 2차 라이선스는 다음 조건에 따릅니다.

위의 저작권 표시 및 이 허가 표시는 이 소프트웨어의 모든 사본 또는 내용에 포함됩니다.

이 소프트웨어는 상품성, 특정 목적에의 적합성 및 비침해에 대한 보증을 포함하되 이에 국한되지 않는 어떠한 종류의 명시적 또는 묵시적 보증 없이 "있는 그대로" 제공됩니다. 어떤 경우에도 저자 또는 저작권 소유자는 계약, 불법 행위 또는 기타 방식에 관계없이 소프트웨어 및 기타 소프트웨어 사용으로 인해 발생하거나 이와 관련하여 발생하는 청구, 손해 또는 기타 책임에 대해 책임을 지지 않습니다.

### 문의하기

연락주세요lishifenging@outlook.com.