# DF Lite Test Module

.NET 8 Windows Forms x64 최소 프로젝트. Main/Rod 하드웨어 및 펌웨어 프로토콜은 기존 규격을 사용합니다.

- Visual Studio: \`DFTestModule.sln\` 또는 루트 \`DF_Firmware.sln\`
- CLI (로컬): \`dotnet build testModule/DFTestModule.csproj -c Debug -p:Platform=x64\`
- 보조 스크립트: \`tools\build-testmodule.cmd Debug\` / \`tools\build-testmodule.cmd Release\`
- Release는 self-contained Windows x64 단일 EXE 게시를 목표로 함.
- 기본 화면: COM 연결/로그, 1초 제한 Main 모터, Rod 출력/IMU, 상태 프레임 수신.
- 새 펌웨어 기능, 업데이트 도구, 3D, 상세 진단은 없음.

**주의:** 1단계에서는 빌드·실행·장비/통신 검증을 하지 않았습니다. 프로토콜 수신/상태 표시와 정지 동작은 실장비에서 별도 확인이 필요합니다. Motor 테스트 전 기구 부하와 안전 정지를 확인하십시오.
