# 🍀 lucky_kernel
> KR260 FPGA 기반 **로직 분석기** 프로젝트

최종 프로젝트 수행 기간 (26.11.12. ~ 26.12.16.)  
깃 개설일 26.10.27

## 📌 프로젝트 개요

| 항목 | 내용 |
|---|---|
| 팀명 | Lucky_Kernel |
| 주제 | 로직 분석기 |
| 수행 기간 | 2026.11.12 ~ 2026.12.18 |
| 저장소 개설일 | 2026.10.27 |
| 일정 및 업무 관리 | Jira |

## 💻 개발 환경

### 하드웨어 / OS

| 구분 | 버전 |
|---|---|
| FPGA 보드 | Xilinx KR260 (Boot FW 1.07 / 2026.1) |
| OS (KR260) | Ubuntu 24.04.2 LTS |
| Linux Kernel | 6.8.0-1015-xilinx |
| 개발 PC | Windows + WSL2 |

### 툴

| 구분 | 툴 |
|---|---|
| FPGA 설계 | Vivado 2026.1 |
| 임베디드 SW | Vitis 2026.1 |
| 원격 접속 | MobaXterm, SSH |

---


## 개인 계정에서 개발하되 깃 저장소 관리는 공용 그룹 lucky

## 🔧 개발 방식

| 종류 | 작성 | 빌드 | 실행 |
|---|---|---|---|
| Verilog/SystemVerilog | Windows | Vivado | KR260 FPGA |
| Device Tree | 본인편한툴 | **KR260** | KR260 |
| Kernel Driver | 본인편한툴 | **KR260** | KR260 |
| DMA/MMIO 테스트 | 본인편한툴 | **KR260** | KR260 |
| TCP Server | 본인편한툴 | **KR260** | KR260 |
| Diagnostic Console | **WSL** | KR260 | KR260 |
| Qt Viewer | Windows | Windows | Windows |
| STM32 | Windows | Windows | STM32 |

## 🐞 이슈 작성 방법

제목: [BUG] AXI DMA 전송 시 interrupt 발생하지 않음  
현상: DMA transfer 시작 후 IRQ handler가 호출되지 않음  
기대 동작: DMA 완료 후 IRQ 발생  
재현 방법: dma_test 실행 → transfer start  
로그: dmesg 내용 첨부(이미지)  
확인한 것: MMIO register 값 정상, DT의 IRQ 번호 확인 필요  
담당: 홍길동  


## 🌿 Git 및 PR 관리

### PR 규칙
- **제목**: `작업 기간 + 작업명 + 작업자 가능한 한 포함`
- **본문**: 변경 목적과 주요 변경 내용
- **검증**: 검증 방법과 결과

```text
26/11/03 ~ 26/11/03 메이크파일 수정 - 이양배
```

### Commit 규칙
- 메시지는 `영역: 변경 내용` 형식으로 작성

```bash
git commit -m "web: 복약 일정 중복 등록 방지"
git commit -m "uart: RESULT 프레임 오류 처리 추가"
git commit -m "motor: X축 원점 복귀 로직 수정"
```
