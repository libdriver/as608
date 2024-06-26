### 1. Board

#### 1.1 Board Info

Board Name: Raspberry Pi 4B.

UART Pin: TX/RX GPIO14/GPIO15.

### 2. Install

#### 2.1 Dependencies

Install the necessary dependencies.

```shell
sudo apt-get install libgpiod-dev pkg-config cmake -y
```
#### 2.2 Configuration

Enable serial port.

Disable serial console.

#### 2.3 Makefile

Build the project.

```shell
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

#### 2.4 CMake

Build the project.

```shell
mkdir build && cd build 
cmake .. 
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

Test the project and this is optional.

```shell
make test
```

Find the compiled library in CMake. 

```cmake
find_package(as608 REQUIRED)
```

#### 2.5 Command Problem

Each command will sent a sleep command to the chip, but raspberry can't run wake up command, so you should power down and power on SYN6288 every time before your any new command.

### 3. AS608

#### 3.1 Command Instruction

1. Show as608 chip and driver information.

   ```shell
   as608 (-i | --information)
   ```

2. Show as608 help.

   ```shell
   as608 (-h | --help)
   ```

3. Show as608 pin connections of the current board.

   ```shell
   as608 (-p | --port)
   ```

4. Run as608 register test.

   ```shell
   as608 (-t reg | --test=reg)
   ```

5. Run as608 fingerprint test.

   ```shell
   as608 (-t fingerprint | --test=fingerprint)
   ```

6. Run as608 basic fingerprint function, address is the chip address and it is hexadecimal.

   ```shell
   as608 (-e basic-fingerprint | --example=basic-fingerprint) [--addr=<address>]
   ```

7. Run as608 basic verify function,  address is the chip address and it is hexadecimal.

   ```shell
   as608 (-e basic-verify | --example=basic-verify) [--addr=<address>]
   ```

8. Run as608 basic fast verify function, address is the chip address and it is hexadecimal.

   ```shell
   as608 (-e basic-fast-verify | --example=basic-fast-verify) [--addr=<address>]
   ```

9. Run as608 basic delete function, address is the chip address and it is hexadecimal, page is the page number.

   ```shell
   as608 (-e basic-delete | --example=basic-delete) [--addr=<address>] [--num=<page>]
   ```

10. Run as608 basic empty function, address is the chip address and it is hexadecimal.

    ```shell
    as608 (-e basic-empty | --example=basic-empty) [--addr=<address>]
    ```

11. Run as608 advance fingerprint function, address is the chip address and it is hexadecimal.

    ```shell
    as608 (-e advance-fingerprint | --example=advance-fingerprint) [--addr=<address>]
    ```

12. Run as608 advance verify function,  address is the chip address and it is hexadecimal.

    ```shell
    as608 (-e advance-verify | --example=advance-verify) [--addr=<address>]
    ```

13. Run as608 advance fast verify function, address is the chip address and it is hexadecimal.

    ```shell
    as608 (-e advance-fast-verify | --example=advance-fast-verify) [--addr=<address>]
    ```

14. Run as608 advance delete function, address is the chip address and it is hexadecimal, page is the page number.

    ```shell
    as608 (-e advance-delete | --example=advance-delete) [--addr=<address>] [--num=<page>]
    ```

15. Run as608 advance empty function, address is the chip address and it is hexadecimal.

    ```shell
    as608 (-e advance-empty | --example=advance-empty) [--addr=<address>]
    ```

16. Run as608 advance write function, address is the chip address and it is hexadecimal, page is the page number, data is the set data and it is hexadecimal with the length of 32 bytes.

    ```shell
    as608 (-e advance-write | --example=advance-write) [--addr=<address>] [--num=<page>] [--data=<data>]
    ```

17. Run as608 advance read function, address is the chip address and it is hexadecimal, page is the page number.

    ```shell
    as608 (-e advance-read | --example=advance-read) [--addr=<address>] [--num=<page>]
    ```

18. Run as608 advance random function, address is the chip address and it is hexadecimal.

    ```shell
    as608 (-e advance-random | --example=advance-random) [--addr=<address>]
    ```

19. Run as608 advance get flash information function, address is the chip address and it is hexadecimal.

    ```shell
    as608 (-e advance-flash | --example=advance-flash) [--addr=<address>]
    ```

20. Run as608 advance param function, address is the chip address and it is hexadecimal.

    ```shell
    as608 (-e advance-param | --example=advance-param) [--addr=<address>]
    ```

