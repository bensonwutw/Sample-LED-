/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING 
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT 
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR 
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE 
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software 
* and to discontinue the availability of this software.  By using this software, 
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : lcd_panel.c
* Version      : 1.00
* Device(s)    : R5F10RLC
* Tool-Chain   : GNURL78 v12.03
* Description  : This file contains the LCD panel driver.
* Creation Date: 21/01/2013
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes 
***********************************************************************************************************************/

#include "r_cg_macrodriver.h"
#include "r_cg_lcd.h"
#include "lcd_panel.h"

/***********************************************************************************************************************
Global Variables & Defined Constants
***********************************************************************************************************************/
/* Declare Array Maps for writing to section A, the middle line of the LCD panel */
LCDMAP SECTA_map[20];

/* Function prototype for initializing Section A glyph maps */
static void Init_SectA_Maps (void);

/***********************************************************************************************************************
User Program Code
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : Validate_Input
* Description   : Takes any char and validates the input for the Update_Display 
*                 function. Capitalizes all letters, and re-maps symbols. Outputs
*                 to a variable specified by the pointer parameter passed.
* Arguments     : uint8_t input
*                         to be validated
*                 uint8_t * output
*                         pointer to output buffer
* Return Value  : uint16_t
*                 0: Unsupported Character
*                 1: Number (or space)
*                 2: Letter A-F
*                 3: Letter G-Z
*                 4: Supported Symbol
***********************************************************************************************************************/
uint16_t Validate_Input (const uint8_t input, uint8_t * const output)
{
    /* Is space or Carriage Return? */
    if ((' ' == input) || (0x0D == input))
    {
        *output = 0x29u;
        
        return 1u;
    }
    /* Is Number? */
    else if ((input < 0x3Au) && (input > 0x2Fu))
    {
        /* Convert from ASCII.
           Casting to ensure use of correct data type.*/
        *output = (uint8_t)(input - 0x30);
        
        return 1u;
    }
    
    /* Is Upper-case Alpha A-F ? */
    else if ((input < 0x47u) && (input > 0x40u))
    {
        /* Convert from ASCII.
           Casting to ensure use of correct data type.*/
        *output = (uint8_t)(input - 0x33);
        
        return 2u;
    }
    
    /* Is Lower-case Alpha A-F */
    else if ((input < 0x67u) && (input > 0x60u))
    {
        /* Shift Case & Convert from ASCII.
           Casting to ensure use of correct data type.*/
        *output = (uint8_t)(input - 0x53u);
        
        return 2u;
    }
    
    /* Is Upper-case Alpha L ? */
    else if (input == 0x4Cu) 
    {
        /* Convert from ASCII.
           Casting to ensure use of correct data type.*/
        *output = (uint8_t)(input - 0x42u);
        
        return 2u;
    }
    /* IsLower-case Alpha L? */
    else if (input == 0x6Cu)
    {
        /* Convert from ASCII.
           Casting to ensure use of correct data type.*/
        *output = (uint8_t)(input - 0x62u);
        
        return 2u;
    }
    
    
    /* Is Symbol? */
    else
    {
        /* Check input against supported symbols */
        switch (input)
        {
            case 0x2Bu:
                {
                    /* Is Plus */
                    *output = 0x0Au;
                }
            break;
            
            case 0x2Du:
                {
                    /* Is Minus */
                    *output = 0x0Bu;
                }
            break;
            
            case 0x2Fu:
                {
                    /* Is Forward Slash */
                    *output = 0x0Cu;
                }
            break;
            
            case 0x5Cu:
                {
                    /* Is Back Slash */
                    *output = 0x0Du;
                }
             break;
            
            case 0x2Eu:
                {
                    /* Is full stop */
                    *output = 0x2Eu;
                }
            break;
            
            case 0x3Au:
                {
                    /* Is colon */
                    *output = 0x3Au;
                }
            break;
            
            case 0x2Au:
                {
                    /* Is asterisk */
                    *output = 0x28u;
                }
            break;
            
            
	    
	    
	    
	    
	    default:
                {
                    /* Unsupported Character, do nothing */
                    return 0u;
                }
            break;
        }
        
        /* The character received is supported */
        return 4u;
    }
}
/***********************************************************************************************************************
* End of function Validate_Input
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: SECTA_Glyph_Map
* Description  : Takes a validated char input and maps the character to a 
*                segment pattern, to be displayed on Section A (mid-section) of the LCD panel.
* Arguments    : const uint8_t glyph  - character to be displayed, 
*                uint16_t digit - digit position of character
* Return Value : none
***********************************************************************************************************************/
void SECTA_Glyph_Map (uint8_t const glyph, uint16_t const digit)
{
    /* Declare the first pointer for mapping the character to the segments */
    volatile uint8_t * PinPtr1 = PDL_NO_PTR;
    
    /* Declare the second pointer for mapping the character to the segments */
    volatile uint8_t * PinPtr2 = PDL_NO_PTR;
    
    /* Set Pointers to Correct Segments */
    switch(digit)
    {
        /* First Digit */
        case CASE_0:
            {
                /* Assignment the segment register address to the pointers */
                PinPtr1 = &SEG12_DEF;
                PinPtr2 = &SEG11_DEF;
            }
        break;
        
        /* Second Digit */
        case CASE_1:
            {
                /* Assignment the segment register address to the pointers */
                PinPtr1 = &SEG14_DEF;
                PinPtr2 = &SEG13_DEF;
            }
        break;
        
        /* Decimal Point */
        case CASE_2:
            {
                /* Assignment the segment register address to the pointers */
                PinPtr1 = &SEG15_DEF;
                PinPtr2 = PDL_NO_PTR;
            }
        break;
        
        /* Third Digit */
        case CASE_3:
            {
                /* Assignment the segment register address to the pointers */
                PinPtr1 = &SEG16_DEF;
                PinPtr2 = &SEG15_DEF;
            }
        break;
        
        default:
            {
                /* Do nothing */
            }
        break;
    }
    
    /* Bit Mask Segments */
    *PinPtr1 &= 0xF0u; //Clear all Right-side segment contents
    *PinPtr2 &= 0xF8u; //clear all left-side segment contents
    
    /* Decimal point? */
    if (2u == digit)
    {
        if ('.' == glyph)
        {
            /* Turn on Decimal Point */
            *PinPtr1 |= 0x08u;
        }
        else
        {
            /* Turn off Decimal Point */
            *PinPtr1 &= 0xF7u;
        }
    }
    else if (GLYPH_CHECK != glyph)
    {        
        /* Digit-Segment Mapping */
        *PinPtr1 |= SECTA_map[glyph].NIBBLE.TWO;
        *PinPtr2 |= SECTA_map[glyph].NIBBLE.ONE;
    }
    else
    {
        /* Do nothing */
    }
}
/***********************************************************************************************************************
End of function SECTA_Glyph_Map
***********************************************************************************************************************/









