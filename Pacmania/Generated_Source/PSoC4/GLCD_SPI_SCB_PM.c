/***************************************************************************//**
* \file GLCD_SPI_SCB_PM.c
* \version 3.20
*
* \brief
*  This file provides the source code to the Power Management support for
*  the SCB Component.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2013-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "GLCD_SPI_SCB.h"
#include "GLCD_SPI_SCB_PVT.h"

#if(GLCD_SPI_SCB_SCB_MODE_I2C_INC)
    #include "GLCD_SPI_SCB_I2C_PVT.h"
#endif /* (GLCD_SPI_SCB_SCB_MODE_I2C_INC) */

#if(GLCD_SPI_SCB_SCB_MODE_EZI2C_INC)
    #include "GLCD_SPI_SCB_EZI2C_PVT.h"
#endif /* (GLCD_SPI_SCB_SCB_MODE_EZI2C_INC) */

#if(GLCD_SPI_SCB_SCB_MODE_SPI_INC || GLCD_SPI_SCB_SCB_MODE_UART_INC)
    #include "GLCD_SPI_SCB_SPI_UART_PVT.h"
#endif /* (GLCD_SPI_SCB_SCB_MODE_SPI_INC || GLCD_SPI_SCB_SCB_MODE_UART_INC) */


/***************************************
*   Backup Structure declaration
***************************************/

#if(GLCD_SPI_SCB_SCB_MODE_UNCONFIG_CONST_CFG || \
   (GLCD_SPI_SCB_SCB_MODE_I2C_CONST_CFG   && (!GLCD_SPI_SCB_I2C_WAKE_ENABLE_CONST))   || \
   (GLCD_SPI_SCB_SCB_MODE_EZI2C_CONST_CFG && (!GLCD_SPI_SCB_EZI2C_WAKE_ENABLE_CONST)) || \
   (GLCD_SPI_SCB_SCB_MODE_SPI_CONST_CFG   && (!GLCD_SPI_SCB_SPI_WAKE_ENABLE_CONST))   || \
   (GLCD_SPI_SCB_SCB_MODE_UART_CONST_CFG  && (!GLCD_SPI_SCB_UART_WAKE_ENABLE_CONST)))

    GLCD_SPI_SCB_BACKUP_STRUCT GLCD_SPI_SCB_backup =
    {
        0u, /* enableState */
    };
#endif