21. Run as608 advance enroll function, address is the chip address and it is hexadecimal.

    ```shell
    as608 (-e advance-enroll | --example=advance-enroll) [--addr=<address>]
    ```

22. Run as608 advance identify function, address is the chip address and it is hexadecimal.

    ```shell
     as608 (-e advance-enroll | --example=advance-enroll) [--addr=<address>]
    ```

23. Run as608 advance upload flash feature function, address is the chip address and it is hexadecimal, page is the page number, path is the file path.

    ```shell
    as608 (-e advance-upload-flash-feature | --example=advance-upload-flash-feature) [--addr=<address>] [--num=<page>] [--path=<path>]
    ```

24. Run as608 advance upload image feature function, address is the chip address and it is hexadecimal,  path is the file path.

    ```shell
    as608 (-e advance-upload-image-feature | --example=advance-upload-image-feature) [--addr=<address>] [--path=<path>]
    ```

25. Run as608 advance download flash feature function, address is the chip address and it is hexadecimal, page is the page number, path is the file path.

    ```shell
    as608 (-e advance-download-flash-feature | --example=advance-download-flash-feature) [--addr=<address>] [--num=<page>] [--path=<path>]
    ```

26. Run as608 advance upload image function, address is the chip address and it is hexadecimal, path is the file path.

    ```shell
    as608 (-e advance-upload-image | --example=advance-upload-image) [--addr=<address>] [--path=<path>]
    ```

27. Run as608 advance download image function, address is the chip address and it is hexadecimal, path is the file path.

    ```shell
    as608 (-e advance-download-image | --example=advance-download-image) [--addr=<address>] [--path=<path>]
    ```

#### 3.2 Command Example

```shell
./as608 -i

as608: chip is Synochip AS608.
as608: manufacturer is Synochip.
as608: interface is UART.
as608: driver version is 1.0.
as608: min supply voltage is 3.0V.
as608: max supply voltage is 3.6V.
as608: max current is 60.00mA.
as608: max temperature is 85.0C.
as608: min temperature is -40.0C.
```

```shell
./as608 -p

as608: TX connected to GPIO15(BCM).
as608: RX connected to GPIO14(BCM).
```