/***********************************************************************************************************************
* Function Name : Symbol_Map
* Description   : Takes a symbol code and turns on/off LCD Panel Symbols. The 
*                 first digit should be which symbol to control (see symbols 
*                 listed below) 
*                 The second should be either a 1 or a 0. 
*                 (1 - Turn on, 0 - Turn Off)
*                 1 - Heat         5 - mmHg             9 - Degrees F
*                 2 - Fan          6 - Volts            A - Alarm
*                 3 - Zone         7 - Heart            B - AM
*                 4 - mg/ml        8 - Degrees C        C - PM
*                 For example '61' would turn on the Volts symbol and 
*                 C0 would turn off the PM symbol.
* Arguments     : int16_t input - symbol to be controlled
* Return Value  : uint8_t
*                           0 - valid input
                            1 - invalid input
***********************************************************************************************************************/
uint8_t Symbol_Map (uint16_t const input )
{
    /* Declare a status variable */
    uint8_t status = 0;
    
    /* Select the case based on the input selection. */
    switch(input)
   
    {
        case LCD_HEAT_ON:
            {
                SEG28_DEF |= 0x01u;
            }
        break;
        
        case LCD_HEAT_OFF:
            {
                SEG28_DEF &= 0xFEu;
            }
        break;
        
        case LCD_FAN_ON:
            {
                SEG34_DEF |= 0x01u;
            }
        break;
        
        case LCD_FAN_OFF:
            {
                SEG34_DEF &= 0xFEu;
            }
        break;
        
        case LCD_ZONE_ON:
            {
                SEG33_DEF |= 0x01u;
            }
        break;
        
        case LCD_ZONE_OFF:
            {
                SEG33_DEF &= 0xFEu;
            }
        break;
        
        case LCD_VOLTS_ON:
            {
                SEG32_DEF |= 0x01u;
            }
        break;
        
        case LCD_VOLTS_OFF:
            {
                SEG32_DEF &= 0xFEu;
            }
        break;
        
        case LCD_HEART_ON:
            {
                SEG31_DEF |= 0x01u;
            }
        break;
        
        case LCD_HEART_OFF:
            {
                SEG31_DEF &= 0xFEu;
            }
        break;
        
        case LCD_DEGREESC_ON:
            {
                SEG17_DEF |= 0x04u;
            }
        break;
        
        case LCD_DEGREESC_OFF:
            {
                SEG17_DEF &= 0xFBu;
            }
        break;
        
        case LCD_DEGREESF_ON:
            {
                SEG17_DEF |= 0x08u;
            }
        break;
        
        case LCD_DEGREESF_OFF:
            {
                SEG17_DEF &= 0xF7u;
            }
        break;
        
        case LCD_ALARM_ON:
            {
                SEG19_DEF |= 0x02u;
            }
        break;
        
        case LCD_ALARM_OFF:
            {
                SEG19_DEF &= 0xFDu;
            }
        break;
        
        case LCD_ALARM_ON_1:
            {
                SEG19_DEF |= 0x02u;
            }
        break;
        
        case LCD_ALARM_OFF_1:
            {
                SEG19_DEF &= 0xFDu;
            }
        break;
        
        case LCD_AM_ON:
            {
                SEG19_DEF |= 0x01u;
            }
        break;
        
        case LCD_AM_OFF:
            {
                SEG19_DEF &= 0xFEu;
            }
        break;
        
        case LCD_AM_ON_1:
            {
                SEG19_DEF |= 0x01u;
            }
        break;
        
        case LCD_AM_OFF_1:
            {
                SEG28_DEF &= 0xFEu;
            }
        break;
        
        case LCD_PM_ON:
            {
                SEG17_DEF |= 0x02u;
            }
        break;
        
        case LCD_PM_OFF:
            {
                SEG17_DEF &= 0xFDu;
            }
        break;
        
        case LCD_PM_ON_1:
            {
                SEG17_DEF |= 0x02u;
            }
        break;
        
        case LCD_PM_OFF_1:
            {
                SEG17_DEF &= 0xFDu;
            }
        break;
        
        case LCD_COOL_ON:
            {
                SEG30_DEF |= 0x01u;
            }
        break;
        
        case LCD_COOL_OFF:
            {
                SEG30_DEF &= 0xF0u;
            }
        break;
        
                
        default:
            {
                /* Bad Selection - Do Nothing */
                status = 1u;
            }
        break;
    }
    
    /* Return the status */
    return status;
}
/***********************************************************************************************************************
End of function Symbol_Map
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : Init_Display_Panel
* Description   : Calls functions in order to prepare the LCD Panel for use
* Arguments     : None
* Return Value  : None
***********************************************************************************************************************/
void Init_Display_Panel (void)
{
    /* Load up Segment Maps */
    Init_Maps();
    
    /* Clear LCD Data Registers */
    Clear_Display();
    
    /* Enable the LCD */
    R_LCD_Start();
    
    /* Enable the voltage boost circuit */
    R_LCD_Set_VoltageOn();
}
/***********************************************************************************************************************
End of function Init_Display_Panel
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : Clear_Display
* Description   : Clears all the segment data registers, thereby clearing the screen by the next LCD frame duration.
* Arguments     : none
* Return Value  : none
***********************************************************************************************************************/
void Clear_Display (void)
{
    /* Declare a loop count variable */
    uint8_t i;
    
     /* Initialize pointer to start of registers */
    volatile uint8_t * RegPtr = &SEG1_DEF;
    
    /* Execute the instructions in the loop 40 times */
    for (i = 0; i < 40;i++)
    {
        /* Write 0 to the register being pointed to.*/
        *RegPtr = 0u;
        
        /* Increment the pointer */
        RegPtr++;
    }
}
/***********************************************************************************************************************
End of function Clear_Display
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : Init_Maps
* Description   : Initializes the glyph-segment maps used to display letters, symbols and numbers.
* Arguments     : none
* Return Value  : none
***********************************************************************************************************************/
void Init_Maps (void)
 {
     /* Initialize section A maps */
     Init_SectA_Maps();
 }
