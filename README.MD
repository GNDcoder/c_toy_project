# STM32F446RE Firmware Platform

NUCLEO-F446RE를 기반으로 I²C/SPI 디바이스 드라이버, 외부 Flash 저장 시스템, Logic Analyzer 검증 및 Firmware Bootloader를 구현하는 임베디드 펌웨어 프로젝트입니다.

단순히 센서 값을 읽는 것에 그치지 않고, 각 하드웨어에 대한 드라이버를 직접 구성하고 계층화하여 재사용 가능한 펌웨어 구조를 만드는 것을 목표로 합니다.

---

## 1. Project Overview

프로젝트는 다음 순서로 진행합니다.

```text
BME280 I²C 통신
        ↓
BME280 Driver 구현
        ↓
SPI NOR Flash 통신
        ↓
Flash Read / Write / Erase
        ↓
Logic Analyzer 통신 검증
        ↓
센서 데이터 Flash 저장
        ↓
Flash Partition 구성
        ↓
Firmware Staging
        ↓
Bootloader
        ↓
Python HIL 자동 테스트
```

최종적으로 다음 구조를 구현하는 것이 목표입니다.

```text
             PC
             │
             │ UART
             ▼
      NUCLEO-F446RE
        │        │
       I²C      SPI
        │        │
        ▼        ▼
     BME280   SPI NOR Flash
        │        │
        └────┬───┘
             │
      Logic Analyzer
```

---

# 2. Hardware

## Main Board

### NUCLEO-F446RE

본 프로젝트의 메인 MCU 보드입니다.

MCU:

```text
STM32F446RET6
ARM Cortex-M4
```

주요 역할:

- BME280 센서 제어
- I²C 통신
- SPI 통신
- SPI NOR Flash 제어
- 센서 데이터 처리
- Flash 데이터 저장
- CRC 검증
- UART Debug 출력
- Firmware Update
- Bootloader 실행

---

## Sensor

### BME280

온도, 습도, 기압을 측정하는 환경 센서입니다.

통신 방식:

```text
I²C
```

프로젝트에서는 BME280을 이용하여 I²C 디바이스 드라이버를 구현합니다.

주요 역할:

- 온도 측정
- 습도 측정
- 기압 측정
- I²C Driver 검증

데이터 흐름:

```text
BME280
   │
   │ I²C
   ▼
STM32
   │
   ▼
BME280 Driver
   │
   ▼
Sensor Service
```

---

# 3. BME280 Wiring

BME280은 STM32의 I²C1을 사용합니다.

| BME280 | NUCLEO-F446RE | MCU Pin | 역할      |
| ------ | ------------- | ------- | --------- |
| VCC    | 3.3V          | -       | 전원      |
| GND    | GND           | -       | Ground    |
| SCL    | D15           | PB8     | I²C Clock |
| SDA    | D14           | PB9     | I²C Data  |

연결 구조:

```text
NUCLEO-F446RE             BME280

3.3V   -----------------> VCC
GND    -----------------> GND

D15
PB8    -----------------> SCL

D14
PB9    <----------------> SDA
```

BME280 전원은 본 프로젝트에서 **3.3V를 사용합니다.**

---

# 4. SPI NOR Flash

SPI NOR Flash는 센서 데이터 및 Firmware Image를 저장하기 위한 외부 비휘발성 메모리입니다.

통신 방식:

```text
SPI
```

주요 역할:

- Sensor Log 저장
- 데이터 Read / Write / Erase
- Firmware Image 저장
- Firmware Staging
- Firmware Backup
- Bootloader Update Image 저장

최종적으로 Flash 내부를 여러 영역으로 나누어 사용할 예정입니다.

```text
SPI NOR FLASH

┌─────────────────────────────┐
│ Sensor Log Area             │
├─────────────────────────────┤
│ Configuration Area          │
├─────────────────────────────┤
│ Firmware Metadata           │
├─────────────────────────────┤
│ Firmware Staging Area       │
├─────────────────────────────┤
│ Firmware Backup Area        │
└─────────────────────────────┘
```

---

# 5. SPI NOR Flash Wiring

SPI1을 사용할 예정입니다.