```shell
./as608 -t reg

as608: chip is Synochip AS608.
as608: manufacturer is Synochip.
as608: interface is UART.
as608: driver version is 1.0.
as608: min supply voltage is 3.0V.
as608: max supply voltage is 3.6V.
as608: max current is 60.00mA.
as608: max temperature is 85.0C.
as608: min temperature is -40.0C.
as608: start register test.
as608: as608_get_last_status test.
as608: last status is 0x00.
as608: as608_print_status test.
as608: ok.
as608: as608_set_baud_rate test.
as608: set baud rate 57600.
as608: as608_set_level test.
as608: set level 3.
as608: as608_set_packet_size test.
as608: set packet size 128bytes.
as608: as608_get_params test.
as608: status is 0x00.
as608: sensor type is 0x00.
as608: fingerprint size is 300.
as608: level is 0x03.
as608: address is 0xFFFFFFFF.
as608: packet size is 0x02.
as608: baud rate is 57600.
as608: as608_set_password test.
as608: set password 0x00000000.
as608: as608_verify_password test.
as608: verify password ok.
as608: as608_get_random test.
as608: random is 0x82C1348D.
as608: as608_set_chip_address test.
as608: set chip address 0xFFFFFFFF.
as608: as608_get_flash_information test.
as608: flash information is
0x00 0x00 0x00 0x00 0x01 0x2C 0x00 0x03 0xFF 0xFF 0xFF 0xFF 0x00 0x02 0x00 0x06 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x20 0x20 0x44 0x65 0x6D 0x6F 0x20 0x20 0x56 0x65 0x72 0x20 0x31 0x33 0x37 0x34 0x53 0x59 0x4E 0x4F 0x44 0x41 0x54 0x41 0x47 0x43 0x30 0x33 0x32 0x38 0x20 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x62 0x3D 0x12 0x34 0x1E 0x30 0x74 0x5A 0x1F 0x34 0x74 0x5E 0x1E 0x10 0x74 0x58 0x1F 0x14 0x74 0x5C 0x1A 0x30 0x74 0x1A 0x1B 0x34 0x74 0x1E 0x1A 0x10 0x74 0x18 0x1B 0x14 0x74 0x1C 0x1E 0xB0 0x74 0x7A 0x1F 0xB4 0x74 0x7E 0x1E 0x90 0x74 0x78 0x1F 0x94 0x74 0x7C 0x1A 0xB0 0x74 0x3A 0x1B 0xB4 0x74 0x3E 0x1A 0x90 0x74 0x38 0x1B 0x94 0x74 0x3C 0x1C 0x30 0x74 0x5B 0x1D 0x34 0x74 0x5F 0x1C 0x10 0x74 0x59 0x1D 0x14 0x74 0x5D 0x18 0x30 0x74 0x1B 0x19 0x34 0x74 0x1F 0x18 0x10 0x74 0x19 0x19 0x14 0x74 0x1D 0x1C 0xB0 0x74 0x7B 0x1D 0xB4 0x74 0x7F 0x1C 0x90 0x74 0x79 0x1D 0x94 0x74 0x7D 0x18 0xB0 0x74 0x3B 0x19 0xB4 0x74 0x3F 0x18 0x90 0x74 0x39 0x19 0x94 0x74 0x3D 0x1E 0x31 0x74 0xCA 0x1F 0x35 0x74 0xCE 0x1E 0x11 0x74 0xC8 0x1F 0x15 0x74 0xCC 0x1A 0x31 0x74 0x8A 0x1B 0x35 0x74 0x8E 0x1A 0x11 0x74 0x88 0x1B 0x15 0x74 0x8C 0x1E 0xB1 0x74 0xEA 0x1F 0xB5 0x74 0xEE 0x1E 0x91 0x74 0xE8 0x1F 0x95 0x74 0xEC 0x1A 0xB1 0x74 0xAA 0x1B 0xB5 0x74 0xAE 0x1A 0x91 0x74 0xA8 0x1B 0x95 0x74 0xAC 0x1C 0x31 0x74 0xCB 0x1D 0x35 0x74 0xCF 0x1C 0x11 0x74 0xC9 0x1D 0x15 0x74 0xCD 0x18 0x31 0x74 0x8B 0x19 0x35 0x74 0x8F 0x18 0x11 0x74 0x89 0x19 0x15 0x74 0x8D 0x1C 0xB1 0x74 0xEB 0x1D 0xB5 0x74 0xEF 0x1C 0x91 0x74 0xE9 0x1D 0x95 0x74 0xED 0x18 0xB1 0x74 0xAB 0x19 0xB5 0x74 0xAF 0x18 0x91 0x74 0xA9 0x19 0x95 0x74 0xAD 0x1E 0x30 0x74 0xDA 0x1F 0x34 0x74 0xDE 0x1E 0x10 0x74 0xD8 0x1F 0x14 0x74 0xDC 0x1A 0x30 0x74 0x9A 0x1B 0x34 0x74 0x9E 0x1A 0x10 0x74 0x98 0x1B 0x14 0x74 0x9C 0x1E 0xB0 0x74 0xFA 0x1F 0xB4 0x74 0xFE 0x1E 0x90 0x74 0xF8 0x1F 0x94 0x74 0xFC 0x1A 0xB0 0x74 0xBA 0x1B 0xB4 0x74 0xBE 0x1A 0x90 0x74 0xB8 0x1B 0x94 0x74 0xBC 0x1C 0x30 0x74 0xDB 0x1D 0x34 0x74 0xDF 0x1C 0x10 0x74 0xD9 0x1D 0x14 0x74 0xDD 0x18 0x30 0x74 0x9B 0x19 0x34 0x74 0x9F 0x18 0x10 0x74 0x99 0x19 0x14 0x74 0x9D 0x1C 0xB0 0x74 0xFB 0x1D 0xB4 0x74 0xFF 0x1C 0x90 0x74 0xF9 0x1D 0x94 0x74 0xFD 0x18 0xB0 0x74 0xBB 0x19 0xB4 0x74 0xBF 0x18 0x90 0x74 0xB9 0x19 0x94 0x74 0xBD 
as608: as608_set_port test.
as608: enable port.
as608: as608_write_notepad/as608_read_notepad test test.
as608: random.
0xA3 0x97 0xA2 0x55 0x53 0xBE 0xF1 0xFC 0xF9 0x79 0x6B 0x52 0x14 0x13 0xE9 0xE2 0x2D 0x51 0x8E 0x1F 0x56 0x08 0x57 0x27 0xA7 0x05 0xD4 0xD0 0x52 0x82 0x77 0x75 
as608: check page number 0 ok.
as608: random.
0x1B 0x99 0x4A 0xED 0x58 0x3D 0x6A 0x52 0x36 0xD5 0x24 0x4A 0x68 0x8E 0xAD 0x95 0x5F 0x3C 0x35 0xB5 0xC4 0x8C 0xDD 0x6C 0x11 0x32 0x3D 0xE2 0xB4 0xB4 0x59 0xCF 
as608: check page number 1 ok.
as608: random.
0xCE 0x23 0x3D 0x27 0xDF 0xA7 0xF9 0x96 0xFC 0x1E 0xE0 0x66 0x2C 0x0E 0x7B 0x8C 0xCA 0x30 0x42 0x8F 0xBC 0x9F 0x7B 0xCE 0xD1 0xB8 0xB1 0x87 0xEC 0x8A 0xD6 0xBB 
as608: check page number 2 ok.
as608: random.
0x2E 0x15 0x63 0x0E 0x3C 0xDC 0xA4 0x3A 0x7A 0x06 0x20 0xA7 0x93 0x1B 0x34 0xDD 0x4C 0xF5 0xEC 0x88 0x96 0x68 0xD6 0x68 0xA0 0x09 0x6F 0x8E 0x93 0x47 0xC9 0x41 
as608: check page number 3 ok.
as608: random.
0xDB 0xAC 0xCF 0x97 0x89 0xF3 0x51 0x05 0x79 0x71 0x2C 0x0E 0x0D 0x60 0x6B 0x59 0xD5 0x59 0xE1 0x6C 0xC1 0xB9 0x55 0x63 0x42 0x44 0x71 0x55 0x0B 0xBA 0x97 0xE6 
as608: check page number 4 ok.
as608: random.
0x68 0x67 0xFE 0x71 0x5B 0x50 0x76 0x55 0xC2 0x22 0x63 0x4F 0x02 0xCE 0xA8 0xD8 0xA7 0x0A 0x45 0x6A 0x43 0x1A 0x4D 0x85 0x5F 0x3E 0xDB 0x6A 0xF8 0x73 0xD1 0x61 
as608: check page number 5 ok.
as608: random.
0x5A 0xD0 0x53 0x35 0x21 0xC9 0x8A 0x63 0xEC 0x6D 0xB2 0xEE 0xBC 0xDA 0x47 0x64 0xE5 0x0D 0x4E 0xA8 0x27 0x1B 0x2F 0x06 0x59 0x8A 0xF0 0x53 0x7D 0xC2 0x34 0xD7 
as608: check page number 6 ok.
as608: random.
0x13 0x87 0x8C 0xB3 0xD1 0x97 0x18 0x3E 0x84 0x4A 0xAC 0xC0 0xA5 0xF4 0x26 0x0B 0x02 0xF3 0xB3 0xA8 0x10 0x62 0xAF 0xE8 0xEC 0xA0 0x3C 0x6A 0xE2 0xF0 0xC1 0xF5 
as608: check page number 7 ok.
as608: random.
0x78 0xCE 0xA9 0xC9 0x66 0x41 0x08 0x6A 0x0C 0x35 0x2C 0xB1 0x2A 0xD1 0x3C 0xAB 0xC5 0xEF 0x54 0x55 0x53 0x83 0x3F 0xBF 0x24 0xFA 0x2B 0x86 0xEB 0x6C 0x7C 0x65 
as608: check page number 8 ok.
as608: random.
0x3B 0xA6 0xAE 0x21 0x67 0xB7 0x8C 0x73 0xEC 0x38 0xA4 0x96 0x0A 0xE0 0x42 0xCF 0x51 0x16 0xA5 0x24 0x9A 0xE4 0xE3 0x3E 0xDF 0x8E 0x45 0xCC 0xFB 0x41 0xB1 0xB6 
as608: check page number 9 ok.
as608: random.
0xE7 0x60 0x58 0xCF 0x97 0x64 0xC2 0x04 0x9C 0x68 0x9A 0xA6 0xC8 0x5C 0xF5 0x99 0xF2 0x9B 0xBD 0x0D 0x00 0x22 0xCA 0xE0 0xB0 0x10 0x2D 0x2C 0x52 0xDE 0xE3 0xB9 
as608: check page number 10 ok.
as608: random.
0xBE 0x3C 0x89 0xD6 0xA0 0x4D 0xDA 0xBC 0x35 0xF5 0x63 0x7D 0xD1 0xD8 0x18 0xC4 0x75 0x55 0xD1 0xF4 0x77 0x1D 0xD5 0x29 0x2D 0x82 0x55 0x7F 0xE0 0xB8 0x3A 0xA0 
as608: check page number 11 ok.
as608: random.
0x74 0x43 0x77 0x94 0x10 0xD1 0x51 0x45 0xC7 0x34 0x43 0x1A 0x0E 0x5B 0xDE 0x03 0x30 0x31 0xF7 0xA8 0x4E 0x4E 0x51 0x7B 0xD0 0x26 0x7B 0xB2 0xDF 0x35 0xD2 0x54 
as608: check page number 12 ok.
as608: random.
0xF7 0xC9 0xE9 0x09 0x9B 0xBA 0x4E 0xE3 0xEF 0x91 0xFD 0xFD 0x6C 0x5C 0x80 0x9D 0x8D 0x78 0xC5 0x5B 0x46 0x17 0x57 0x18 0x3D 0xD2 0x4A 0x9C 0x87 0x1D 0x71 0x7F 
as608: check page number 13 ok.
as608: random.
0xE6 0xDA 0x88 0x02 0x95 0x57 0xE5 0x85 0x68 0xE3 0x03 0xD5 0x41 0x83 0xF2 0x4E 0x7C 0x38 0xAA 0xC2 0x4F 0x02 0x5A 0x0C 0x54 0xA4 0xA9 0xDB 0x41 0x9A 0xDA 0x28 
as608: check page number 14 ok.
as608: random.
0x75 0x64 0x2B 0x0B 0x3B 0x90 0x11 0xA3 0x75 0x14 0xF8 0x36 0x18 0xEB 0x84 0x14 0x24 0xAE 0x56 0xF2 0xB0 0xB1 0x00 0x84 0xD5 0x29 0x60 0x18 0xC3 0x3C 0xBF 0x39 
as608: check page number 15 ok.
as608: as608_set_gpio_level test.
as608: set gpio0 level low.
as608: set gpio0 level low.
as608: set gpio1 level high.
as608: set gpio1 level high.
as608: finish register test.
```

