/*******************************************************************************
* File Name: GLCD_1_SCLK.c  
* Version 1.90
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "GLCD_1_SCLK.h"

#define SetP4PinDriveMode(shift, mode)  \
    do { \
        GLCD_1_SCLK_PC =   (GLCD_1_SCLK_PC & \
                                (uint32)(~(uint32)(GLCD_1_SCLK_DRIVE_MODE_IND_MASK << (GLCD_1_SCLK_DRIVE_MODE_BITS * (shift))))) | \
                                (uint32)((uint32)(mode) << (GLCD_1_SCLK_DRIVE_MODE_BITS * (shift))); \
    } while (0)


/*******************************************************************************
* Function Name: GLCD_1_SCLK_Write
********************************************************************************
*
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  None 
*  
*******************************************************************************/
void GLCD_1_SCLK_Write(uint8 value) 
{
    uint8 drVal = (uint8)(GLCD_1_SCLK_DR & (uint8)(~GLCD_1_SCLK_MASK));
    drVal = (drVal | ((uint8)(value << GLCD_1_SCLK_SHIFT) & GLCD_1_SCLK_MASK));
    GLCD_1_SCLK_DR = (uint32)drVal;
}


/*******************************************************************************
* Function Name: GLCD_1_SCLK_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to this drive mode.
*
* Return: 
*  None
*
*******************************************************************************/
void GLCD_1_SCLK_SetDriveMode(uint8 mode) 
{
	SetP4PinDriveMode(GLCD_1_SCLK__0__SHIFT, mode);
}


/*******************************************************************************
* Function Name: GLCD_1_SCLK_Read
********************************************************************************
*
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro GLCD_1_SCLK_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 GLCD_1_SCLK_Read(void) 
{
    return (uint8)((GLCD_1_SCLK_PS & GLCD_1_SCLK_MASK) >> GLCD_1_SCLK_SHIFT);
}


/*******************************************************************************
* Function Name: GLCD_1_SCLK_ReadDataReg
********************************************************************************
*
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 GLCD_1_SCLK_ReadDataReg(void) 
{
    return (uint8)((GLCD_1_SCLK_DR & GLCD_1_SCLK_MASK) >> GLCD_1_SCLK_SHIFT);
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(GLCD_1_SCLK_INTSTAT) 

    /*******************************************************************************
    * Function Name: GLCD_1_SCLK_ClearInterrupt
    ********************************************************************************
    *
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  None 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 GLCD_1_SCLK_ClearInterrupt(void) 
    {
		uint8 maskedStatus = (uint8)(GLCD_1_SCLK_INTSTAT & GLCD_1_SCLK_MASK);
		GLCD_1_SCLK_INTSTAT = maskedStatus;
        return maskedStatus >> GLCD_1_SCLK_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */
