# DF Main/Rod protocol contract

Updated: 2026-10-20

## Wire format

ESP-NOW application message는 ASCII 두 자리 PID 뒤에 payload를 이어 붙인다. delimiter와 terminating NUL은 무선 전송 길이에 포함하지 않는다. 현재 고정 송신 버퍼는 128바이트이므로 계약상 wire 길이는 최대 127바이트다.

```text
byte 0..1 : decimal PID (`00`..`99`)
byte 2..N : command-specific payload
```

## Main to Rod

| PID | 이름 | 현재 payload |
| ---: | --- | --- |
| 05 | PowerOn | 현재 상태 문자열 |
| 07 | RodInfoRequest | 없음 |
| 09 | Alive/SleepCheck | 없음 또는 sleep mode 한 자리 |
| 11 | Break | 기존 break 제어 문자열 |
| 13 | ImuDataControl | IMU 출력 제어 문자열 |
| 19 | VerticalMotor | count/on/off 제어 문자열 |
| 21 | VersionRead | 없음 |
| 27 | ButtonLed | 위치/동작/count/on/off 문자열 |
| 29 | AllOutputsOff | `00` |
| 31 | MainAddress | MAC address 문자열 |
| 33 | ApInfo | AP/Main 정보 문자열 |
| 34 | Sleep | sleep enable 설정 문자열 |
| 35 | EncoderCalibration | 단일 동작 문자: `S` 시작, `C` 측정 완료, `A` 적용/저장, `X` 취소, `Q` 상태 조회 |
| 37 | ImuCalibration | `C`: 정지 상태를 기다린 뒤 자이로 보정 요청 |

## Rod to Main

| PID | 이름 | 현재 payload |
| ---: | --- | --- |
| 02 | BoardType | board 정보 문자열 |
| 10 | Alive | 없음 |
| 14 | ImuData | IMU 측정 문자열 |
| 16 | Button | button 상태 문자열 |
| 18 | Encoder | encoder count 문자열 |
| 22 | Version | Rod 및 IMU version 문자열 |
| 24 | ImuConnection | IMU 연결 상태 문자열 |
| 26 | Battery | 3자리 battery level 문자열 |
| 28 | BatteryDiagnostic | `현재ADC,20회평균ADC,ADC환산퍼센트,배터리요청반환퍼센트` 정수 필드; TM에서만 1초 주기 |
| 32 | RodAddress | MAC address 문자열 |
| 34 | Sleep | sleep/wakeup 상태 문자열 |
| 36 | EncoderCalibration | `상태,보정유효,보정중원시카운트,저장된회전당원시카운트` 필드 |
| 38 | ImuCalibration | 상태 문자: `W`, `R`, `S`, `M`, `T`, `E`, `B`, `U` |

PID 36 상태 문자는 `0` 대기, `1` 보정 중, `2` 저장 완료, `3` 무효 카운트, `4` Rod 저장 오류, `5` Rod 연결 안 됨, `6` 잘못된 동작, `7` 측정 완료/적용 대기다. 보정 유효 필드는 `0` 또는 `1`이다. 진행 중에는 원시 카운트가 바뀔 때 상태를 전송한다. `C`는 측정을 끝내고 임시 대기 상태로 만들며, `A`를 받아 파일 저장에 성공한 뒤에만 새 보정값을 적용한다. `X`는 측정 또는 적용 대기 결과를 버리고 이전 보정값을 유지한다. PID 35/36은 릴 엔코더 보정 전용이며 기존 PID 18 엔코더 표시 payload는 유지한다.

TM↔Main 시리얼에서는 `$37`을 사용한다. `$15`는 기존 장치 제어 요청에 예약되어 있다. TM 명령은 `$37S%`, `$37C%`, `$37A%`, `$37X%`, `$37Q%`이며, Main은 Rod 미연결/잘못된 동작을 포함해 `$37상태,보정유효,원시카운트,저장값%` 형식으로 응답한다. Rod의 PID 36 상태 payload는 Main이 `$37` 시리얼 상태 프레임으로 중계한다.

### 사용자 앱/TM 공통 IMU 자이로 보정