```shell
./as608 -t fingerprint

as608: chip is Synochip AS608.
as608: manufacturer is Synochip.
as608: interface is UART.
as608: driver version is 1.0.
as608: min supply voltage is 3.0V.
as608: max supply voltage is 3.6V.
as608: max current is 60.00mA.
as608: max temperature is 85.0C.
as608: min temperature is -40.0C.
as608: start fingerprint test.
as608: please put your finger on the sensor.
as608: generate feature success.
as608: please put your finger on the sensor again.
as608: generate feature success.
as608: match feature and score is 112.
as608: combine feature.
as608: store feature to page 208.
as608: please put your finger on the sensor.
as608: high speed found in the page 208 and score is 343.
as608: please put your finger on the sensor.
as608: found in the page 208 and score is 225.
as608: valid template number is 1.
index table 0: 
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x01 0x00 0x00 0x00 0x00 0x00 
index table 1: 
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 
index table 2: 
0x02 0x00 0x26 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 
index table 3: 
0x03 0x00 0x27 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 
as608: load feature from page number 208.
as608: upload feature is 768bytes.
0xAA 0xDC 0xFF 0x27 0x2A 0x46 0xDF 0x6A 0xB4 0x82 0x3F 0x16 0xB4 0x06 0xFF 0x44 0x39 0x05 0x9F 0x4B 0xC7 0xC4 0x9F 0x40 0xCA 0x05 0x1F 0x56 0xCF 0x41 0x9F 0x51 0x92 0x89 0xBC 0x23 0xCB 0x1C 0xBC 0x58 0x12 0xE1 0x3D 0x2A 0x4C 0x86 0x1D 0x66 0x51 0x17 0xFD 0x5E 0xD0 0x19 0x3B 0x7A 0x16 0x24 0x34 0x78 0x9A 0xAA 0x35 0x55 0x85 0x62 0xBE 0x39 0x07 0xA1 0x1E 0x64 0x0B 0xA4 0x9E 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x01 0x00 0x00 0x00 0x0F 0x45 0x00 0x10 0xD0 0x00 0x00 0x00 0x00 0xC2 0x01 0x20 0x03 0x03 0x63 0x22 0x1C 0x01 0x59 0x01 0x7A 0x00 0x00 0x00 0x00 0x00 0x00 
0xE4 0x14 0x73 0x99 0x6A 0x15 0x51 0x06 0xE2 0x9E 0x34 0x89 0xE0 0xFE 0x60 0x0C 0xA4 0x7E 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x0E 0x00 0x00 0x00 0x91 0x00 0x00 0x00 0x03 
0x03 0x03 0x5D 0x22 0x18 0x01 0x5B 0x01 0x7C 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x0A 0x00 0x00 0x00 0x93 0x00 0x00 0x00 0x00 0x00 0x00 0xCC 0xCF 0xCF 0xFF 0xFF 0xBF 0xFB 0xFF 0xEE 0xFE 0xFF 0xFF 0xBA 0xEA 0xAA 0xAA 0xA9 0xA9 0x99 0x99 0x66 0x56 0x55 0x45 0x55 0x55 0x51 0x54 0x54 0x54 0x44 0x40 0x01 0x01 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x01 0x01 0x01 0x01 0x01 0x01 0x01 0x01 0x01 0x01 0x01 0x01 0x01 0x01 0x00 0x53 0x98 0xA0 0x3E 0x41 0x1A 0xC7 0xDE 0x69 0x30 0x01 0xBE 0x6F 0xC3 0x84 0x5E 0x3B 0xC3 0x45 0xDE 0x47 0xC3 0xDB 0xDE 0x2F 0xC4 0xDD 0x3E 0x6A 0xCB 0x59 0x5E 0x38 0x92 0xA0 0xBF 0x4B 0x9D 0x5F 0x1F 0x1A 0x9F 0xDF 0x1F 0x75 0x24 0xC0 0x1F 0x3E 0xA3 0xDE 0x9F 0x31 0xA8 0xDD 0x7F 0x4C 0xAA 0x05 0x5F 0x56 
0x00 0xCC 0xCF 0x3F 0xFF 0xFE 0xFF 0xEF 0xFF 0xBF 0xFF 0xFF 0xEE 0xEA 0xAA 0xAA 0xAA 0xA9 0x99 0xA9 0x59 0x65 0x65 0x55 0x55 0x45 0x54 0x54 0x55 0x51 0x11 0x10 0x04 0x01 0x01 0x01 0x01 0x01 0x01 0x01 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x01 0x01 0x01 0x01 0x01 0x01 0x01 0x01 0x01 0x01 0x01 0x01 0x01 0x01 0x01 0x01 0x01 0x01 0x01 0x01 0x4E 0x98 0xE0 0x1E 0x3C 0x1C 0x07 0xBE 0x68 0x2F 0x41 0x9E 0x72 0x42 0x84 0x3E 0x3E 0x44 0x85 0xBE 0x4A 0x44 0xDB 0xBE 0x32 0x46 0x9D 0x1E 0x6E 0x4A 0x99 0x3E 0x32 0x94 0x20 0x9F 0x47 0x1E 0x5E 0xFF 0x17 0x22 0x9E 0xFF 0x72 0x23 0xEC 0xFF 0x3B 0xA5 0x1E 0x7F 0x2F 0x2A 0x9D 0x5F 0x4A 0xAA 0x85 0x3F 0x54 0xAB 0x1C 0xDF 0x25 0x2C 0xC6 0xBF 0x6A 0x33 0xC2 0x1F 0x16 0xB7 0x06 0xDF 0x44 0xBA 0x05 0x7F 0x4E 0xC8 0x44 0x7F 0x44 0xCB 0x44 0xFF 0x5B 0x4F 0x81 0x7F 0x4B 0x93 0x09 0x9C 0x27 0xCD 0x9C 0x9C 0x52 0x13 0x21 0x1D 0x2E 0xCE 0x85 0xFD 0x6A 0xD0 0x57 0xDD 0x63 0x4F 0xD9 0x1B 0x74 0x94 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x01 0x00 0x00 0x00 0x0F 0x45 0x00 0x10 0xD0 0x00 0x00 0x00 0x00 0xC2 0x01 0x20 
as608: download feature ok.
as608: upload image is 36864bytes.
as608: download image ok.
as608: please put your another finger on the sensor.
as608: enroll ok with page number 0.
as608: please put your another finger on the sensor.
as608: identify ok with page number 0 and score 155.
as608: delete feature in page 208.
as608: empty all feature.
as608: finish fingerprint test.
```

