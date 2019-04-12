/*******************************************************************************
* File Name: GLCD_SPIM_UDB_IntClock.c
* Version 2.20
*
*  Description:
*   Provides system API for the clocking, interrupts and watchdog timer.
*
*  Note:
*   Documentation of the API's in this file is located in the
*   System Reference Guide provided with PSoC Creator.
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include <cydevice_trm.h>
#include "GLCD_SPIM_UDB_IntClock.h"

#if defined CYREG_PERI_DIV_CMD

/*******************************************************************************
* Function Name: GLCD_SPIM_UDB_IntClock_StartEx
********************************************************************************
*
* Summary:
*  Starts the clock, aligned to the specified running clock.
*
* Parameters:
*  alignClkDiv:  The divider to which phase alignment is performed when the
*    clock is started.
*
* Returns:
*  None
*
*******************************************************************************/
void GLCD_SPIM_UDB_IntClock_StartEx(uint32 alignClkDiv)
{
    /* Make sure any previous start command has finished. */
    while((GLCD_SPIM_UDB_IntClock_CMD_REG & GLCD_SPIM_UDB_IntClock_CMD_ENABLE_MASK) != 0u)
    {
    }
    
    /* Specify the target divider and it's alignment divider, and enable. */
    GLCD_SPIM_UDB_IntClock_CMD_REG =
        ((uint32)GLCD_SPIM_UDB_IntClock__DIV_ID << GLCD_SPIM_UDB_IntClock_CMD_DIV_SHIFT)|
        (alignClkDiv << GLCD_SPIM_UDB_IntClock_CMD_PA_DIV_SHIFT) |
        (uint32)GLCD_SPIM_UDB_IntClock_CMD_ENABLE_MASK;
}

#else

/*******************************************************************************
* Function Name: GLCD_SPIM_UDB_IntClock_Start
********************************************************************************
*
* Summary:
*  Starts the clock.
*
* Parameters:
*  None
*
* Returns:
*  None
*
*******************************************************************************/

void GLCD_SPIM_UDB_IntClock_Start(void)
{
    /* Set the bit to enable the clock. */
    GLCD_SPIM_UDB_IntClock_ENABLE_REG |= GLCD_SPIM_UDB_IntClock__ENABLE_MASK;
}

#endif /* CYREG_PERI_DIV_CMD */


/*******************************************************************************
* Function Name: GLCD_SPIM_UDB_IntClock_Stop
********************************************************************************
*
* Summary:
*  Stops the clock and returns immediately. This API does not require the
*  source clock to be running but may return before the hardware is actually
*  disabled.
*
* Parameters:
*  None
*
* Returns:
*  None
*
*******************************************************************************/
void GLCD_SPIM_UDB_IntClock_Stop(void)
{
#if defined CYREG_PERI_DIV_CMD

    /* Make sure any previous start command has finished. */
    while((GLCD_SPIM_UDB_IntClock_CMD_REG & GLCD_SPIM_UDB_IntClock_CMD_ENABLE_MASK) != 0u)
    {
    }
    
    /* Specify the target divider and it's alignment divider, and disable. */
    GLCD_SPIM_UDB_IntClock_CMD_REG =
        ((uint32)GLCD_SPIM_UDB_IntClock__DIV_ID << GLCD_SPIM_UDB_IntClock_CMD_DIV_SHIFT)|
        ((uint32)GLCD_SPIM_UDB_IntClock_CMD_DISABLE_MASK);

#else

    /* Clear the bit to disable the clock. */
    GLCD_SPIM_UDB_IntClock_ENABLE_REG &= (uint32)(~GLCD_SPIM_UDB_IntClock__ENABLE_MASK);
    
#endif /* CYREG_PERI_DIV_CMD */
}


/*******************************************************************************
* Function Name: GLCD_SPIM_UDB_IntClock_SetFractionalDividerRegister
********************************************************************************
*
* Summary:
*  Modifies the clock divider and the fractional divider.
*
* Parameters:
*  clkDivider:  Divider register value (0-65535). This value is NOT the
*    divider; the clock hardware divides by clkDivider plus one. For example,
*    to divide the clock by 2, this parameter should be set to 1.
*  fracDivider:  Fractional Divider register value (0-31).
* Returns:
*  None
*
*******************************************************************************/
void GLCD_SPIM_UDB_IntClock_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional)
{
    uint32 maskVal;
    uint32 regVal;
    
#if defined (GLCD_SPIM_UDB_IntClock__FRAC_MASK) || defined (CYREG_PERI_DIV_CMD)
    
	/* get all but divider bits */
    maskVal = GLCD_SPIM_UDB_IntClock_DIV_REG & 
                    (uint32)(~(uint32)(GLCD_SPIM_UDB_IntClock_DIV_INT_MASK | GLCD_SPIM_UDB_IntClock_DIV_FRAC_MASK)); 
	/* combine mask and new divider vals into 32-bit value */
    regVal = maskVal |
        ((uint32)((uint32)clkDivider <<  GLCD_SPIM_UDB_IntClock_DIV_INT_SHIFT) & GLCD_SPIM_UDB_IntClock_DIV_INT_MASK) |
        ((uint32)((uint32)clkFractional << GLCD_SPIM_UDB_IntClock_DIV_FRAC_SHIFT) & GLCD_SPIM_UDB_IntClock_DIV_FRAC_MASK);
    
#else
    /* get all but integer divider bits */
    maskVal = GLCD_SPIM_UDB_IntClock_DIV_REG & (uint32)(~(uint32)GLCD_SPIM_UDB_IntClock__DIVIDER_MASK);
    /* combine mask and new divider val into 32-bit value */
    regVal = clkDivider | maskVal;
    
#endif /* GLCD_SPIM_UDB_IntClock__FRAC_MASK || CYREG_PERI_DIV_CMD */

    GLCD_SPIM_UDB_IntClock_DIV_REG = regVal;
}


/*******************************************************************************
* Function Name: GLCD_SPIM_UDB_IntClock_GetDividerRegister
********************************************************************************
*
* Summary:
*  Gets the clock divider register value.
*
* Parameters:
*  None
*
* Returns:
*  Divide value of the clock minus 1. For example, if the clock is set to
*  divide by 2, the return value will be 1.
*
*******************************************************************************/
uint16 GLCD_SPIM_UDB_IntClock_GetDividerRegister(void)
{
    return (uint16)((GLCD_SPIM_UDB_IntClock_DIV_REG & GLCD_SPIM_UDB_IntClock_DIV_INT_MASK)
        >> GLCD_SPIM_UDB_IntClock_DIV_INT_SHIFT);
}


/*******************************************************************************
* Function Name: GLCD_SPIM_UDB_IntClock_GetFractionalDividerRegister
********************************************************************************
*
* Summary:
*  Gets the clock fractional divider register value.
*
* Parameters:
*  None
*
* Returns:
*  Fractional Divide value of the clock
*  0 if the fractional divider is not in use.
*
*******************************************************************************/
uint8 GLCD_SPIM_UDB_IntClock_GetFractionalDividerRegister(void)
{
#if defined (GLCD_SPIM_UDB_IntClock__FRAC_MASK)
    /* return fractional divider bits */
    return (uint8)((GLCD_SPIM_UDB_IntClock_DIV_REG & GLCD_SPIM_UDB_IntClock_DIV_FRAC_MASK)
        >> GLCD_SPIM_UDB_IntClock_DIV_FRAC_SHIFT);
#else
    return 0u;
#endif /* GLCD_SPIM_UDB_IntClock__FRAC_MASK */
}


/* [] END OF FILE */