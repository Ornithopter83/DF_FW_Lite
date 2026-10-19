# DF Lite TestModule

원본 `DF_FW_Code/testModule`의 V041 WinForms 화면 구조를 복원한 Lite 버전입니다.

- 원본 기준: `Ornithopter83/DF_FW_Code` @ `926c010b2f65263cfb96b207cf4ad57ddb2510ea`.
- 화면: **모터 제어**, **릴 / IMU**, **입력**, **LED**, **업데이트**. 상단 COM·버전 표시, 좌측 메뉴, 우측 수동 명령·로그 UI 유지.
- 제거: **LM JIG** 화면·명령과 **전원/USB 입력 감시** 그룹·전원 명령. 다른 입력 감시 기능은 유지.
- 유지: 배터리 퍼센트 및 ADC 상세 표시(`$31`), 릴 엔코더 보정(`$37`), Main 장치 검사(`$1501`), 버튼 상태·IMU 자세·LED·진동·Rod 등록.
- ADC 상세 정보 송수신 ON/OFF 추가: TM의 릴/IMU 화면에서 **배터리 ADC 상세 정보** 스위치 사용. 연결 후 기본 ON, 연결 해제 시 OFF. TM→Main `$31S1%`/`$31S0%` 명령이며, Main→Rod 기존 ApInfo PID 33의 하위 종류 `02`를 통해 설정합니다. ON일 때만 Rod→Main PID 28 및 Main→TM `$31` 상세 정보가 흐릅니다. 일반 배터리 잔량 PID 26 및 TM `$12`는 상시 유지합니다.
- 원본 TestModule의 **MAIN USB 유선 업데이트**와 **ROD USB 유선 업데이트**를 복원했습니다. 내장 `esptool_V4.5.1.exe`로 bootloader/partitions/boot_app0/application 네 영역을 기록하고, 포트·펌웨어 버전·SHA-256·진행률·로그를 표시합니다.
- **ROD 무선 업데이트는 비활성화**했습니다. Lite Main/Rod 펌웨어에는 해당 OTA 수신 경로가 제거되어 있어 TM만 복구해도 동작하지 않습니다. Rod 등록 기능은 계속 사용 가능합니다.
- 원본의 Torque/BLDC 제어 슬라이더·버튼 복원. Torque 출력은 안전을 위해 한 번에 1초 명령으로 전송하며, 연결 해제 시 모터 정지 명령을 시도합니다.
- 우측 수동 명령 창은 Lite에서 지원하는 명령 접두어만 허용합니다.

## 내장 업데이터 복원 및 빌드

Visual Studio 또는 .NET SDK에서 TestModule을 빌드할 때 `tools/restore-tm-updater.ps1`이 실행됩니다. 원본 `DF_FW_Code/testModule/esptool_V4.5.1.exe`를 인접 디렉터리에서 찾거나 원본 GitHub raw URL에서 받아 파일 크기(6,638,312 bytes)와 원본 Git blob SHA-1(`dc686a0d53221ee77e3f4de3967be18c23a786b2`)을 확인합니다. 파일이 확인되면 `DFTestModule.esptool_V4.5.1.exe` 리소스로 TM 실행 파일에 내장됩니다. 배포된 TM에서는 네트워크 연결 없이 사용할 수 있습니다.

원본 파일을 자동으로 가져올 수 없으면 동일 파일을 `testModule/esptool_V4.5.1.exe`에 직접 배치하거나 다음 명령으로 복구한 후 다시 빌드할 수 있습니다.

```powershell
powershell -ExecutionPolicy Bypass -File .\tools\restore-tm-updater.ps1
```

ROD 무선 업데이트 버튼은 사용할 수 없습니다. MAIN/ROD USB 업데이트는 대상 보드에 직접 USB 연결이 필요하며, 파일명 `DF_Main.ino.*.bin` 또는 `DF_Rod.ino.*.bin`을 포함한 원본 규격 네 파일이 필요합니다. 업데이트 전에 COM 포트와 대상 버전을 확인해야 합니다.

## 원본 릴 사진 텍스처 복원

`ImuOrientationView`와 3D 메시 렌더링 소스는 복원했습니다. 원본 이미지 두 개를 사용할 경우 다음 경로에 복사하면 빌드 시 자동 포함됩니다.

- `testModule/Assets/reel-top-v2.png`
- `testModule/Assets/reel-bottom.png`

원본 저장소의 두 파일을 쉽게 복사하려면 `powershell -ExecutionPolicy Bypass -File .\tools\restore-tm-assets.ps1`을 실행하십시오. 원본을 로컬에서 찾을 수 없다면 해당 스크립트가 GitHub 공개 원본 경로에서 내려받습니다. 이미지가 없어도 3D 형상 렌더링은 작동하도록 설계돼 있으나 원본 사진 텍스처는 표시되지 않습니다.

## 실행

Visual Studio 2022에서 `testModule/DFTestModule.sln` 또는 루트 `DF_Firmware.sln`을 열어 `DFTestModuleLite`을 선택할 수 있습니다. `.NET 8 Windows Forms x64` 및 `System.IO.Ports`가 사용됩니다.

이번 변경은 **빌드·실행·장비 검증을 수행하지 않은 소스 복원**입니다. 모터 구동 전 부하·정지 경로를 확인하십시오.