```shell
./as608 -e basic-fingerprint --addr=0xFFFFFFFF

please put your finger on the sensor.
please put your finger on the sensor again.
generate feature success.
input fingerprint success with score 282 in page number 0.
```

```shell
./as608 -e basic-verify --addr=0xFFFFFFFF

verify success with score 187 in found page 0.
```

```shell
./as608 -e basic-fast-verify --addr=0xFFFFFFFF

verify success with score 174 in found page 0.
```

```shell
./as608 -e basic-delete --addr=0xFFFFFFFF --num=0

delate page number 0 success.
```

```shell
./as608 -e basic-empty --addr=0xFFFFFFFF

empty success.
```

```shell
./as608 -e advance-fingerprint --addr=0xFFFFFFFF

please put your finger on the sensor.
please put your finger on the sensor again.
generate feature success.
input fingerprint success with score 411 in page number 0.
```

```shell
./as608 -e advance-verify --addr=0xFFFFFFFF

verify success with score 119 in found page 0.
```

```shell
./as608 -e advance-fast-verify --addr=0xFFFFFFFF

verify success with score 165 in found page 0.
```

```shell
./as608 -e advance-delete --addr=0xFFFFFFFF --num=0

delate page number 0 success.
```

```shell
./as608 -e advance-empty --addr=0xFFFFFFFF

empty success.
```

