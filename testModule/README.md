# DF Lite TestModule

원본 `DF_FW_Code/testModule`의 V041 WinForms 화면 구조를 복원한 Lite 버전입니다.

- 원본 기준: `Ornithopter83/DF_FW_Code` @ `926c010b2f65263cfb96b207cf4ad57ddb2510ea`.
- 화면: **모터 제어**, **릴 / IMU**, **입력**, **LED**, **업데이트**. 상단 COM·버전 표시, 좌측 메뉴, 우측 수동 명령·로그 UI 유지.
- 제거: **LM JIG** 화면·명령과 **전원/USB 입력 감시** 그룹·전원 명령. 다른 입력 감시 기능은 유지.
- 유지: 배터리 퍼센트 및 ADC 상세 표시(`$31`), 릴 엔코더 보정(`$37`), Main 장치 검사(`$1501`), 버튼 상태·IMU 자세·LED·진동·Rod 등록.
- 원본 화면의 업데이트 메뉴 자리는 유지하되 **펌웨어 업데이트 버튼은 비활성화**했습니다. Lite 펌웨어에서 제거된 OTA/내장 업데이터를 호출하지 않습니다. Rod 등록은 사용 가능합니다.
- 원본의 Torque/BLDC 제어 슬라이더·버튼 복원. Torque 출력은 안전을 위해 한 번에 1초 명령으로 전송하며, 연결 해제 시 모터 정지 명령을 시도합니다.
- 우측 수동 명령 창은 Lite에서 지원하는 명령 접두어만 허용합니다.

## 원본 릴 사진 텍스처 복원

`ImuOrientationView`와 3D 메시 렌더링 소스는 복원했습니다. 원본 이미지 두 개를 사용할 경우 다음 경로에 복사하면 빌드 시 자동 포함됩니다.

- `testModule/Assets/reel-top-v2.png`
- `testModule/Assets/reel-bottom.png`

원본 저장소의 두 파일을 쉽게 복사하려면 `powershell -ExecutionPolicy Bypass -File .\tools\restore-tm-assets.ps1`을 실행하십시오. 원본을 로컬에서 찾을 수 없다면 해당 스크립트가 GitHub 공개 원본 경로에서 내려받습니다. 이미지가 없어도 3D 형상 렌더링은 작동하도록 설계돼 있으나 원본 사진 텍스처는 표시되지 않습니다.

## 실행

Visual Studio 2022에서 `testModule/DFTestModule.sln` 또는 루트 `DF_Firmware.sln`을 열어 `DFTestModuleLite`을 선택할 수 있습니다. `.NET 8 Windows Forms x64` 및 `System.IO.Ports`가 사용됩니다.

이번 변경은 **빌드·실행·장비 검증을 수행하지 않은 소스 복원**입니다. 모터 구동 전 부하·정지 경로를 확인하십시오.
