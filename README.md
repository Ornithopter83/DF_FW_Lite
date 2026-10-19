# DF Firmware Lite

Main/Rod 하드웨어를 그대로 사용하면서 핵심 동작만 남기기 위한 **독립 소스 저장소**입니다.

## 현재 상태 (2026-10-19)

**1단계: 독립 저장소로 이관.** Main/Rod의 활성 펌웨어 소스는 원본 내용 그대로 복사하며 기능 삭제나 동작 변경은 하지 않습니다. TM은 최소 WinForms 프로젝트로 별도 구성합니다. Lite 기능 축소는 2단계 작업입니다.

- 원본: https://github.com/Ornithopter83/DF_FW_Code
- 원본 기준 커밋: `926c010b2f65263cfb96b207cf4ad57ddb2510ea`
- 하드웨어: 기존 Main / Rod 보드 사용, 핀·회로 변경 없음
- 시리얼: 115200bps / `$...%` 프레임, Main↔Rod: 기존 ESP-NOW PID
- **빌드, 테스트, 플래시, 장비 검증: 이번 이관 작업에서 수행하지 않음**

## 폴더

- `firmware/DF_Main`, `firmware/DF_Rod`: 원본 활성 펌웨어 소스 사본 (각각 51 / 35개 파일)
- `libraries/DFProtocol`: 두 펌웨어가 공유하는 통신 프로토콜
- `testModule`: 업데이트·3D 렌더링 등을 제거한 최소 PC 프로그램의 출발점
- `vs`, `DF_Firmware.sln`: 기존 Main/Rod VS2022 Makefile 프로젝트 및 Lite TM 솔루션 진입점
- `tools`: 펌웨어 빌드에 필요한 최소 스크립트
- `docs`: 통신 및 하드웨어 계약, 이관 기록

## 로컬 작업 준비

```powershell
git clone https://github.com/Ornithopter83/DF_FW_Lite.git
cd DF_FW_Lite
```

- Windows + Visual Studio 2022 C++ 개발 도구, .NET 8 SDK, ESP32 Arduino CLI/ESP32 core 2.0.17이 필요합니다.
- **기존 스크립트는 `toolchain/`에 로컬 Arduino CLI와 설정이 설치되어 있다고 가정합니다.** 이는 컴퓨터별 환경이므로 Git에 포함하지 않았습니다. Arduino 외부 라이브러리(예: ESP32Servo)도 작업 PC에 별도 준비가 필요합니다.
- TM은 `testModule/DFTestModule.csproj`를 열어 개발합니다. NuGet 패키지 복원은 로컬에서 별도로 필요합니다.
- 이관 후 빌드 가능 여부/연결 동작 여부는 아직 검증되지 않았습니다. 실제 모터 제어 전에 보호 장치와 정지 경로를 확인하십시오.

## 다음 단계

Main GameControl/Scheduler/통신 의존성 파악 후 최소 상태 머신으로 축소, Rod 입력/출력 최소화, TM 기능 확정 및 세 제품 통합 검증. **원본 저장소는 수정하지 않습니다.**

세부 내용: [이관 기록](docs/MIGRATION_STAGE1.md).

## Lite 2단계 기능 경계 (2026-10-19)

- 유지: Main Torque/BLDC 수동 제어와 타임아웃·안전 정지, ESP-NOW, WDT, 필수 전원·USB 분리 보호.
- 유지: Rod 버튼·엔코더·IMU·배터리·LED·진동 및 등록/주소 설정.
- **유지: Rod 배터리 ADC 상세 진단(PID 28 → TM `$31`), 엔코더 보정(PID 35/36 → TM `$37`), Main 장치 검사와 TM 검사 명령.**
- 제거 대상: LM JIG 전용 하드웨어 제어, 게임 전용 동작, 별도 Board Wi-Fi AP/TCP, Main/Rod OTA, 제조·에이징·불필요한 상세 관리 기능, TM 전원/LM JIG 기능.
- 소스 주석은 제거한다. 이 문서와 라이선스·필수 메타데이터는 보존한다.
- 경량화 변경은 장비 동작을 증명하지 않는다. 빌드/테스트는 사용자 사전 승인 후, 플래시와 실제 장비 시험은 사용자가 수행한다.
- DF_FW_Code 원본 저장소는 수정하지 않는다.