일반 사용자 앱과 TM은 모두 MAIN에 `$38C%`를 전송한다. MAIN은 ROD가 연결되고
활성 상태이며 IMU도 연결되어 있을 때에만 ESP-NOW PID 37/`C`로 전달한다.
ROD는 자이로 3축(GX/GY/GZ) 출력으로 충분한 정지 구간을 확인한 뒤
사용자에게 방향 회전을 요구하지 않는 `<cg>` 명령을 EBIMU에 전송한다.
MAIN은 ESP-NOW PID 38로 받은 상태를 `$38상태%`로 모든 AP 유형에 중계한다.

| 상태 | 의미 |
| --- | --- |
| `W` | 정지 판정 중(최대 12초) |
| `R` | EBIMU `<cg>` 보정 전송 후 응답 대기(최대 5초) |
| `S` | EBIMU의 `<ok>` 수신, 보정 성공 |
| `M` | 정지 감지 실패 또는 보정 중 움직임 감지 |
| `T` | 보정 완료 응답 타임아웃 |
| `E` | 잘못된 요청 또는 EBIMU 오류 |
| `B` | 이미 자이로 보정 진행 중 |
| `U` | ROD/IMU 미연결, 수면 모드 또는 등록 모드 |

정지 판정은 3축 각속도 절대값 ≤1.5°/s인 샘플 10개 이상과 2.2초 연속 안정화,
최신 데이터 500ms 이내라는 시작 기준을 사용한다. 실제 물리적 정지를 완전히
보장하는 판정은 아니다. `S` 응답은 IMU 자체의 보정 성공이며, 3D 화면의 0점
재설정과는 별개다. `<cmf>` 지자기 보정이나 EBIMU AGC/RHA 설정을 바꾸지 않는다.
센서 설정·지자기 왜곡으로 발생하는 Yaw Drift의 근본 해결 여부는 실장비에서
별도로 확인해야 한다.

기존 PID 번호, payload 조합 순서와 기존 문자열 형식은 변경하지 않는다. 코드 계약은 `libraries/DFProtocol/src/DFProtocol.h`가 단일 기준이다.

## Golden messages

| 방향 | 의미 | byte 문자열 |
| --- | --- | --- |
| Main → Rod | Rod 정보 요청 | `07` |
| Main → Rod | Alive | `09` |
| Main → Rod | 전체 출력 OFF | `2900` |
| Main → Rod | AP type 정보 | `330100` |
| Rod → Main | 엔코더 보정 상태 | `36상태,유효,원시카운트,회전당펄스` |
| TM → Main | 엔코더 보정 시작 | `$37S%` |
| TM → Main | 엔코더 측정 완료 | `$37C%` |
| TM → Main | 보정값 적용 | `$37A%` |
| Main → Rod | 엔코더 보정 시작 | `35S` |
| Main → Rod | 엔코더 보정 완료 | `35C` |
| Main → Rod | 엔코더 보정 취소 | `35X` |
| Main → Rod | 엔코더 보정값 조회 | `35Q` |
| Rod → Main | Alive 응답 | `10` |
| Rod → Main | Battery 95% 예시 | `2695%` |
| Rod → Main | TM battery diagnostic 예시 | `283520,3512,95,95` |
| Rod → Main | 보정 진행 중 원시 카운트 12, 저장값 미보정 기본 30 | `361,0,12,30` |
| Rod → Main | 보정 저장 완료, 회전당 원시 카운트 27 | `362,1,27,27` |

PID 28은 기존 PID 26과 역할을 분리한다. PID 26 payload와 전송 정책은 바꾸지 않는다. Main은 TM AP일 때에만 PID 28을 PC 시리얼 `$31` 응답으로 중계하며, 게임 프로그램(AP_IS_DF)에는 진단 프레임을 보내지 않는다. 세 번째 필드는 ADC 평균값을 기준점 2828~3546에서 선형으로 환산한 원값이다. 네 번째 필드는 Rod가 PID 26 배터리 payload로 보내는 값과 동일하며, TM 좌측 잔량 및 배터리 요청 응답과 일치시킨다. 따라서 ADC 환산값과 요청 반환값이 다를 수 있으며, 전자는 측정 분석용 참고값이다.

Golden 문자열은 현재 결합 규칙의 대표 fixture이며 실제 장비 결과로 검증하지 않은 상태다.