```shell
./as608 -e advance-write --addr=0xFFFFFFFF --num=0 --data=000102030405060708090A0B0C0D0E0F101112131415161718191A1B1C1D1E1F

write success.
```

```shell
./as608 -e advance-read --addr=0xFFFFFFFF --num=0

00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F 
```

```shell
./as608 -e advance-random --addr=0xFFFFFFFF

random is 0xA6DD0C9F.
```

```shell
./as608 -e advance-flash --addr=0xFFFFFFFF

00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 74 6D 70 2E 64 61 74 61 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 78 E4 F7 65 5F 61 64 76 61 6E 63 65 2D 66 6C 61 73 68 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 3E E4 F7 00 00 00 00 3C F7 E1 F7 01 00 00 00 64 54 E1 F7 16 00 00 00 34 55 E1 F7 C0 85 E4 F7 EC F5 A0 FF E8 F5 A0 FF 9C 10 C0 F7 E0 38 E4 F7 00 00 00 00 34 55 E1 F7 04 53 E1 F7 40 83 D4 F7 EF B5 BD 85 AF ED 2D 04 74 F6 A0 FF 80 78 E4 F7 00 80 E4 F7 EC F5 A0 FF 38 3C E4 F7 06 00 00 00 80 78 E4 F7 98 AD 01 00 00 00 00 00 00 00 00 00 68 00 00 00 A0 AD 01 00 00 00 00 00 00 00 00 00 69 00 00 00 AC AD 01 00 00 00 00 00 00 00 00 00 70 00 00 00 B4 AD 01 00 01 00 00 00 00 00 00 00 65 00 00 00 BC AD 01 00 01 00 00 00 00 00 00 00 74 00 00 00 C4 AD 01 00 01 00 00 00 00 00 00 00 01 00 00 00 CC AD 01 00 01 00 00 00 00 00 00 00 02 00 00 00 D4 AD 01 00 01 00 00 00 00 00 00 00 03 00 00 00 D8 AD 01 00 01 00 00 00 00 00 00 00 04 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 E0 38 E4 F7 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 D0 96 01 00 00 00 00 00 C8 0A 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 80 E4 F7 00 00 00 00 88 0A 01 00 D0 96 01 00 40 B7 C0 F7 00 30 D4 F7 24 F8 A0 FF 04 00 00 00 80 0A 01 00 A9 0E 15 A8 95 4E 75 A0 D0 96 01 00 00 00 00 00 C8 0A 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 80 E4 F7 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
```

