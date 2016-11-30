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
* File Name    : r_cg_timer_user.c
* Version      : CodeGenerator for RL78/L12 V2.03.03.03 [07 Mar 2016]
* Device(s)    : R5F10RLC
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for TAU module.
* Creation Date: 2016/11/18
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt INTTM00 r_tau0_channel0_interrupt
#pragma interrupt INTTM01 r_tau0_channel1_interrupt
#pragma interrupt INTTM02 r_tau0_channel2_interrupt
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_timer.h"
/* Start user code for include. Do not edit comment generated here */
#include "lcd_panel.h"
#include "r_cg_adc.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* String to be scrolled across the star-burst segments.
   Leave at least one space following the last alphabetic/numeric character in the string*/
char g_scroll_data[SCROLL_LINES][SCROLL_BUFF_SIZE] =
{
    " YRPB RL78/L12 Demo ",
    " Benson Go Go Go    ",
    "1234567890abcdefghi"
};
    

/* Pointer used to specify the start address of buffer 
arrays containing data to be scrolled on the LCD panel */
char * g_scroll_data_pointer = g_scroll_data[0];

/* Holds statuses for enabling/disabling text scrolling on the LCD panel */
uint8_t g_enable_scroll = 0u;
uint8_t g_sw1_enable_sample = 0u,g_sw2_enable_sample = 0u;
volatile uint8_t g_sw=0;
extern uint8_t g_sw1_hold,g_sw2_hold;
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_tau0_channel0_interrupt
* Description  : This function is INTTM00 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_tau0_channel0_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    /* 500ms */
	
	if (g_sw == 1 )
    	{
    	Symbol_Map(LCD_HEAT_ON);
	g_sw=0;
    	}
    	else 
    	{
	Symbol_Map(LCD_HEAT_OFF);
	g_sw=1;
    	}
	
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_tau0_channel1_interrupt
* Description  : This function is INTTM01 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_tau0_channel1_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    /* Declare count variables */
    /* 200ms */
    static uint8_t i = 0;
    static uint8_t j = 0;
    uint8_t u_k;
    float f_k;
    char res[5];
    /* Update the LCD every second timer interrupt */
	if (1u == i++)
	{
		/* Display the text on the LCD panel.
		   Casting to ensure correct data type */
		//Display_Panel_String(g_scroll_data_pointer);

		/* Reset the count */
		i = 0;

		/* Increment the address */
		g_scroll_data_pointer++;
	}

	/* Check if the NULL character has been encountered */
	if (0u == *g_scroll_data_pointer)
	{
		/* Copy the start address of the text to be scrolled */
		g_scroll_data_pointer = g_scroll_data[j];

		/* Reset the counter after all scroll sentences have been displayed */
		if (SCROLL_LINES == ++j)
		{
			j = 0;
		}

		/* Stop scrolling? */
		if (FINITE_SCROLL & g_enable_scroll)
		{
			/* Disable scrolling */
			g_enable_scroll &= ~ENABLE_SCROLL;
		}
	}
     
	
     R_ADC_Get_Result_8bit(&u_k);
     f_k = 25.0 + (1.05 - 0.005664 * ((float) u_k))/3.6;
     ftoa(f_k, res);     
     Display_Panel_String(res);
    /* Clear the interrupt flag */
   //Benson TMIF00 = 0x0;    
/* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_tau0_channel2_interrupt
* Description  : This function is INTTM02 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_tau0_channel2_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    /* 100ms*/
    if (g_sw1_enable_sample) 
    g_sw1_period ++; 
    else 
    g_sw1_period =0;
    
    if (g_sw2_enable_sample) 
    g_sw2_period ++; 
    else 
    g_sw2_period =0;
    
    if (g_sw1_period >=5)
    {
    g_sw1_hold = 1;
    g_sw1_enable_sample =0;
    }
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