| SPI NOR Flash | NUCLEO-F446RE | MCU Pin | 역할          |
| ------------- | ------------- | ------- | ------------- |
| VCC           | 3.3V          | -       | 전원          |
| GND           | GND           | -       | Ground        |
| SCK / CLK     | D13           | PA5     | SPI Clock     |
| MISO / DO     | D12           | PA6     | Flash → STM32 |
| MOSI / DI     | D11           | PA7     | STM32 → Flash |
| CS / SS       | D10           | PB6     | Chip Select   |

연결 구조:

```text
NUCLEO-F446RE              SPI NOR Flash

3.3V   ------------------> VCC
GND    ------------------> GND

D13
PA5    ------------------> SCK

D12
PA6    <------------------ MISO

D11
PA7    ------------------> MOSI

D10
PB6    ------------------> CS
```

---

# 6. Logic Analyzer

Logic Analyzer는 STM32와 외부 장치 사이에서 실제로 전달되는 디지털 신호를 측정하기 위해 사용합니다.

코드만 확인하는 것이 아니라 실제 I²C / SPI 프로토콜의 동작을 검증하는 것이 목적입니다.

---

## I²C 측정

BME280 통신 검증 시 다음과 같이 연결합니다.

| Logic Analyzer | 연결 신호 |
| -------------- | --------- |
| CH0            | SDA       |
| CH1            | SCL       |
| GND            | STM32 GND |

```text
STM32 PB9 ────── BME280 SDA
       │
       └──────── Logic Analyzer CH0


STM32 PB8 ────── BME280 SCL
       │
       └──────── Logic Analyzer CH1


STM32 GND ────── BME280 GND
       │
       └──────── Logic Analyzer GND
```

Logic Analyzer는 통신선에 **병렬로 연결하여 신호를 관찰**합니다.

확인 항목:

- START
- STOP
- Slave Address
- Read / Write
- ACK / NACK
- Register Address
- Data

---

## SPI 측정

SPI NOR Flash 검증 시 다음과 같이 연결합니다.

| Logic Analyzer | SPI Signal |
| -------------- | ---------- |
| CH0            | SCK        |
| CH1            | MOSI       |
| CH2            | MISO       |
| CH3            | CS         |
| GND            | STM32 GND  |

```text
STM32 PA5 ───── Flash SCK
       │
       └────── Logic Analyzer CH0

STM32 PA7 ───── Flash MOSI
       │
       └────── Logic Analyzer CH1

STM32 PA6 ───── Flash MISO
       │
       └────── Logic Analyzer CH2

STM32 PB6 ───── Flash CS
       │
       └────── Logic Analyzer CH3
```

확인 항목:

- CS Low / High
- Clock
- MOSI Command
- MISO Response
- JEDEC ID
- Read Command
- Page Program
- Sector Erase

---

# 7. Module Roles

전체 시스템에서 각 모듈의 역할은 다음과 같습니다.

| Module         | Interface     | 역할                        |
| -------------- | ------------- | --------------------------- |
| NUCLEO-F446RE  | -             | 전체 시스템 제어            |
| BME280         | I²C           | 온도 / 습도 / 기압 측정     |
| SPI NOR Flash  | SPI           | 데이터 및 Firmware 저장     |
| Logic Analyzer | Digital Input | I²C / SPI 신호 분석         |
| ST-LINK        | SWD           | Firmware Download / Debug   |
| PC             | USB/UART      | Debug 및 향후 Firmware 전송 |

---

# 8. System Architecture

```text
                        PC
                        │
                   UART / USB
                        │
                        ▼
                ┌───────────────┐
                │ Application   │
                └───────┬───────┘
                        │
             ┌──────────┴───────────┐
             │                      │
             ▼                      ▼
      Sensor Service        Storage Service
             │                      │
             ▼                      ▼
       BME280 Driver          SPI NOR Driver
             │                      │
             ▼                      ▼
        I2C Platform           SPI Platform
             │                      │
             ▼                      ▼
           I2C1                    SPI1
             │                      │
             ▼                      ▼
          BME280              SPI NOR Flash
```

---

# 9. Software Structure

현재 소스 구조입니다. 이후 센서 측정과 저장 기능을 이 구조에 추가합니다.

```text
STM32_Firmware_Platform/
│
├── Core/
│   ├── Inc/                 # HAL 및 보드 초기화 헤더
│   └── Src/                 # main, clock, I2C1, USART2, C runtime 초기화
│
├── App/
│   ├── app.c
│   └── app.h
│
├── Device/
│   └── bme280/               # BME280 장치 드라이버
│
├── Platform/
│   ├── i2c_bus.c             # I2C HAL 접근 계층
│   └── debug_uart.c          # UART 로그 출력
├── Drivers/                  # ST HAL 및 ARM/ST CMSIS 의존성
├── cmake/                    # ARM GCC toolchain 설정
├── linker/                   # STM32F446RE Flash/RAM 배치
├── CMakeLists.txt
├── CMakePresets.json
├── build.ps1
└── README.MD
```

