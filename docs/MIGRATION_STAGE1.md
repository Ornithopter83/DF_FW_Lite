# DF Firmware Lite — 1단계 독립 저장소 이관

작성일: 2026-10-19

## 기준 및 목적

- 원본 저장소: https://github.com/Ornithopter83/DF_FW_Code
- 이관 저장소: https://github.com/Ornithopter83/DF_FW_Lite
- 기준 원본 커밋: \`926c010b2f65263cfb96b207cf4ad57ddb2510ea\`
- 1단계 목표: 원본의 필수 구성요소를 독립 Git 저장소로 옮겨 향후 Lite 기능 축소의 출발점 만들기
- 이번 단계에서 Main/Rod FW 동작은 변경하지 않음. 현재 이관된 펌웨어는 **아직 경량화되지 않음**.

## 이관한 구성

| 경로 | 내용 |
| --- | --- |
| \`firmware/DF_Main/\` | 활성 Main 펌웨어 51개 파일 (내용 변경 없이 복사) |
| \`firmware/DF_Rod/\` | 활성 Rod 펌웨어 35개 파일 (내용 변경 없이 복사) |
| \`libraries/DFProtocol/\` | Main/Rod 공용 ESP-NOW 프로토콜 3개 파일 |
| \`vs/\` | VS2022 Main/Rod 프로젝트 및 환경 속성 4개 파일 (빌드 로그 제외) |
| \`DF_Firmware.sln\` | Main/Rod/TM 통합 솔루션 |
| \`global.json\` | 원본 SDK 고정값 |
| \`tools/\` | 원본 FW 빌드/정리용 필수 스크립트, Lite TM용 간단 스크립트 |
| \`testModule/\` | 원본 FrameDecoder·SerialConnection, 새 기본 WinForms/명령 프로젝트 |
| \`docs/protocol/\`, \`docs/hardware/\` | 통신·하드웨어 기준 문서 |

## TM 최소 범위

- \`testModule/DFTestModule.csproj\`: .NET 8 Windows Forms x64 독립 프로젝트.
- \`SerialConnection.cs\`, \`FrameDecoder.cs\`: 원본에서 이관.
- \`DeviceProtocol.cs\`, \`MainForm.cs\`, \`Program.cs\`: 경량 TM 시작점으로 새로 작성.
- 기본 UI: COM 연결/끊기, 연결 후 기존 단계별 초기화, 2초 상태 polling, 명령 송수신 로그, Torque/BLDC 1초 제한 구동과 정지, Rod 진동/좌우 LED, IMU on/off, 버전·상태 수신, 수동 프레임 전송.
- TM에 펌웨어 업데이트, OTA, 3D 렌더링, 장치 진단 상세 화면, 제조·설정·LM Jig UI는 포함하지 않음.
- TM은 아직 실제 장비에서 명령 및 화면 동작을 확인하지 않았음.

## 이관하지 않은 구성

\`legacy/\`, \`deprecated/\`, \`BoardWifiConnector/\`, 기존 TM의 대형 UI/3D/이미지/업데이트 코드, 과거 작업 일지, 오래된 바이너리, \`bin/\`, \`artifacts/\`, 로컬 \`toolchain/\`, VS 생성 로그, 비밀정보/개인 PC 설정.

## 로컬 준비 사항

1. \`git clone https://github.com/Ornithopter83/DF_FW_Lite.git\`
2. Visual Studio 2022 + C++ 빌드 지원 설치. \`DF_Firmware.sln\` 또는 \`testModule/DFTestModule.sln\` 열기.
3. .NET 8 SDK를 \`global.json\`과 맞추고, TM NuGet (\`System.IO.Ports\`) 복원 준비.
4. 원본과 같은 ESP32 Arduino Core 2.0.17 / Arduino CLI 환경 및 ESP32Servo를 **로컬** \`toolchain/\` 기준으로 준비. Arduino 설정 경로·드라이브 매핑은 \`tools/arduino-env.cmd\`와 \`tools/ensure-arduino-drive.ps1\` 참고.
5. 빌드 실행은 별도 승인 후에 진행. 현재 Main/Rod 바이너리, TM EXE는 생성하지 않았음.

## 다음 단계 (별도 작업)

1. Main의 게임/관리/스케줄러 의존성과 출력 안전 경로 분리.
2. Rod의 필수 입출력·통신 경계 확정, OTA·진단·보정 부가 기능 축소.
3. TM 기능별 실물 동작 확인 후 최소 명령/표시만 유지.
4. 컴파일과 안전 검증, 장비 실험은 각각 별도 승인 및 사용자 장비 진행.

**검증 상태**: 1단계 Git 파일 작성 작업만 수행했다. 컴파일/링크, 단위 테스트, 실행, COM/ESP-NOW, 플래시, 장비 검증은 미실행이다.
