//=========================================================
// src/InitDevice.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================

// USER INCLUDES
#include <SI_EFM8SB1_Register_Enums.h>
#include "InitDevice.h"

// USER PROTOTYPES
// USER FUNCTIONS

// $[Library Includes]

#include "cslib.h"
// [Library Includes]$

//==============================================================================
// enter_DefaultMode_from_RESET
//==============================================================================
extern void
enter_DefaultMode_from_RESET (void)
{
  // $[Config Calls]
  // Save the SFRPAGE
  uint8_t SFRPAGE_save = SFRPAGE;
  PCA_0_enter_DefaultMode_from_RESET ();
  PCACH_2_enter_DefaultMode_from_RESET ();
  VREG_0_enter_DefaultMode_from_RESET ();
  PORTS_0_enter_DefaultMode_from_RESET ();
  PORTS_1_enter_DefaultMode_from_RESET ();
  PBCFG_0_enter_DefaultMode_from_RESET ();
  VDDMON_0_enter_DefaultMode_from_RESET ();
  RSTSRC_0_enter_DefaultMode_from_RESET ();
  HFOSC_0_enter_DefaultMode_from_RESET ();
  RTC_0_enter_DefaultMode_from_RESET ();
  CLOCK_0_enter_DefaultMode_from_RESET ();
  TIMER16_2_enter_DefaultMode_from_RESET ();
  CSLIB_0_enter_DefaultMode_from_RESET ();
  INTERRUPT_0_enter_DefaultMode_from_RESET ();
  // Restore the SFRPAGE
  SFRPAGE = SFRPAGE_save;
  // [Config Calls]$

}

//================================================================================
// PORTS_1_enter_DefaultMode_from_RESET
//================================================================================
extern void
PORTS_1_enter_DefaultMode_from_RESET (void)
{
  // $[P1 - Port 1 Pin Latch]
  // [P1 - Port 1 Pin Latch]$

  // $[P1MDOUT - Port 1 Output Mode]
  /***********************************************************************
   - P1.0 output is open-drain
   - P1.1 output is open-drain
   - P1.2 output is push-pull
   - P1.3 output is push-pull
   - P1.5 output is push-pull
   - P1.6 output is push-pull
   - P1.7 output is push-pull
   ***********************************************************************/
  P1MDOUT = P1MDOUT_B0__OPEN_DRAIN | P1MDOUT_B1__OPEN_DRAIN
      | P1MDOUT_B2__PUSH_PULL | P1MDOUT_B3__PUSH_PULL | P1MDOUT_B5__PUSH_PULL
      | P1MDOUT_B6__PUSH_PULL | P1MDOUT_B7__PUSH_PULL;
  // [P1MDOUT - Port 1 Output Mode]$

  // $[P1MDIN - Port 1 Input Mode]
  /***********************************************************************
   - P1.0 pin is configured for analog mode
   - P1.1 pin is configured for analog mode
   - P1.2 pin is configured for digital mode
   - P1.3 pin is configured for digital mode
   - P1.5 pin is configured for digital mode
   - P1.6 pin is configured for digital mode
   - P1.7 pin is configured for digital mode
   ***********************************************************************/
  P1MDIN = P1MDIN_B0__ANALOG | P1MDIN_B1__ANALOG | P1MDIN_B2__DIGITAL
      | P1MDIN_B3__DIGITAL | P1MDIN_B5__DIGITAL | P1MDIN_B6__DIGITAL
      | P1MDIN_B7__DIGITAL;
  // [P1MDIN - Port 1 Input Mode]$

  // $[P1SKIP - Port 1 Skip]
  /***********************************************************************
   - P1.0 pin is skipped by the crossbar
   - P1.1 pin is skipped by the crossbar
   - P1.2 pin is skipped by the crossbar
   - P1.3 pin is not skipped by the crossbar
   - P1.5 pin is not skipped by the crossbar
   - P1.6 pin is not skipped by the crossbar
   - P1.7 pin is not skipped by the crossbar
   ***********************************************************************/
  P1SKIP = P1SKIP_B0__SKIPPED | P1SKIP_B1__SKIPPED | P1SKIP_B2__SKIPPED
      | P1SKIP_B3__NOT_SKIPPED | P1SKIP_B5__NOT_SKIPPED | P1SKIP_B6__NOT_SKIPPED
      | P1SKIP_B7__NOT_SKIPPED;
  // [P1SKIP - Port 1 Skip]$

  // $[Missing Pin Skip]
  // Skip any pins that are not available on this package
  P1SKIP |= P1SKIP_B4__SKIPPED;
  // [Missing Pin Skip]$

  // $[P1MASK - Port 1 Mask]
  // [P1MASK - Port 1 Mask]$

  // $[P1MAT - Port 1 Match]
  // [P1MAT - Port 1 Match]$

  // $[P1DRV - Port 1 Drive Strength]
  // [P1DRV - Port 1 Drive Strength]$

}