/***********************************************************************************************************************
End of function Init_Maps
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : Init_SectA_Maps
* Description   : Initializes Sect A maps.
* Arguments     : none
* Return Value  : none
***********************************************************************************************************************/
void Init_SectA_Maps (void)
{
     /* Section A maps START */
     
     /* 0 */
     SECTA_map[0x0].WORD = 0x00F5u;
     
     /* 1 */
     SECTA_map[0x1].WORD = 0x0060u;
     
     /* 2 */
     SECTA_map[0x2].WORD = 0x00B6u;
     
     /* 3 */
     SECTA_map[0x3].WORD = 0x00F2u;
     
     /* 4 */
     SECTA_map[0x4].WORD = 0x0063u;
     
     /* 5 */
     SECTA_map[0x5].WORD = 0x00D3u;
     
     /* 6 */
     SECTA_map[0x6].WORD = 0x00D7u;
     
     /* 7 */
     SECTA_map[0x7].WORD = 0x0070u;
     
     /* 8 */
     SECTA_map[0x8].WORD = 0x00F7u;
     
     /* 9 */
     SECTA_map[0x9].WORD = 0x00F3u;
     
      /* L */
     SECTA_map[0xA].WORD = 0x0085u;
     
     /* A */
     SECTA_map[0xE].WORD = 0x0077u;
     
     /* B */
     SECTA_map[0xF].WORD = 0x00C7u;
     
     /* C */
     SECTA_map[0x10].WORD = 0x0095u;
     
     /* D */
     SECTA_map[0x11].WORD = 0x00E6u;
     
     /* E */
     SECTA_map[0x12].WORD = 0x0097u;
     
     /* F */
     SECTA_map[0x13].WORD = 0x0017u;
     
    
     
}
/***********************************************************************************************************************
End of function Init_SectA_Maps
***********************************************************************************************************************/
/***********************************************************************************************************************
Function Name : SECTD_Glyph_Map
Description   : Takes a number input, and sets the level of the battery indicator (section D)
Argument      : uint8_t level       - Indicator Level (char)
*               uint8_t bat_outline - Turns on or off the battery outline
Return Values : None
***********************************************************************************************************************/
void SECTD_Glyph_Map (uint8_t const level, uint8_t const bat_outline)
{
    /* Turn off all levels */
    SEG18_DEF &= 0xF0u;
    
    /* Select the case based on the input level */
    switch (level)
    { 
        case LEVEL_0:
            {
                /* Battery Outline & 0 Level */
                SEG18_DEF |= 0x00u;
            }
        break;
        
        case LEVEL_1:
            {
                /* Battery Outline & 1 Level */
                SEG18_DEF |= 0x08u;
            }
        break;
        
        case LEVEL_2:
            {
                /* Battery Outline & 2 Levels */
                SEG18_DEF |= 0x0Cu;
            }
        break;
        
        case LEVEL_3:
            {
                /* Battery Outline & 3 Levels */
                SEG18_DEF |= 0x0Eu;
            }
        break;
        
        case LEVEL_4:
            {
                /* All Levels On */
                SEG18_DEF |= 0x0Fu;
            }
        break;
        
        default:
            {
                /* Bad Selection */
            }
        break;
    }
    
    /* Check if the battery outline is required */
    if (BATTERY_OUTLINE_OFF == bat_outline)
    {
        /* Turn off the battery outline */
        SEG17_DEF &= ~(0x01);
    }
    else
    {
        /* Turn on the battery outline */
        SEG17_DEF |= 0x01;
    }
}
/***********************************************************************************************************************
End of function SECTD_Glyph_Map
***********************************************************************************************************************/


