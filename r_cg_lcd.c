/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2012, 2016 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_lcd.c
* Version      : CodeGenerator for RL78/L12 V2.03.03.03 [07 Mar 2016]
* Device(s)    : R5F10RLC
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for LCD module.
* Creation Date: 2016/11/18
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_lcd.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_LCD_Create
* Description  : This function initializes the LCD module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LCD_Create(void)
{
    volatile uint32_t wt_count;
    
    RTCEN = 1U;    /* supply LCD clock */
    LCDON = 0U;    /* disable LCD clock operation */
    LCDM1 |= _00_LCD_VOLTAGE_HIGH;
    LCDM0 = _00_LCD_DISPLAY_WAVEFORM_A | _0D_LCD_DISPLAY_MODE1;
    LCDM0 |= _40_LCD_VOLTAGE_MODE_INTERNAL;
    /* Set CAPL and CAPH pins */
    ISCLCD &= (uint8_t)~_01_LCD_CAPLH_BUFFER_VALID;
    P12 &= 0x3FU;
    PM12 |= 0xC0U;
    /* Set segment pins */
    PFSEG0 |= 0xF0U;
    PFSEG1 |= 0xFFU;
    PFSEG2 |= 0xF9U;
    PFSEG3 |= 0x9FU;
    PFSEG4 |= 0x7FU;
    POM1 &= 0x5EU;
    PIM1 &= 0x9EU;
    PU1 &= 0x06U;
    PMC1 &= 0xE7U;
    P1 &= 0x06U;
    PM1 &= 0x06U;
    PU3 &= 0xFEU;
    P3 &= 0xFEU;
    PM3 &= 0xFEU;
    PU4 &= 0xF1U;
    PMC4 &= 0xFDU;
    P4 &= 0xF1U;
    PM4 &= 0xF1U;
    PU5 &= 0xE0U;
    P5 &= 0xE0U;
    PM5 &= 0xE0U;
    P6 &= 0xFCU;
    PM6 &= 0xFCU;
    PU7 &= 0xE0U;
    P7 &= 0xE0U;
    PM7 &= 0xE0U;
    PU12 &= 0xFEU;
    PMC12 &= 0xFEU;
    P12 &= 0xFEU;
    PM12 &= 0xFEU;
    PU14 &= 0x00U;
    PMC14 &= 0xC3U;
    P14 &= 0x00U;
    PM14 &= 0x00U;
    LCDM1 |= _00_LCD_DISPLAY_PATTERN_A;
    LCDC0 = _07_LCD_CLOCK_FSUB_FIL_8;
    VLCD = _0E_LCD_BOOST_VOLTAGE_150V;

    /* Change the waiting time according to the system */
    for (wt_count = 0U; wt_count <= LCD_REFVOLTAGE_WAITTIME; wt_count++)
    {
        NOP();
    }
}

/***********************************************************************************************************************
* Function Name: R_LCD_Start
* Description  : This function enables the LCD display.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LCD_Start(void)
{
    LCDON = 1U;
}

/***********************************************************************************************************************
* Function Name: R_LCD_Stop
* Description  : This function disables the LCD display.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LCD_Stop(void)
{
    LCDON = 0U;
}

/***********************************************************************************************************************
* Function Name: R_LCD_Set_VoltageOn
* Description  : This function enables voltage boost circuit or capacitor split circuit.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LCD_Set_VoltageOn(void)
{
    volatile uint32_t wt_count;

    LCDM0 |= _40_LCD_VOLTAGE_MODE_INTERNAL;
    VLCON = 1U;

    /* Change the waiting time according to the system */
    for(wt_count = 0U; wt_count <= LCD_VOLTAGE_WAITTIME; wt_count++)
    {
        NOP();
    }
    
    SCOC = 1U;
}

/***********************************************************************************************************************
* Function Name: R_LCD_Set_VoltageOff
* Description  : This function disables voltage boost circuit or capacitor split circuit.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LCD_Set_VoltageOff(void)
{
    SCOC = 0U;
    VLCON = 0U;
    LCDM0 &= (uint8_t)~_C0_LCD_VOLTAGE_MODE_INITIALVALUE;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