extern void
CLOCK_0_enter_DefaultMode_from_RESET (void)
{

  // $[CLKSEL - Clock Select]
  /***********************************************************************
   - SYSCLK is equal to selected clock source divided by 1
   - Clock derived from the internal precision High-Frequency Oscillator
   ***********************************************************************/
  CLKSEL = CLKSEL_CLKDIV__SYSCLK_DIV_1 | CLKSEL_CLKSL__HFOSC;
  // Wait for the clock to be ready
  while ((CLKSEL & CLKSEL_CLKRDY__BMASK) != CLKSEL_CLKRDY__SET)
    ;
  // [CLKSEL - Clock Select]$

}

extern void
VREG_0_enter_DefaultMode_from_RESET (void)
{

  // $[REG0CN - Voltage Regulator Control]
  /***********************************************************************
   - Enable the precision High Frequency Oscillator bias
   ***********************************************************************/
  REG0CN = REG0CN_OSCBIAS__ENABLED;
  // [REG0CN - Voltage Regulator Control]$

}

extern void
PBCFG_0_enter_DefaultMode_from_RESET (void)
{

  // $[XBR2 - Port I/O Crossbar 2]
  /***********************************************************************
   - Weak Pullups enabled 
   - Crossbar enabled
   ***********************************************************************/
  XBR2 = XBR2_WEAKPUD__PULL_UPS_ENABLED | XBR2_XBARE__ENABLED;
  // [XBR2 - Port I/O Crossbar 2]$

  // $[XBR0 - Port I/O Crossbar 0]
  // [XBR0 - Port I/O Crossbar 0]$

  // $[XBR1 - Port I/O Crossbar 1]
  // [XBR1 - Port I/O Crossbar 1]$

}

extern void
HFOSC_0_enter_DefaultMode_from_RESET (void)
{

  // $[HFO#CAL - High Frequency Oscillator Calibration]
  // [HFO#CAL - High Frequency Oscillator Calibration]$

  // $[HFO#CN - High Frequency Oscillator Control]
  /***********************************************************************
   - High Frequency Oscillator enabled
   ***********************************************************************/
  HFO0CN |= HFO0CN_IOSCEN__ENABLED;
  // [HFO#CN - High Frequency Oscillator Control]$

  // $[Oscillator Ready]
  while ((HFO0CN & HFO0CN_IFRDY__BMASK) == HFO0CN_IFRDY__NOT_SET)
    ;
  // [Oscillator Ready]$

}

