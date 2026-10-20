# Lite Main / Rod 활성 하드웨어 핀 맵

작성일: 2026-10-20
소스 기준 버전: Main `Vm1.0.10.0`, Rod `Vr1.0.2.0`

## 범위

이 문서는 현재 펌웨어 구성에서 실제 초기화하거나 사용하는 MCU GPIO와 통신 핀만 모은 목록이다. 핀 기능은 펌웨어 상수, 활성 설정, 초기화·사용 경로를 기준으로 정리했다. 회로도나 실물 PCB 배선을 대조한 전기적 검증 결과는 아니다.

## Main

| GPIO | 연결 신호 / 장치 | 방향·활성 동작 | 펌웨어 식별자 |
| ---: | --- | --- | --- |
| 0 | Boot mode | 입력, 부트 모드 감지 | `BOOT_MODE_PIN` |
| 3 | PC USB 5 V 감지 | 입력 풀업 | `PC_USB5V_PIN` |
| 4 | Bobbin BLDC FG | 입력 풀업 | `BLDC_FG_PIN` |
| 5 | Bobbin 모터 ON | 출력, HIGH 켜짐 | `BBN_MOT_ON_PIN` |
| 6 | Bobbin 모터 방향 | 출력, HIGH=CW / LOW=CCW | `BBN_MOT_DIR_PIN` |
| 7 | Bobbin 모터 PWM | 출력, LEDC CH0, 22 kHz / 10 bit | `BBN_MOT_PWM_PIN` |
| 8 | I2C SDA | 양방향, ESP32-S3 Arduino 기본 SDA | `I2C_SDA_PIN` |
| 9 | I2C SCL | 출력, ESP32-S3 Arduino 기본 SCL | `I2C_SCL_PIN` |
| 10 | Torque 제어용 SPI MOSI | 출력 | `TORQ_MOSI` |
| 11 | Torque 제어용 SPI SCK | 출력 | `TORQ_SCK` |
| 12 | Torque 제어용 SPI SS | 출력 | `TORQ_SS` |
| 13, 14, 21 | 중앙 RGB LED R/G/B | PWM 출력 | `LED_CENT_R_PIN`, `LED_CENT_G_PIN`, `LED_CENT_B_PIN` |
| 15 | SUB AC 전원 차단 제어 | 출력, LOW 동작 | `SUB_ACOFF_PIN` |
| 16 | 전원 스위치 입력 | 입력, LOW 동작 | `PWROFF_SWC_PIN` |
| 17 | Bobbin encoder A / interrupt | 입력 풀업, FALLING 인터럽트 | `BBN_ENC_A_CK_PIN`, `ENC_INTR_PIN` |
| 18 | Bobbin encoder B | 입력 풀업 | `BBN_ENC_B_DT_PIN` |
| 19, 20 | USB D-/D+ | ESP32-S3 USB 인터페이스 | `USB_DM`, `USB_DP` |
| 38 | Bobbin BLDC 24 V enable | 출력, LOW 켜짐 | `BBN_BLDC_24VON_PIN` |
| 39, 40, 41 | 보드 LED2/LED3/LED4 | 출력 | `BD_LED2_PIN`, `BD_LED3_PIN`, `BD_LED4_PIN` |
| 42 | 하단 LED | PWM 출력 | `LED_BTM_PIN` |
| 43, 44 | UART1 로그 TX/RX | 115200, 8N1 | `UART1_TX_PIN`, `UART1_RX_PIN` |
| 47 | 외부 WDT 출력 | 출력, 초기화 시 HIGH→LOW→HIGH 토글 | `WDT_OUT_PIN` |

활성 설정 근거: torque, Bobbin, LED, 전원 제어, I2C가 활성화되어 있다. 외부 EEPROM은 I2C를 사용한다. I2C 기본 핀은 ESP32-S3 variant의 `SDA=8`, `SCL=9` 설정과 일치한다.

## Rod

| GPIO | 연결 신호 / 장치 | 방향·활성 동작 | 펌웨어 식별자 |
| ---: | --- | --- | --- |
| 0 | Boot mode | 입력, 부트 모드 감지 | `BOOT_MODE_PIN` |
| 1, 2 | IMU UART1 RX/TX | 115200, 8N1 | `IMU_RX`, `IMU_TX` |
| 4, 5 | 좌/우 버튼 V2 | 입력 | `BTN_BLUE_PIN_NEW`, `BTN_RED_PIN` |
| 6, 7 | Green / Red 상태 LED | 출력 | `LED1_GRN_PIN`, `LED2_RED_PIN` |
| 8, 9 | 좌/우 버튼 LED | 출력, LOW 켜짐 | `BTN_LED_LF_RED_PIN`, `BTN_LED_RT_BLUE_PIN` |
| 12 | 진동 모터 ON | 출력, HIGH 켜짐 | `VRT_MOT_ON_PIN` |
| 14 | 배터리 전압 ADC | 아날로그 입력 | `BATTERY_LVL_PIN` |
| 18 | Reel encoder V3 Hall 입력 | 입력 풀업 | `ENC_HALL_SEN_PIN` |
| 19, 20 | USB D-/D+ | ESP32-S3 USB 인터페이스 (`Serial`) | USB CDC |
| 43, 44 | Debug UART2 TX/RX | 115200, 8N1 | `DEBUG_TX_PIN`, `DEBUG_RX_PIN` |
| 48 | Setup mode 감지 | 입력, LOW=Normal / HIGH=Setup | `SETUP_MODE_PIN` |

현재 구성은 Button V2, Brake 없음, Reel encoder V3, Battery 800 V2로 고정되어 있다.

## 정의되어 있지만 활성 목록에서 제외한 핀

- Main GPIO 35/36/37/45/46/48은 strap 또는 board-type 후보 상수로만 정의되어 있으며 현재 활성 pin I/O로 사용하지 않는다. Board type 판별 경로도 비활성이다.
- Rod GPIO 39의 board-type 감지는 `ROD_BDTYPE_DETECT=0`으로 비활성이다.
- Rod GPIO 10/11/12의 SPI potentiometer 상수는 brake 경로용이다. 현재 `BREAK_NO` 구성에서는 초기화·호출되지 않으며, GPIO 12는 활성 진동 모터 출력으로 사용된다.
- Main Servo와 외부 UART는 설정에서 비활성이다. Servo 관련 대체 GPIO 연결은 이 목록에 포함하지 않았다.

## 근거 파일

- Main 핀 및 기능 정의: `firmware/DF_Main/src/Common.h`, 활성 구성: `firmware/DF_Main/src/Config.h`
- Rod 핀 및 보드 구성 정의: `firmware/DF_Rod/src/Common.h`, `firmware/DF_Rod/src/DF_Rod_State.cpp`
- 초기화·사용 경로: `firmware/DF_Main/src/DF_Main_Application.cpp`, Main 장치별 `init()` 구현, `firmware/DF_Rod/src/DF_Rod_Application.cpp`
- ESP32-S3 I2C 기본 핀: `toolchain/arduino-data/packages/esp32/hardware/esp32/2.0.17/variants/esp32s3/pins_arduino.h`

실제 PCB 배선과 커넥터 실크, 회로도는 별도 대조가 필요하다. 장비 검증을 완료한 핀아웃으로 간주하지 않는다.