/***********************************************************************************************************************
* Function Name : Display_Panel_String
* Description     This function writes to line 1, line 2 or 3 of the LCD. You need to use the defines LCD_LINE1, 
                  LCD_LINE2 and LCD_LINE3 in order to specify the starting position. 
* Arguments     : (const char *) string - pointer to data to be written to display (up to 8 chars).
* Return Value  : None
***********************************************************************************************************************/
void Display_Panel_String (char * const string)
{
    /* Declare variable to hold the output data */
    uint8_t output_buf = 0u;
    
    /* Declare a variable flag to hold the input validation result */
    uint16_t flag = 0u;
    
    /* Declare a loop count variable */
    uint16_t i  = 0u;
    
    /* Display input onto LCD.Castings used to ensure correct data types in assignments and passing on of arguments */
   
                for (i = 0u; i < 5u; i++)
                {
                    /* Validate Input.Casting used to ensure correct data type is used. */
                    uint8_t cc;
		       cc =  string[i];
		       flag = Validate_Input((uint8_t)string[i], &output_buf);
                
                    /* Is Number? */
                    if ((1u == flag) || (2u == flag) || (4u == flag))
                    {
                        if ((i > 1) && ('.' != string[2]))
                        {
                            /* Map Glyph to Segment Pattern */
                            SECTA_Glyph_Map(output_buf, (i+1u));
                        }
                        else
                        {
                            /* Map Glyph to Segment Pattern */
                            SECTA_Glyph_Map(output_buf, i);
                        }
                    }
                }

}
/***********************************************************************************************************************
End of function Display_Panel_String
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : Display_Panel_Delay
* Description   : Delay routine for LCD or any other devices.
* Arguments     :(uint32_t) units - time in, approximately, microseconds
* Return Value  : None
***********************************************************************************************************************/
void Display_Panel_Delay (uint32_t units)
{
    /* Declare a delay count */
    uint32_t counter = units * PANEL_DELAY_TIMING;
    
    /* Decrement the counter until it reaches 0 */
    while (counter--)
    {
        /* Do nothing */
    }
}
/***********************************************************************************************************************
End of function Display_Panel_Delay
************************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: LCD_OFF
* Description  : This function turns off all LCD segments.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void LCD_OFF (void)
{
    SEG0 = 0x00;
    SEG1 = 0x00;
    SEG2 = 0x00;
    SEG3 = 0x00;
    SEG4 = 0x00;
    SEG5 = 0x00;
    SEG6 = 0x00;
    SEG7 = 0x00;
    SEG8 = 0x00;
    SEG9 = 0x00;
    SEG10 = 0x00;
    SEG11 = 0x00;
    SEG12 = 0x00;
    SEG13 = 0x00;
    SEG14 = 0x00;
    SEG15 = 0x00;
    SEG16 = 0x00;
    SEG19 = 0x00;
    SEG20 = 0x00;
    SEG21 = 0x00;
    SEG22 = 0x00;
    SEG23 = 0x00;
    SEG24 = 0x00;
    SEG25 = 0x00;
    SEG26 = 0x00;
    SEG27 = 0x00;
    SEG28 = 0x00;
    SEG31 = 0x00;
    SEG32 = 0x00;
    SEG33 = 0x00;
    SEG34 = 0x00;
    SEG35 = 0x00;
    SEG36 = 0x00;
    SEG37 = 0x00;
    SEG38 = 0x00;
}
/***********************************************************************************************************************
End of function LCD_OFF
************************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: LCD_ON
* Description  : This function turns on all LCD segments.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void LCD_ON (void)
{
    SEG0 = 0x0f;
    SEG1 = 0x0f;
    SEG2 = 0x0f;
    SEG3 = 0x0f;
    SEG4 = 0x0f;
    SEG5 = 0x0f;
    SEG6 = 0x0f;
    SEG7 = 0x0f;
    SEG8 = 0x0f;
    SEG9 = 0x0f;
    
    /* Renesas symbol is always off */
    SEG10 = 0x07;
    SEG11 = 0x0f;
    SEG12 = 0x0f;
    SEG13 = 0x0f;
    SEG14 = 0x0f;
    SEG15 = 0x0f;
    SEG16 = 0x0f;
    SEG19 = 0x0f;
    SEG20 = 0x0f;
    SEG21 = 0x0f;
    SEG22 = 0x0f;
    SEG23 = 0x0f;
    SEG24 = 0x0f;
    SEG25 = 0x0f;
    SEG26 = 0x0f;
    SEG27 = 0x0f;
    
    /* The first 7-segment of the digital clock is turned off */
    SEG28 = 0x01;
    SEG31 = 0x0f;
    SEG32 = 0x0f;
    SEG33 = 0x0f;
    SEG34 = 0x0f;
    SEG35 = 0x0f;
    SEG36 = 0x0f;
    
    /* Turn off T12 */
    SEG37 = 0x0e;
    
    /* Turn off T11 */
    SEG38 = 0x0e;
}
/***********************************************************************************************************************
End of function LCD_ON
************************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: GTC_DISPLAY 
* Description  : This function display GTC 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void GTC_DISPLAY (void)
{
  uint8_t weekno,secno,minno,hourno;
  //int8_t  g_lcd_buffer[12] = "12:34:78\.9";
  int8_t  g_lcd_buffer[12] = "78.9";
      
    
    Symbol_Map(LCD_FAN_ON);
    Symbol_Map(LCD_ZONE_ON);
    Symbol_Map(LCD_VOLTS_ON);   
    Symbol_Map(LCD_HEART_ON);
    Symbol_Map(LCD_DEGREESC_ON);
    Symbol_Map(LCD_DEGREESF_ON);
    Symbol_Map(LCD_ALARM_ON);
    Symbol_Map(LCD_ALARM_ON_1);
    Symbol_Map(LCD_AM_ON);
    Symbol_Map(LCD_AM_ON_1);
    Symbol_Map(LCD_PM_ON);
    Symbol_Map(LCD_PM_ON_1);
    Symbol_Map(LCD_COOL_ON);
    
    SECTD_Glyph_Map(BATT_LEVEL_2, BATTERY_OUTLINE_ON);
    
    
    /* Update the time on the LCD panel.   Casting to ensure use of correct data type.*/
    Display_Panel_String(g_lcd_buffer);
}


/***********************************************************************************************************************
* Function Name: ftoa
* Description  : This function convert float number to string.
* Input Arguments: float n
* Return Value : char *res
***********************************************************************************************************************/
char ftoa(float n, char *res)
{
    // Extract integer part
    uint16_t ipart = (uint16_t)n;
 
    // Extract floating part
    uint16_t int_fpart  =  (uint16_t)( (n*10.0)  - (float)(ipart* 10));
          
    // convert integer part to string
  if ((ipart < 10) && (n >= 0.0))  sprintf(res," %d.%d",ipart,int_fpart);
  else if ((ipart >= 10) && (ipart < 100)) sprintf(res,"%d.%d",ipart,int_fpart);
  else if ((ipart >= 100) && (ipart <= 103)) sprintf(res,"%d",ipart,int_fpart);
  else if ((n < 0.0) || (ipart > 103)) sprintf(res,"---");
 
  
   
  
}