extern void
RTC_0_enter_DefaultMode_from_RESET (void)
{

  // $[RTC Initialization]
  // [RTC Initialization]$

  // $[RTC0XCN0 - RTC Oscillator Control: Initial start-up setting]
  // [RTC0XCN0 - RTC Oscillator Control: Initial start-up setting]$

  // $[RTC0XCN - RTC Oscillator Control]
  /***********************************************************************
   - Self-Oscillate Mode selected
   - Disable AGC
   - Enable the Bias Double feature
   - LFOSC0 enabled and selected as RTC oscillator source
   ***********************************************************************/
  RTC0ADR = RTC0XCN0;
  RTC0DAT = RTC0XCN0_XMODE__SELF_OSCILLATE | RTC0XCN0_AGCEN__DISABLED
      | RTC0XCN0_BIASX2__ENABLED | RTC0XCN0_LFOEN__ENABLED;
  while ((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET)
    ;    //Poll Busy Bit
  // [RTC0XCN - RTC Oscillator Control]$

  // $[RTC0XCF - RTC Oscillator Configuration]
  // [RTC0XCF - RTC Oscillator Configuration]$

  // $[CAPTURE0 - RTC Timer Capture 0]
  // [CAPTURE0 - RTC Timer Capture 0]$

  // $[CAPTURE1 - RTC Timer Capture 1]
  // [CAPTURE1 - RTC Timer Capture 1]$

  // $[CAPTURE2 - RTC Timer Capture 2]
  // [CAPTURE2 - RTC Timer Capture 2]$

  // $[CAPTURE3 - RTC Timer Capture 3]
  // [CAPTURE3 - RTC Timer Capture 3]$

  // $[ALARM0 - RTC Alarm Programmed Value 0]
  /***********************************************************************
   - RTC Alarm Programmed Value 0 = 0x14
   ***********************************************************************/
  RTC0ADR = ALARM0;
  RTC0DAT = (0x14 << ALARM0_ALARM0__SHIFT);
  while ((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET)
    ;    //Poll Busy Bit
  // [ALARM0 - RTC Alarm Programmed Value 0]$

  // $[ALARM1 - RTC Alarm Programmed Value 1]
  // [ALARM1 - RTC Alarm Programmed Value 1]$

  // $[ALARM2 - RTC Alarm Programmed Value 2]
  // [ALARM2 - RTC Alarm Programmed Value 2]$

  // $[ALARM3 - RTC Alarm Programmed Value 3]
  // [ALARM3 - RTC Alarm Programmed Value 3]$

  // $[RTC0CN - RTC Control]
  /***********************************************************************
   - Enable RTC oscillator
   - RTC timer is stopped
   - Disable missing RTC detector
   - Disable RTC alarm
   - Alarm event flag is not set or disable the auto reset function
   - Do not start a capture operation
   - Do not start a set operation
   ***********************************************************************/
  RTC0ADR = RTC0CN0;
  RTC0DAT = RTC0CN0_RTC0EN__ENABLED | RTC0CN0_RTC0TR__STOP
      | RTC0CN0_MCLKEN__DISABLED | RTC0CN0_RTC0AEN__DISABLED
      | RTC0CN0_ALRM__NOT_SET | RTC0CN0_RTC0CAP__NOT_SET
      | RTC0CN0_RTC0SET__NOT_SET;
  while ((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET)
    ;    //Poll Busy Bit

  // [RTC0CN - RTC Control]$

  // $[External Oscillator Setup]
  // [External Oscillator Setup]$

}

extern void
CSLIB_0_enter_DefaultMode_from_RESET (void)
{

  // $[CSLIB Init]

  // -----------------------------------------------------------------------------
  // Configures all peripherals controlled by capsense, including the sensing block
  // and port pins
  // -----------------------------------------------------------------------------
  CSLIB_initHardware ();

  // -----------------------------------------------------------------------------
  // Initializes capsense variables and performs some scans to initialize the
  // baselines
  // -----------------------------------------------------------------------------
  CSLIB_initLibrary ();

  // [CSLIB Init]$

}

extern void
PORTS_0_enter_DefaultMode_from_RESET (void)
{

  // $[P0 - Port 0 Pin Latch]
  // [P0 - Port 0 Pin Latch]$

  // $[P0MDOUT - Port 0 Output Mode]
  /***********************************************************************
   - P0.0 output is open-drain
   - P0.1 output is open-drain
   - P0.2 output is open-drain
   - P0.3 output is open-drain
   - P0.4 output is push-pull
   - P0.5 output is push-pull
   - P0.6 output is push-pull
   - P0.7 output is open-drain
   ***********************************************************************/
  P0MDOUT = P0MDOUT_B0__OPEN_DRAIN | P0MDOUT_B1__OPEN_DRAIN
      | P0MDOUT_B2__OPEN_DRAIN | P0MDOUT_B3__OPEN_DRAIN | P0MDOUT_B4__PUSH_PULL
      | P0MDOUT_B5__PUSH_PULL | P0MDOUT_B6__PUSH_PULL | P0MDOUT_B7__OPEN_DRAIN;
  // [P0MDOUT - Port 0 Output Mode]$

  // $[P0MDIN - Port 0 Input Mode]
  /***********************************************************************
   - P0.0 pin is configured for analog mode
   - P0.1 pin is configured for analog mode
   - P0.2 pin is configured for analog mode
   - P0.3 pin is configured for analog mode
   - P0.4 pin is configured for digital mode
   - P0.5 pin is configured for digital mode
   - P0.6 pin is configured for digital mode
   - P0.7 pin is configured for analog mode
   ***********************************************************************/
  P0MDIN = P0MDIN_B0__ANALOG | P0MDIN_B1__ANALOG | P0MDIN_B2__ANALOG
      | P0MDIN_B3__ANALOG | P0MDIN_B4__DIGITAL | P0MDIN_B5__DIGITAL
      | P0MDIN_B6__DIGITAL | P0MDIN_B7__ANALOG;
  // [P0MDIN - Port 0 Input Mode]$

  // $[P0SKIP - Port 0 Skip]
  /***********************************************************************
   - P0.0 pin is skipped by the crossbar
   - P0.1 pin is skipped by the crossbar
   - P0.2 pin is skipped by the crossbar
   - P0.3 pin is skipped by the crossbar
   - P0.4 pin is not skipped by the crossbar
   - P0.5 pin is not skipped by the crossbar
   - P0.6 pin is not skipped by the crossbar
   - P0.7 pin is skipped by the crossbar
   ***********************************************************************/
  P0SKIP = P0SKIP_B0__SKIPPED | P0SKIP_B1__SKIPPED | P0SKIP_B2__SKIPPED
      | P0SKIP_B3__SKIPPED | P0SKIP_B4__NOT_SKIPPED | P0SKIP_B5__NOT_SKIPPED
      | P0SKIP_B6__NOT_SKIPPED | P0SKIP_B7__SKIPPED;
  // [P0SKIP - Port 0 Skip]$

  // $[P0MASK - Port 0 Mask]
  // [P0MASK - Port 0 Mask]$

  // $[P0MAT - Port 0 Match]
  // [P0MAT - Port 0 Match]$

  // $[P0DRV - Port 0 Drive Strength]
  // [P0DRV - Port 0 Drive Strength]$

}

extern void
PCA_0_enter_DefaultMode_from_RESET (void)
{
  // $[PCA0MD - PCA Mode]
  /***********************************************************************
   - Disable Watchdog Timer
   - System clock divided by 12
   - PCA continues to function normally while the system controller is in
   Idle Mode
   - Disable the CF interrupt
   - Disable Watchdog Timer
   - Watchdog Timer Enable unlocked
   ***********************************************************************/
  SFRPAGE = 0x00;
  PCA0MD &= ~PCA0MD_WDTE__BMASK;
  PCA0MD = PCA0MD_CPS__SYSCLK_DIV_12 | PCA0MD_CIDL__NORMAL
      | PCA0MD_ECF__OVF_INT_DISABLED | PCA0MD_WDTE__DISABLED
      | PCA0MD_WDLCK__UNLOCKED;
  // [PCA0MD - PCA Mode]$

  // $[PCA0H - PCA Counter/Timer High Byte]
  // [PCA0H - PCA Counter/Timer High Byte]$

  // $[PCA0L - PCA Counter/Timer Low Byte]
  // [PCA0L - PCA Counter/Timer Low Byte]$

  // $[PCA0PWM - PCA PWM Configuration]
  // [PCA0PWM - PCA PWM Configuration]$

  // $[PCA0CN0 - PCA Control 0]
  // [PCA0CN0 - PCA Control 0]$

}

extern void
PCACH_2_enter_DefaultMode_from_RESET (void)
{
  // $[PCA0 Settings Save]
  // Select Capture/Compare register)
  PCA0PWM &= ~PCA0PWM_ARSEL__BMASK;
  // [PCA0 Settings Save]$

  // $[PCA0CPM2 - PCA Channel 2 Capture/Compare Mode]
  /***********************************************************************
   - Disable negative edge capture
   - Disable CCF2 interrupts
   - Enable match function
   - 8 to 11-bit PWM selected
   - Disable positive edge capture
   - Disable comparator function
   - Disable PWM function
   - Disable toggle function
   ***********************************************************************/
  PCA0CPM2 = PCA0CPM2_CAPN__DISABLED | PCA0CPM2_ECCF__DISABLED
      | PCA0CPM2_MAT__ENABLED | PCA0CPM2_PWM16__8_BIT | PCA0CPM2_CAPP__DISABLED
      | PCA0CPM2_ECOM__DISABLED | PCA0CPM2_PWM__DISABLED
      | PCA0CPM2_TOG__DISABLED;
  // [PCA0CPM2 - PCA Channel 2 Capture/Compare Mode]$

  // $[PCA0CPL2 - PCA Channel 2 Capture Module Low Byte]
  // [PCA0CPL2 - PCA Channel 2 Capture Module Low Byte]$

  // $[PCA0CPH2 - PCA Channel 2 Capture Module High Byte]
  PCA0CPH2 = 0x00;
  // [PCA0CPH2 - PCA Channel 2 Capture Module High Byte]$

  // $[Auto-reload]
  // [Auto-reload]$

  // $[PCA0 Settings Restore]
  // [PCA0 Settings Restore]$

}

extern void
TIMER_SETUP_0_enter_DefaultMode_from_RESET (void)
{
  // $[CKCON0 - Clock Control 0]
  /***********************************************************************
   - System clock divided by 12
   - Counter/Timer 0 uses the clock defined by the prescale field, SCA
   - Timer 2 high byte uses the clock defined by T2XCLK in TMR2CN0
   - Timer 2 low byte uses the clock defined by T2XCLK in TMR2CN0
   - Timer 3 high byte uses the clock defined by T3XCLK in TMR3CN0
   - Timer 3 low byte uses the system clock
   - Timer 1 uses the clock defined by the prescale field, SCA
   ***********************************************************************/
  CKCON0 = CKCON0_SCA__SYSCLK_DIV_12 | CKCON0_T0M__PRESCALE
      | CKCON0_T2MH__EXTERNAL_CLOCK | CKCON0_T2ML__EXTERNAL_CLOCK
      | CKCON0_T3MH__EXTERNAL_CLOCK | CKCON0_T3ML__SYSCLK
      | CKCON0_T1M__PRESCALE;
  // [CKCON0 - Clock Control 0]$

  // $[TMOD - Timer 0/1 Mode]
  // [TMOD - Timer 0/1 Mode]$

  // $[TCON - Timer 0/1 Control]
  // [TCON - Timer 0/1 Control]$

}

extern void
INTERRUPT_0_enter_DefaultMode_from_RESET (void)
{
  // $[EIE1 - Extended Interrupt Enable 1]
  // [EIE1 - Extended Interrupt Enable 1]$

  // $[EIP1 - Extended Interrupt Priority 1]
  // [EIP1 - Extended Interrupt Priority 1]$

  // $[IE - Interrupt Enable]
  /***********************************************************************
   - Enable each interrupt according to its individual mask setting
   - Disable external interrupt 0
   - Disable external interrupt 1
   - Disable all SPI0 interrupts
   - Disable all Timer 0 interrupt
   - Disable all Timer 1 interrupt
   - Enable interrupt requests generated by the TF2L or TF2H flags
   - Disable UART0 interrupt
   ***********************************************************************/
  IE = IE_EA__ENABLED | IE_EX0__DISABLED | IE_EX1__DISABLED | IE_ESPI0__DISABLED
      | IE_ET0__DISABLED | IE_ET1__DISABLED | IE_ET2__ENABLED
      | IE_ES0__DISABLED;
  // [IE - Interrupt Enable]$

  // $[IP - Interrupt Priority]
  // [IP - Interrupt Priority]$

  // $[EIE2 - Extended Interrupt Enable 2]
  // [EIE2 - Extended Interrupt Enable 2]$

  // $[EIP2 - Extended Interrupt Priority 2]
  // [EIP2 - Extended Interrupt Priority 2]$

}

extern void
TIMER01_0_enter_DefaultMode_from_RESET (void)
{
  // $[Timer Initialization]
  //Save Timer Configuration
  uint8_t TCON_save;
  TCON_save = TCON;
  //Stop Timers
  TCON &= ~TCON_TR0__BMASK & ~TCON_TR1__BMASK;

  // [Timer Initialization]$

  // $[TH0 - Timer 0 High Byte]
  /***********************************************************************
   - Timer 0 High Byte = 0x40
   ***********************************************************************/
  TH0 = (0x40 << TH0_TH0__SHIFT);
  // [TH0 - Timer 0 High Byte]$

  // $[TL0 - Timer 0 Low Byte]
  /***********************************************************************
   - Timer 0 Low Byte = 0x9C
   ***********************************************************************/
  TL0 = (0x9C << TL0_TL0__SHIFT);
  // [TL0 - Timer 0 Low Byte]$

  // $[TH1 - Timer 1 High Byte]
  /***********************************************************************
   - Timer 1 High Byte = 0x40
   ***********************************************************************/
  TH1 = (0x40 << TH1_TH1__SHIFT);
  // [TH1 - Timer 1 High Byte]$

  // $[TL1 - Timer 1 Low Byte]
  /***********************************************************************
   - Timer 1 Low Byte = 0x9C
   ***********************************************************************/
  TL1 = (0x9C << TL1_TL1__SHIFT);
  // [TL1 - Timer 1 Low Byte]$

  // $[Timer Restoration]
  //Restore Timer Configuration
  TCON |= (TCON_save & TCON_TR0__BMASK) | (TCON_save & TCON_TR1__BMASK);

  // [Timer Restoration]$

}

extern void
FLASH_0_enter_DefaultMode_from_RESET (void)
{

  // $[FLSCL - Flash Scale]
  /***********************************************************************
   - The system clock determines the flash read time
   ***********************************************************************/
  FLSCL = FLSCL_BYPASS__SYSCLK;
  // [FLSCL - Flash Scale]$

}

extern void
TIMER16_2_enter_DefaultMode_from_RESET (void)
{
  // $[Timer Initialization]
  // Save Timer Configuration
  uint8_t TMR2CN0_TR2_save;
  TMR2CN0_TR2_save = TMR2CN0 & TMR2CN0_TR2__BMASK;
  // Stop Timer
  TMR2CN0 &= ~(TMR2CN0_TR2__BMASK);
  // [Timer Initialization]$

  // $[TMR2CN0 - Timer 2 Control]
  // [TMR2CN0 - Timer 2 Control]$

  // $[TMR2H - Timer 2 High Byte]
  /***********************************************************************
   - Timer 2 High Byte = 0xBC
   ***********************************************************************/
  TMR2H = (0xBC << TMR2H_TMR2H__SHIFT);
  // [TMR2H - Timer 2 High Byte]$

  // $[TMR2L - Timer 2 Low Byte]
  /***********************************************************************
   - Timer 2 Low Byte = 0x6A
   ***********************************************************************/
  TMR2L = (0x6A << TMR2L_TMR2L__SHIFT);
  // [TMR2L - Timer 2 Low Byte]$

  // $[TMR2RLH - Timer 2 Reload High Byte]
  /***********************************************************************
   - Timer 2 Reload High Byte = 0xD8
   ***********************************************************************/
  TMR2RLH = (0xD8 << TMR2RLH_TMR2RLH__SHIFT);
  // [TMR2RLH - Timer 2 Reload High Byte]$

  // $[TMR2RLL - Timer 2 Reload Low Byte]
  /***********************************************************************
   - Timer 2 Reload Low Byte = 0x20
   ***********************************************************************/
  TMR2RLL = (0x20 << TMR2RLL_TMR2RLL__SHIFT);
  // [TMR2RLL - Timer 2 Reload Low Byte]$

  // $[TMR2CN0]
  // [TMR2CN0]$

  // $[Timer Restoration]
  // Restore Timer Configuration
  TMR2CN0 |= TMR2CN0_TR2_save;
  // [Timer Restoration]$

}

extern void
TIMER16_3_enter_DefaultMode_from_RESET (void)
{

}

extern void
VDDMON_0_enter_DefaultMode_from_RESET (void)
{

  // $[VDM0CN - VDD Supply Monitor Control]
  /***********************************************************************
   - Disable the VDD Early Warning interrupt
   ***********************************************************************/
  VDM0CN &= ~VDM0CN_VDDOKIE__BMASK;
  // [VDM0CN - VDD Supply Monitor Control]$

}

extern void
RSTSRC_0_enter_DefaultMode_from_RESET (void)
{

  // $[RSTSRC - Reset Source]
  /***********************************************************************
   - A power-on or supply monitor reset occurred
   - A missing clock detector reset did not occur
   - A Comparator 0 reset did not occur
   - A RTC alarm or oscillator fail reset did not occur
   ***********************************************************************/
  RSTSRC = RSTSRC_PORSF__SET | RSTSRC_MCDRSF__NOT_SET | RSTSRC_C0RSEF__NOT_SET
      | RSTSRC_RTC0RE__NOT_SET;
  // [RSTSRC - Reset Source]$

}