/*******************************************************************************
* Function Name: GLCD_SPI_SCB_Sleep
****************************************************************************//**
*
*  Prepares the GLCD_SPI_SCB component to enter Deep Sleep.
*  The �Enable wakeup from Deep Sleep Mode� selection has an influence on this 
*  function implementation:
*  - Checked: configures the component to be wakeup source from Deep Sleep.
*  - Unchecked: stores the current component state (enabled or disabled) and 
*    disables the component. See SCB_Stop() function for details about component 
*    disabling.
*
*  Call the GLCD_SPI_SCB_Sleep() function before calling the 
*  CyPmSysDeepSleep() function. 
*  Refer to the PSoC Creator System Reference Guide for more information about 
*  power management functions and Low power section of this document for the 
*  selected mode.
*
*  This function should not be called before entering Sleep.
*
*******************************************************************************/
void GLCD_SPI_SCB_Sleep(void)
{
#if(GLCD_SPI_SCB_SCB_MODE_UNCONFIG_CONST_CFG)

    if(GLCD_SPI_SCB_SCB_WAKE_ENABLE_CHECK)
    {
        if(GLCD_SPI_SCB_SCB_MODE_I2C_RUNTM_CFG)
        {
            GLCD_SPI_SCB_I2CSaveConfig();
        }
        else if(GLCD_SPI_SCB_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            GLCD_SPI_SCB_EzI2CSaveConfig();
        }
    #if(!GLCD_SPI_SCB_CY_SCBIP_V1)
        else if(GLCD_SPI_SCB_SCB_MODE_SPI_RUNTM_CFG)
        {
            GLCD_SPI_SCB_SpiSaveConfig();
        }
        else if(GLCD_SPI_SCB_SCB_MODE_UART_RUNTM_CFG)
        {
            GLCD_SPI_SCB_UartSaveConfig();
        }
    #endif /* (!GLCD_SPI_SCB_CY_SCBIP_V1) */
        else
        {
            /* Unknown mode */
        }
    }
    else
    {
        GLCD_SPI_SCB_backup.enableState = (uint8) GLCD_SPI_SCB_GET_CTRL_ENABLED;

        if(0u != GLCD_SPI_SCB_backup.enableState)
        {
            GLCD_SPI_SCB_Stop();
        }
    }

#else

    #if (GLCD_SPI_SCB_SCB_MODE_I2C_CONST_CFG && GLCD_SPI_SCB_I2C_WAKE_ENABLE_CONST)
        GLCD_SPI_SCB_I2CSaveConfig();

    #elif (GLCD_SPI_SCB_SCB_MODE_EZI2C_CONST_CFG && GLCD_SPI_SCB_EZI2C_WAKE_ENABLE_CONST)
        GLCD_SPI_SCB_EzI2CSaveConfig();

    #elif (GLCD_SPI_SCB_SCB_MODE_SPI_CONST_CFG && GLCD_SPI_SCB_SPI_WAKE_ENABLE_CONST)
        GLCD_SPI_SCB_SpiSaveConfig();

    #elif (GLCD_SPI_SCB_SCB_MODE_UART_CONST_CFG && GLCD_SPI_SCB_UART_WAKE_ENABLE_CONST)
        GLCD_SPI_SCB_UartSaveConfig();

    #else

        GLCD_SPI_SCB_backup.enableState = (uint8) GLCD_SPI_SCB_GET_CTRL_ENABLED;

        if(0u != GLCD_SPI_SCB_backup.enableState)
        {
            GLCD_SPI_SCB_Stop();
        }

    #endif /* defined (GLCD_SPI_SCB_SCB_MODE_I2C_CONST_CFG) && (GLCD_SPI_SCB_I2C_WAKE_ENABLE_CONST) */

#endif /* (GLCD_SPI_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: GLCD_SPI_SCB_Wakeup
****************************************************************************//**
*
*  Prepares the GLCD_SPI_SCB component for Active mode operation after 
*  Deep Sleep.
*  The �Enable wakeup from Deep Sleep Mode� selection has influence on this 
*  function implementation:
*  - Checked: restores the component Active mode configuration.
*  - Unchecked: enables the component if it was enabled before enter Deep Sleep.
*
*  This function should not be called after exiting Sleep.
*
*  \sideeffect
*   Calling the GLCD_SPI_SCB_Wakeup() function without first calling the 
*   GLCD_SPI_SCB_Sleep() function may produce unexpected behavior.
*
*******************************************************************************/
void GLCD_SPI_SCB_Wakeup(void)
{
#if(GLCD_SPI_SCB_SCB_MODE_UNCONFIG_CONST_CFG)

    if(GLCD_SPI_SCB_SCB_WAKE_ENABLE_CHECK)
    {
        if(GLCD_SPI_SCB_SCB_MODE_I2C_RUNTM_CFG)
        {
            GLCD_SPI_SCB_I2CRestoreConfig();
        }
        else if(GLCD_SPI_SCB_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            GLCD_SPI_SCB_EzI2CRestoreConfig();
        }
    #if(!GLCD_SPI_SCB_CY_SCBIP_V1)
        else if(GLCD_SPI_SCB_SCB_MODE_SPI_RUNTM_CFG)
        {
            GLCD_SPI_SCB_SpiRestoreConfig();
        }
        else if(GLCD_SPI_SCB_SCB_MODE_UART_RUNTM_CFG)
        {
            GLCD_SPI_SCB_UartRestoreConfig();
        }
    #endif /* (!GLCD_SPI_SCB_CY_SCBIP_V1) */
        else
        {
            /* Unknown mode */
        }
    }
    else
    {
        if(0u != GLCD_SPI_SCB_backup.enableState)
        {
            GLCD_SPI_SCB_Enable();
        }
    }

#else

    #if (GLCD_SPI_SCB_SCB_MODE_I2C_CONST_CFG  && GLCD_SPI_SCB_I2C_WAKE_ENABLE_CONST)
        GLCD_SPI_SCB_I2CRestoreConfig();

    #elif (GLCD_SPI_SCB_SCB_MODE_EZI2C_CONST_CFG && GLCD_SPI_SCB_EZI2C_WAKE_ENABLE_CONST)
        GLCD_SPI_SCB_EzI2CRestoreConfig();

    #elif (GLCD_SPI_SCB_SCB_MODE_SPI_CONST_CFG && GLCD_SPI_SCB_SPI_WAKE_ENABLE_CONST)
        GLCD_SPI_SCB_SpiRestoreConfig();

    #elif (GLCD_SPI_SCB_SCB_MODE_UART_CONST_CFG && GLCD_SPI_SCB_UART_WAKE_ENABLE_CONST)
        GLCD_SPI_SCB_UartRestoreConfig();

    #else

        if(0u != GLCD_SPI_SCB_backup.enableState)
        {
            GLCD_SPI_SCB_Enable();
        }

    #endif /* (GLCD_SPI_SCB_I2C_WAKE_ENABLE_CONST) */

#endif /* (GLCD_SPI_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/* [] END OF FILE */
