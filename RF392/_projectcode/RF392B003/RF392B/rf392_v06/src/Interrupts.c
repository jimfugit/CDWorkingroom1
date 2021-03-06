//=========================================================
// src/Interrupts.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================

// USER INCLUDES
#include <SI_EFM8SB1_Register_Enums.h>

#include "USER_PROGRAM.h"
#include "hgsemi74hc.h"
#include "board_efm8sb.h"
#include "pwr.h"
extern unsigned char xdata STANDBY_TIME;
unsigned char xdata timertoggleflag;
unsigned char xdata sleeptimer; //

//-----------------------------------------------------------------------------
// TIMER2_ISR
//-----------------------------------------------------------------------------
//
// TIMER2 ISR Content goes here. Remember to clear flag bits:
// TMR2CN0::TF2H (Timer # High Byte Overflow Flag)
// TMR2CN0::TF2L (Timer # Low Byte Overflow Flag)
//
//-----------------------------------------------------------------------------
#ifdef TIMER2_INT
SI_INTERRUPT (TIMER2_ISR, TIMER2_IRQn)
  {
  TMR2CN0_TF2H = 0;                           // Reset Interrupt
    /*if(!timertoggleflag)
     {
     //user_led1_litup();
     LED1TURNON();
     }
     else
     {
     //user_led1_litoff();
     LEDALL_OFF();
     }
     timertoggleflag = ~timertoggleflag;*/
  /*INDICADE_PIN = !INDICADE_PIN;
    sleeptimer++;
    if(sleeptimer >= 83)
      {
        //SEMI74HC_SCK = !SEMI74HC_SCK;
        sleeptimer = 0;
        if(STANDBY_TIME)
          {
            STANDBY_TIME--;
          }
      }
    user_timer_process();*/

  }
#endif

//-----------------------------------------------------------------------------
// TIMER2_ISR
//-----------------------------------------------------------------------------
//
// TIMER2 ISR Content goes here. Remember to clear flag bits:
// TMR2CN0::TF2H (Timer # High Byte Overflow Flag)
// TMR2CN0::TF2L (Timer # Low Byte Overflow Flag)
//
//-----------------------------------------------------------------------------
SI_INTERRUPT (TIMER2_ISR, TIMER2_IRQn)
  {
    TMR2CN0_TF2H = 0;
    //INDICADE_PIN = !INDICADE_PIN;
    user_timer_process();
  }

//-----------------------------------------------------------------------------
// VDDMON_ISR
//-----------------------------------------------------------------------------
//
// VDDMON ISR Content goes here. Remember to clear flag bits:
// VDM0CN::VDDOK (V<subscript>DD</subscript> Supply Status (Early Warning))
//
//-----------------------------------------------------------------------------
/*SI_INTERRUPT (VDDMON_ISR, VDDMON_IRQn)
  {
  VDM0CN &= 0xdf;//clr

  //PWR_enterSleep(PWR_PMAT_WF);
  }*/

