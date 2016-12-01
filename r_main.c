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
* File Name    : r_main.c
* Version      : CodeGenerator for RL78/L12 V2.03.03.03 [07 Mar 2016]
* Device(s)    : R5F10RLC
* Tool-Chain   : CA78K0R
* Description  : This file implements main function.
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
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_intc.h"
#include "r_cg_adc.h"
#include "r_cg_timer.h"
#include "r_cg_rtc.h"
#include "r_cg_lcd.h"
/* Start user code for include. Do not edit comment generated here */
#include "lcd_panel.h"
#include "pfdl.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
volatile unsigned short g_sw1_period = 0u,g_sw2_period = 0u;
uint8_t g_sw1_pressed =0,g_sw1_hold=0,g_sw2_pressed=0,g_sw2_hold=0,g_sw3_pressed=0,g_sw3_hold=0;
extern uint8_t g_sw1_enable_sample,g_sw2_enable_sample ;
extern void FlashMemoryAccess (void);
    char *res = "99.9";
    float f_k;
    // int8_t  g_buffer[12] = "80.9";
/* End user code. Do not edit comment generated here */
void R_MAIN_UserInit(void);

/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    R_MAIN_UserInit();
    /* Start user code. Do not edit comment generated here */
    /* Initialize the LCD panel */

    R_INTC3_Start();
    STOP();
    R_TAU0_Channel0_Start();
    R_TAU0_Channel1_Start();
    R_TAU0_Channel2_Start();
    R_INTC0_Start();
    R_INTC4_Start();
    R_RTC_Start();
    R_ADC_Start();
    Init_Display_Panel(); 
    
    //GTC_DISPLAY();
    while (1U)
    {
        if (g_sw1_pressed)
	{
		g_sw1_pressed =0;
		P13.0=~P13.0;
	}
	if (g_sw1_hold) /* SW1 hold */
	{
			g_sw1_hold =0;
			P2.1=~P2.1;		
				
	}
	
		//P12.5=~P12.5;;
		
    }
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MAIN_UserInit(void)
{
    /* Start user code. Do not edit comment generated here */
    //FlashMemoryAccess(); //Benson
    EI();
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
