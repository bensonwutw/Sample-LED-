
/***********************************************************************************************************************
* File Name    : ZAC.c
* Version      : 1.00
* Device(s)    : ALL Series
* Tool-Chain   : GNURL78 v12.03
* Description  : This file contains the ZAC wire driver.
* Creation Date: 10/09/2016
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes 
***********************************************************************************************************************/

#include "r_cg_macrodriver.h"
#include "ZAC.h"


/***********************************************************************************************************************
Global Variables & Defined Constants
***********************************************************************************************************************/
/* Declare a variable for A/D results */
volatile uint16_t g_adc_result = 0;
/* Global buffer array for storing the ADC result integer to string conversion  */
volatile uint8_t g_lcd_buffer[4] = "xxx";

/***********************************************************************************************************************
User Program Code
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : ZacInit
* Description   : ZACwire interface initialization
* Arguments     : NA
*                         
* Return Value  : NA
***********************************************************************************************************************/
void ZacInit(void)
{
	SMATE_ON=0;
	SMATE_OUT=0;	
}




/***********************************************************************************************************************
* Function Name : getSmateSensor
* Description   : 
* Arguments     : 
*                         
* Return Value  : 
***********************************************************************************************************************/
uint16_t getSmateSensor (uint16_t *temp_value16)
{
	uint16_t temp_value1 = 0,temp_value2 = 0,Temperature;
	uint8_t i,parity;

	SMATE_ON = 1;
	P1 &= 0xfd;
	PM1 |= 0x02;	
	ZacWireWait(); // wait for stabilization
	ZacWireWait();

	while (SMATE_OUT); // wait until start bit starts
	// wait, TStrobe
	while (!SMATE_OUT);

	// first data byte
	// read 8 data bits and 1 parity bit
	for (i = 0; i < 9; i++)
	{
		while (SMATE_OUT); // wait for falling edge
		ZacWireWait();

		if (SMATE_OUT)
			temp_value1 |= 1 << (8-i); // get the bit
		else
			while (!SMATE_OUT); // wait until line comes high again
	}

	// second byte
	while (SMATE_OUT);
	// wait, TStrobe
	while (!SMATE_OUT);
	
	// read 8 data bits and 1 parity bit
	for (i = 0; i < 9; i++)
	{
		while (SMATE_OUT); // wait for falling edge
		ZacWireWait();

		if (SMATE_OUT)
			temp_value2 |= 1 << (8-i); // get the bit
		else
			while (!SMATE_OUT); // wait until line comes high again
	}

	P1 |= 0x02;
	PM1 &= 0xfd;	
	SMATE_ON=0; // switch CHIPCAP off
	
	// check parity for byte 1
	parity = 0;
	for (i = 0; i < 9; i++)
		if (temp_value1 & (1 << i))
			parity++;
	if (parity % 2)
		return FALSE;

	// check parity for byte 2
	parity = 0;
	for (i = 0; i < 9; i++)
		if (temp_value2 & (1 << i))
			parity++;
	if (parity % 2)
		return FALSE;
	
	temp_value1 >>= 1; // delete parity bit
	temp_value2 >>= 1; // delete parity bit
	Temperature = (temp_value1 << 8) | temp_value2;
	*temp_value16 = Temperature;

	return TRUE; // parity is OK
}

/***********************************************************************************************************************
* Function Name : ZacWireWait
* Description   : 
* Arguments     : 
*                         
* Return Value  : 120us wait
***********************************************************************************************************************/

void ZacWireWait(void)
{
	unsigned int DelayTime;

	for(DelayTime = 0;DelayTime<400;DelayTime++)
		NOP();
}