현재 시작 코드는 I2C1(PB8/PB9)과 USART2(PA2/PA3)를 초기화하고, BME280 주소 0x76/0x77에서 Chip ID 레지스터 0xD0을 읽도록 구성되어 있습니다. STM32CubeF4 HAL, CMSIS 코어·디바이스 파일, GCC 시작 코드, 링커 스크립트와 CMake 설정을 포함합니다.

---

# 10. Development Environment

| 항목                    | 사용 환경          |
| ----------------------- | ------------------ |
| MCU                     | STM32F446RET6      |
| Board                   | NUCLEO-F446RE      |
| Editor                  | Visual Studio Code |
| Configuration           | STM32CubeMX        |
| Compiler                | Arm GNU Toolchain 15.2.1 |
| Build                   | CMake 4.4.4 / Ninja 1.13.2 |
| Debugger                | ST-LINK            |
| Logic Analyzer          | 24 MHz / 8 Channel |
| Logic Analyzer Software | PulseView          |
| Host Test               | Python / pytest    |

---

# 11. Development Roadmap

현재 개발 순서입니다.

- [x] STM32 프로젝트 생성 및 CMake 빌드
- [x] UART Debug 출력 코드
- [x] I²C1 초기화 코드
- [ ] BME280 Chip ID 확인
- [ ] BME280 온도 측정
- [ ] BME280 습도 측정
- [ ] BME280 기압 측정
- [ ] Logic Analyzer I²C 검증
- [ ] SPI1 설정
- [ ] SPI NOR JEDEC ID 확인
- [ ] Flash Read
- [ ] Flash Page Program
- [ ] Flash Sector Erase
- [ ] Logic Analyzer SPI 검증
- [ ] Sensor Data Flash 저장
- [ ] Sensor Log 관리
- [ ] CRC 검증
- [ ] Flash Partition 구성
- [ ] UART Firmware Transfer
- [ ] Firmware Staging
- [ ] Bootloader
- [ ] Firmware Rollback
- [ ] Python HIL Test

---

# 12. Current Goal

현재 첫 번째 목표는 BME280과 STM32F446RE 사이의 I²C 통신을 구현하는 것입니다.

첫 번째 테스트에서는 BME280의 Chip ID Register를 읽어 정상 연결 여부를 확인합니다.

```text
BME280

Register : 0xD0
Expected : 0x60
```

정상 동작 시 UART 출력:

```text
[BME280] Initializing...

[BME280] Chip ID : 0x60
[BME280] Device detected

[I2C] Communication OK
```

이후 온도, 습도, 기압 측정 기능을 순차적으로 구현합니다.

현재 펌웨어는 교차 컴파일되어 ELF/HEX/BIN 이미지 생성까지 확인했습니다. UART 신호와 BME280 Chip ID의 실제 보드 검증은 보드를 연결한 뒤 진행해야 합니다.

### Starter Firmware

초기 소스 뼈대에서는 HSI를 PLL로 84 MHz까지 올리고, I2C1을 100 kHz, USART2를 115200 baud로 설정합니다. NUCLEO-F446RE의 ST-LINK 가상 COM 포트로 UART 로그를 확인할 수 있습니다. BME280 I2C 라인에는 외부 풀업이 있는 모듈을 사용해야 합니다.

### Build

Windows용 ARM GNU Toolchain 15.2, CMake 4.4.4, Ninja 1.13.2는 사용자 프로필의 `AppData/Local/Programs/STM32Toolchain` 경로에 설치합니다. 프로젝트는 `Drivers/` 아래의 ST HAL 및 CMSIS 소스를 사용합니다. 설치 후 PowerShell에서 다음 명령으로 ELF, HEX, BIN 이미지를 빌드할 수 있습니다.

```powershell
.\build.ps1
```

STM32CubeMX는 이 소스의 주변장치 초기화에 필요하지 않습니다. 빌드는 `CMakePresets.json`과 `cmake/arm-none-eabi-toolchain.cmake` 설정을 사용합니다.