```shell
./as608 -e advance-param --addr=0xFFFFFFFF

status is 0x00.
sensor type is 0x00.
fingerprint size is 300.
level is 0x03.
address is 0xFFFFFFFF.
packet size is 2.
baud rate is 57600.
```

```shell
./as608 -e advance-enroll --addr=0xFFFFFFFF

enroll ok and page number is 0.
```

```shell
./as608 -e advance-identify --addr=0xFFFFFFFF

identify ok and page number is 0, score is 171.
```

```shell
./as608 -e advance-upload-flash-feature --addr=0xFFFFFFFF --num=0 --path=feature.data

upload flash feature ok.
```

```shell
./as608 -e advance-upload-image-feature --addr=0xFFFFFFFF --path=feature.data

upload image feature ok.
```

```shell
./as608 -e advance-download-flash-feature --addr=0xFFFFFFFF --num=0 --path=feature.data

download flash feature ok.
```

```shell
./as608 -e advance-upload-image --addr=0xFFFFFFFF --path=image.data

upload image ok.
```

```shell
./as608 -e advance-download-image --addr=0xFFFFFFFF --path=image.data

download image ok.
```

```shell
./as608 -h

Usage:
  as608 (-i | --information)
  as608 (-h | --help)
  as608 (-p | --port)
  as608 (-t reg | --test=reg)
  as608 (-t fingerprint | --test=fingerprint)
  as608 (-e basic-fingerprint | --example=basic-fingerprint) [--addr=<address>]
  as608 (-e basic-verify | --example=basic-verify) [--addr=<address>]
  as608 (-e basic-fast-verify | --example=basic-fast-verify) [--addr=<address>]
  as608 (-e basic-delete | --example=basic-delete) [--addr=<address>] [--num=<page>]
  as608 (-e basic-empty | --example=basic-empty) [--addr=<address>]
  as608 (-e advance-fingerprint | --example=advance-fingerprint) [--addr=<address>]
  as608 (-e advance-verify | --example=advance-verify) [--addr=<address>]
  as608 (-e advance-fast-verify | --example=advance-fast-verify) [--addr=<address>]
  as608 (-e advance-delete | --example=advance-delete) [--addr=<address>] [--num=<page>]
  as608 (-e advance-empty | --example=advance-empty) [--addr=<address>]
  as608 (-e advance-write | --example=advance-write) [--addr=<address>] [--num=<page>] [--data=<data>]
  as608 (-e advance-read | --example=advance-read) [--addr=<address>] [--num=<page>]
  as608 (-e advance-random | --example=advance-random) [--addr=<address>]
  as608 (-e advance-flash | --example=advance-flash) [--addr=<address>]
  as608 (-e advance-param | --example=advance-param) [--addr=<address>]
  as608 (-e advance-enroll | --example=advance-enroll) [--addr=<address>]
  as608 (-e advance-identify | --example=advance-identify) [--addr=<address>]
  as608 (-e advance-upload-flash-feature | --example=advance-upload-flash-feature) [--addr=<address>] [--num=<page>] [--path=<path>]
  as608 (-e advance-upload-image-feature | --example=advance-upload-image-feature) [--addr=<address>] [--path=<path>]
  as608 (-e advance-download-flash-feature | --example=advance-download-flash-feature) [--addr=<address>] [--num=<page>] [--path=<path>]
  as608 (-e advance-upload-image | --example=advance-upload-image) [--addr=<address>] [--path=<path>]
  as608 (-e advance-download-image | --example=advance-download-image) [--addr=<address>] [--path=<path>]

Options:
      --addr=<address>    Set chip address and it is hexadecimal.([default: 0xFFFFFFFF])
      --data=<data>       Set notepad data and it is hexadecimal with 32 bytes.([default: random])
  -e <basic-fingerprint | basic-verify | basic-fast-verify | basic-delete | basic-empty
     | advance-fingerprint | advance-verify | advance-fast-verify | advance-delete | advance-empty
     | advance-write | advance-read | advance-random | advance-flash | advance-param | advance-enroll
     | advance-identify | advance-upload-flash-feature | advance-upload-image-feature |
     | advance-download-flash-feature | advance-upload-image | advance-download-image>, --example=<basic-fingerprint
     | basic-verify | basic-fast-verify | basic-delete | basic-empty
     | advance-fingerprint | advance-verify | advance-fast-verify | advance-delete | advance-empty
     | advance-write | advance-read | advance-random | advance-flash | advance-param | advance-enroll
     | advance-identify | advance-upload-flash-feature | advance-upload-image-feature |
     | advance-download-flash-feature | advance-upload-image | advance-download-image>
                          Run the driver example.
  -h, --help              Show the help.
  -i, --information       Show the chip information.
      --num=<page>        Set flash page.([default: 0])
  -p, --port              Display the pin connections of the current board.
      --path=<path>       Set file path.([default: "tmp.data"])
  -t <reg | fingerprint>, --test=<reg | fingerprint>
                          Run the driver test.
```

