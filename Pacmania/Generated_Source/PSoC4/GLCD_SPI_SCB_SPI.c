/***************************************************************************//**
* \file GLCD_SPI_SCB_SPI.c
* \version 3.20
*
* \brief
*  This file provides the source code to the API for the SCB Component in
*  SPI mode.
*
* Note:
*
*******************************************************************************
* \copyright
* Copyright 2013-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "GLCD_SPI_SCB_PVT.h"
#include "GLCD_SPI_SCB_SPI_UART_PVT.h"

#if(GLCD_SPI_SCB_SCB_MODE_UNCONFIG_CONST_CFG)

    /***************************************
    *  Configuration Structure Initialization
    ***************************************/

    const GLCD_SPI_SCB_SPI_INIT_STRUCT GLCD_SPI_SCB_configSpi =
    {
        GLCD_SPI_SCB_SPI_MODE,
        GLCD_SPI_SCB_SPI_SUB_MODE,
        GLCD_SPI_SCB_SPI_CLOCK_MODE,
        GLCD_SPI_SCB_SPI_OVS_FACTOR,
        GLCD_SPI_SCB_SPI_MEDIAN_FILTER_ENABLE,
        GLCD_SPI_SCB_SPI_LATE_MISO_SAMPLE_ENABLE,
        GLCD_SPI_SCB_SPI_WAKE_ENABLE,
        GLCD_SPI_SCB_SPI_RX_DATA_BITS_NUM,
        GLCD_SPI_SCB_SPI_TX_DATA_BITS_NUM,
        GLCD_SPI_SCB_SPI_BITS_ORDER,
        GLCD_SPI_SCB_SPI_TRANSFER_SEPARATION,
        0u,
        NULL,
        0u,
        NULL,
        (uint32) GLCD_SPI_SCB_SCB_IRQ_INTERNAL,
        GLCD_SPI_SCB_SPI_INTR_RX_MASK,
        GLCD_SPI_SCB_SPI_RX_TRIGGER_LEVEL,
        GLCD_SPI_SCB_SPI_INTR_TX_MASK,
        GLCD_SPI_SCB_SPI_TX_TRIGGER_LEVEL,
        (uint8) GLCD_SPI_SCB_SPI_BYTE_MODE_ENABLE,
        (uint8) GLCD_SPI_SCB_SPI_FREE_RUN_SCLK_ENABLE,
        (uint8) GLCD_SPI_SCB_SPI_SS_POLARITY
    };


    /*******************************************************************************
    * Function Name: GLCD_SPI_SCB_SpiInit
    ****************************************************************************//**
    *
    *  Configures the GLCD_SPI_SCB for SPI operation.
    *
    *  This function is intended specifically to be used when the GLCD_SPI_SCB 
    *  configuration is set to �Unconfigured GLCD_SPI_SCB� in the customizer. 
    *  After initializing the GLCD_SPI_SCB in SPI mode using this function, 
    *  the component can be enabled using the GLCD_SPI_SCB_Start() or 
    * GLCD_SPI_SCB_Enable() function.
    *  This function uses a pointer to a structure that provides the configuration 
    *  settings. This structure contains the same information that would otherwise 
    *  be provided by the customizer settings.
    *
    *  \param config: pointer to a structure that contains the following list of 
    *   fields. These fields match the selections available in the customizer. 
    *   Refer to the customizer for further description of the settings.
    *
    *******************************************************************************/
    void GLCD_SPI_SCB_SpiInit(const GLCD_SPI_SCB_SPI_INIT_STRUCT *config)
    {
        if(NULL == config)
        {
            CYASSERT(0u != 0u); /* Halt execution due to bad function parameter */
        }
        else
        {
            /* Configure pins */
            GLCD_SPI_SCB_SetPins(GLCD_SPI_SCB_SCB_MODE_SPI, config->mode, GLCD_SPI_SCB_DUMMY_PARAM);

            /* Store internal configuration */
            GLCD_SPI_SCB_scbMode       = (uint8) GLCD_SPI_SCB_SCB_MODE_SPI;
            GLCD_SPI_SCB_scbEnableWake = (uint8) config->enableWake;
            GLCD_SPI_SCB_scbEnableIntr = (uint8) config->enableInterrupt;

            /* Set RX direction internal variables */
            GLCD_SPI_SCB_rxBuffer      =         config->rxBuffer;
            GLCD_SPI_SCB_rxDataBits    = (uint8) config->rxDataBits;
            GLCD_SPI_SCB_rxBufferSize  = (uint8) config->rxBufferSize;

            /* Set TX direction internal variables */
            GLCD_SPI_SCB_txBuffer      =         config->txBuffer;
            GLCD_SPI_SCB_txDataBits    = (uint8) config->txDataBits;
            GLCD_SPI_SCB_txBufferSize  = (uint8) config->txBufferSize;

            /* Configure SPI interface */
            GLCD_SPI_SCB_CTRL_REG     = GLCD_SPI_SCB_GET_CTRL_OVS(config->oversample)           |
                                            GLCD_SPI_SCB_GET_CTRL_BYTE_MODE(config->enableByteMode) |
                                            GLCD_SPI_SCB_GET_CTRL_EC_AM_MODE(config->enableWake)    |
                                            GLCD_SPI_SCB_CTRL_SPI;

            GLCD_SPI_SCB_SPI_CTRL_REG = GLCD_SPI_SCB_GET_SPI_CTRL_CONTINUOUS    (config->transferSeperation)  |
                                            GLCD_SPI_SCB_GET_SPI_CTRL_SELECT_PRECEDE(config->submode &
                                                                          GLCD_SPI_SCB_SPI_MODE_TI_PRECEDES_MASK) |
                                            GLCD_SPI_SCB_GET_SPI_CTRL_SCLK_MODE     (config->sclkMode)            |
                                            GLCD_SPI_SCB_GET_SPI_CTRL_LATE_MISO_SAMPLE(config->enableLateSampling)|
                                            GLCD_SPI_SCB_GET_SPI_CTRL_SCLK_CONTINUOUS(config->enableFreeRunSclk)  |
                                            GLCD_SPI_SCB_GET_SPI_CTRL_SSEL_POLARITY (config->polaritySs)          |
                                            GLCD_SPI_SCB_GET_SPI_CTRL_SUB_MODE      (config->submode)             |
                                            GLCD_SPI_SCB_GET_SPI_CTRL_MASTER_MODE   (config->mode);

            /* Configure RX direction */
            GLCD_SPI_SCB_RX_CTRL_REG     =  GLCD_SPI_SCB_GET_RX_CTRL_DATA_WIDTH(config->rxDataBits)         |
                                                GLCD_SPI_SCB_GET_RX_CTRL_BIT_ORDER (config->bitOrder)           |
                                                GLCD_SPI_SCB_GET_RX_CTRL_MEDIAN    (config->enableMedianFilter) |
                                                GLCD_SPI_SCB_SPI_RX_CTRL;

            GLCD_SPI_SCB_RX_FIFO_CTRL_REG = GLCD_SPI_SCB_GET_RX_FIFO_CTRL_TRIGGER_LEVEL(config->rxTriggerLevel);

            /* Configure TX direction */
            GLCD_SPI_SCB_TX_CTRL_REG      = GLCD_SPI_SCB_GET_TX_CTRL_DATA_WIDTH(config->txDataBits) |
                                                GLCD_SPI_SCB_GET_TX_CTRL_BIT_ORDER (config->bitOrder)   |
                                                GLCD_SPI_SCB_SPI_TX_CTRL;

            GLCD_SPI_SCB_TX_FIFO_CTRL_REG = GLCD_SPI_SCB_GET_TX_FIFO_CTRL_TRIGGER_LEVEL(config->txTriggerLevel);

            /* Configure interrupt with SPI handler but do not enable it */
            CyIntDisable    (GLCD_SPI_SCB_ISR_NUMBER);
            CyIntSetPriority(GLCD_SPI_SCB_ISR_NUMBER, GLCD_SPI_SCB_ISR_PRIORITY);
            (void) CyIntSetVector(GLCD_SPI_SCB_ISR_NUMBER, &GLCD_SPI_SCB_SPI_UART_ISR);

            /* Configure interrupt sources */
            GLCD_SPI_SCB_INTR_I2C_EC_MASK_REG = GLCD_SPI_SCB_NO_INTR_SOURCES;
            GLCD_SPI_SCB_INTR_SPI_EC_MASK_REG = GLCD_SPI_SCB_NO_INTR_SOURCES;
            GLCD_SPI_SCB_INTR_SLAVE_MASK_REG  = GLCD_SPI_SCB_GET_SPI_INTR_SLAVE_MASK(config->rxInterruptMask);
            GLCD_SPI_SCB_INTR_MASTER_MASK_REG = GLCD_SPI_SCB_GET_SPI_INTR_MASTER_MASK(config->txInterruptMask);
            GLCD_SPI_SCB_INTR_RX_MASK_REG     = GLCD_SPI_SCB_GET_SPI_INTR_RX_MASK(config->rxInterruptMask);
            GLCD_SPI_SCB_INTR_TX_MASK_REG     = GLCD_SPI_SCB_GET_SPI_INTR_TX_MASK(config->txInterruptMask);
            
            /* Configure TX interrupt sources to restore. */
            GLCD_SPI_SCB_IntrTxMask = LO16(GLCD_SPI_SCB_INTR_TX_MASK_REG);

            /* Set active SS0 */
            GLCD_SPI_SCB_SpiSetActiveSlaveSelect(GLCD_SPI_SCB_SPI_SLAVE_SELECT0);

            /* Clear RX buffer indexes */
            GLCD_SPI_SCB_rxBufferHead     = 0u;
            GLCD_SPI_SCB_rxBufferTail     = 0u;
            GLCD_SPI_SCB_rxBufferOverflow = 0u;

            /* Clear TX buffer indexes */
            GLCD_SPI_SCB_txBufferHead = 0u;
            GLCD_SPI_SCB_txBufferTail = 0u;
        }
    }

#else

    /*******************************************************************************
    * Function Name: GLCD_SPI_SCB_SpiInit
    ****************************************************************************//**
    *
    *  Configures the SCB for the SPI operation.
    *
    *******************************************************************************/
    void GLCD_SPI_SCB_SpiInit(void)
    {
        /* Configure SPI interface */
        GLCD_SPI_SCB_CTRL_REG     = GLCD_SPI_SCB_SPI_DEFAULT_CTRL;
        GLCD_SPI_SCB_SPI_CTRL_REG = GLCD_SPI_SCB_SPI_DEFAULT_SPI_CTRL;

        /* Configure TX and RX direction */
        GLCD_SPI_SCB_RX_CTRL_REG      = GLCD_SPI_SCB_SPI_DEFAULT_RX_CTRL;
        GLCD_SPI_SCB_RX_FIFO_CTRL_REG = GLCD_SPI_SCB_SPI_DEFAULT_RX_FIFO_CTRL;

        /* Configure TX and RX direction */
        GLCD_SPI_SCB_TX_CTRL_REG      = GLCD_SPI_SCB_SPI_DEFAULT_TX_CTRL;
        GLCD_SPI_SCB_TX_FIFO_CTRL_REG = GLCD_SPI_SCB_SPI_DEFAULT_TX_FIFO_CTRL;

        /* Configure interrupt with SPI handler but do not enable it */
    #if(GLCD_SPI_SCB_SCB_IRQ_INTERNAL)
            CyIntDisable    (GLCD_SPI_SCB_ISR_NUMBER);
            CyIntSetPriority(GLCD_SPI_SCB_ISR_NUMBER, GLCD_SPI_SCB_ISR_PRIORITY);
            (void) CyIntSetVector(GLCD_SPI_SCB_ISR_NUMBER, &GLCD_SPI_SCB_SPI_UART_ISR);
    #endif /* (GLCD_SPI_SCB_SCB_IRQ_INTERNAL) */

        /* Configure interrupt sources */
        GLCD_SPI_SCB_INTR_I2C_EC_MASK_REG = GLCD_SPI_SCB_SPI_DEFAULT_INTR_I2C_EC_MASK;
        GLCD_SPI_SCB_INTR_SPI_EC_MASK_REG = GLCD_SPI_SCB_SPI_DEFAULT_INTR_SPI_EC_MASK;
        GLCD_SPI_SCB_INTR_SLAVE_MASK_REG  = GLCD_SPI_SCB_SPI_DEFAULT_INTR_SLAVE_MASK;
        GLCD_SPI_SCB_INTR_MASTER_MASK_REG = GLCD_SPI_SCB_SPI_DEFAULT_INTR_MASTER_MASK;
        GLCD_SPI_SCB_INTR_RX_MASK_REG     = GLCD_SPI_SCB_SPI_DEFAULT_INTR_RX_MASK;
        GLCD_SPI_SCB_INTR_TX_MASK_REG     = GLCD_SPI_SCB_SPI_DEFAULT_INTR_TX_MASK;

        /* Configure TX interrupt sources to restore. */
        GLCD_SPI_SCB_IntrTxMask = LO16(GLCD_SPI_SCB_INTR_TX_MASK_REG);
            
        /* Set active SS0 for master */
    #if (GLCD_SPI_SCB_SPI_MASTER_CONST)
        GLCD_SPI_SCB_SpiSetActiveSlaveSelect(GLCD_SPI_SCB_SPI_SLAVE_SELECT0);
    #endif /* (GLCD_SPI_SCB_SPI_MASTER_CONST) */

    #if(GLCD_SPI_SCB_INTERNAL_RX_SW_BUFFER_CONST)
        GLCD_SPI_SCB_rxBufferHead     = 0u;
        GLCD_SPI_SCB_rxBufferTail     = 0u;
        GLCD_SPI_SCB_rxBufferOverflow = 0u;
    #endif /* (GLCD_SPI_SCB_INTERNAL_RX_SW_BUFFER_CONST) */

    #if(GLCD_SPI_SCB_INTERNAL_TX_SW_BUFFER_CONST)
        GLCD_SPI_SCB_txBufferHead = 0u;
        GLCD_SPI_SCB_txBufferTail = 0u;
    #endif /* (GLCD_SPI_SCB_INTERNAL_TX_SW_BUFFER_CONST) */
    }
#endif /* (GLCD_SPI_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */


/*******************************************************************************
* Function Name: GLCD_SPI_SCB_SpiPostEnable
****************************************************************************//**
*
*  Restores HSIOM settings for the SPI master output pins (SCLK and/or SS0-SS3) 
*  to be controlled by the SCB SPI.
*
*******************************************************************************/
void GLCD_SPI_SCB_SpiPostEnable(void)
{
#if(GLCD_SPI_SCB_SCB_MODE_UNCONFIG_CONST_CFG)

    if (GLCD_SPI_SCB_CHECK_SPI_MASTER)
    {
    #if (GLCD_SPI_SCB_SCLK_PIN)
        /* Set SCB SPI to drive output pin */
        GLCD_SPI_SCB_SET_HSIOM_SEL(GLCD_SPI_SCB_SCLK_HSIOM_REG, GLCD_SPI_SCB_SCLK_HSIOM_MASK,
                                       GLCD_SPI_SCB_SCLK_HSIOM_POS, GLCD_SPI_SCB_SCLK_HSIOM_SEL_SPI);
    #endif /* (GLCD_SPI_SCB_SCLK_PIN) */

    #if (GLCD_SPI_SCB_SS0_PIN)
        /* Set SCB SPI to drive output pin */
        GLCD_SPI_SCB_SET_HSIOM_SEL(GLCD_SPI_SCB_SS0_HSIOM_REG, GLCD_SPI_SCB_SS0_HSIOM_MASK,
                                       GLCD_SPI_SCB_SS0_HSIOM_POS, GLCD_SPI_SCB_SS0_HSIOM_SEL_SPI);
    #endif /* (GLCD_SPI_SCB_SS0_PIN) */

    #if (GLCD_SPI_SCB_SS1_PIN)
        /* Set SCB SPI to drive output pin */
        GLCD_SPI_SCB_SET_HSIOM_SEL(GLCD_SPI_SCB_SS1_HSIOM_REG, GLCD_SPI_SCB_SS1_HSIOM_MASK,
                                       GLCD_SPI_SCB_SS1_HSIOM_POS, GLCD_SPI_SCB_SS1_HSIOM_SEL_SPI);
    #endif /* (GLCD_SPI_SCB_SS1_PIN) */

    #if (GLCD_SPI_SCB_SS2_PIN)
        /* Set SCB SPI to drive output pin */
        GLCD_SPI_SCB_SET_HSIOM_SEL(GLCD_SPI_SCB_SS2_HSIOM_REG, GLCD_SPI_SCB_SS2_HSIOM_MASK,
                                       GLCD_SPI_SCB_SS2_HSIOM_POS, GLCD_SPI_SCB_SS2_HSIOM_SEL_SPI);
    #endif /* (GLCD_SPI_SCB_SS2_PIN) */

    #if (GLCD_SPI_SCB_SS3_PIN)
        /* Set SCB SPI to drive output pin */
        GLCD_SPI_SCB_SET_HSIOM_SEL(GLCD_SPI_SCB_SS3_HSIOM_REG, GLCD_SPI_SCB_SS3_HSIOM_MASK,
                                       GLCD_SPI_SCB_SS3_HSIOM_POS, GLCD_SPI_SCB_SS3_HSIOM_SEL_SPI);
    #endif /* (GLCD_SPI_SCB_SS3_PIN) */
    }

#else

    #if (GLCD_SPI_SCB_SPI_MASTER_SCLK_PIN)
        /* Set SCB SPI to drive output pin */
        GLCD_SPI_SCB_SET_HSIOM_SEL(GLCD_SPI_SCB_SCLK_M_HSIOM_REG, GLCD_SPI_SCB_SCLK_M_HSIOM_MASK,
                                       GLCD_SPI_SCB_SCLK_M_HSIOM_POS, GLCD_SPI_SCB_SCLK_M_HSIOM_SEL_SPI);
    #endif /* (GLCD_SPI_SCB_MISO_SDA_TX_PIN_PIN) */

    #if (GLCD_SPI_SCB_SPI_MASTER_SS0_PIN)
        /* Set SCB SPI to drive output pin */
        GLCD_SPI_SCB_SET_HSIOM_SEL(GLCD_SPI_SCB_SS0_M_HSIOM_REG, GLCD_SPI_SCB_SS0_M_HSIOM_MASK,
                                       GLCD_SPI_SCB_SS0_M_HSIOM_POS, GLCD_SPI_SCB_SS0_M_HSIOM_SEL_SPI);
    #endif /* (GLCD_SPI_SCB_SPI_MASTER_SS0_PIN) */

    #if (GLCD_SPI_SCB_SPI_MASTER_SS1_PIN)
        /* Set SCB SPI to drive output pin */
        GLCD_SPI_SCB_SET_HSIOM_SEL(GLCD_SPI_SCB_SS1_M_HSIOM_REG, GLCD_SPI_SCB_SS1_M_HSIOM_MASK,
                                       GLCD_SPI_SCB_SS1_M_HSIOM_POS, GLCD_SPI_SCB_SS1_M_HSIOM_SEL_SPI);
    #endif /* (GLCD_SPI_SCB_SPI_MASTER_SS1_PIN) */

    #if (GLCD_SPI_SCB_SPI_MASTER_SS2_PIN)
        /* Set SCB SPI to drive output pin */
        GLCD_SPI_SCB_SET_HSIOM_SEL(GLCD_SPI_SCB_SS2_M_HSIOM_REG, GLCD_SPI_SCB_SS2_M_HSIOM_MASK,
                                       GLCD_SPI_SCB_SS2_M_HSIOM_POS, GLCD_SPI_SCB_SS2_M_HSIOM_SEL_SPI);
    #endif /* (GLCD_SPI_SCB_SPI_MASTER_SS2_PIN) */

    #if (GLCD_SPI_SCB_SPI_MASTER_SS3_PIN)
        /* Set SCB SPI to drive output pin */
        GLCD_SPI_SCB_SET_HSIOM_SEL(GLCD_SPI_SCB_SS3_M_HSIOM_REG, GLCD_SPI_SCB_SS3_M_HSIOM_MASK,
                                       GLCD_SPI_SCB_SS3_M_HSIOM_POS, GLCD_SPI_SCB_SS3_M_HSIOM_SEL_SPI);
    #endif /* (GLCD_SPI_SCB_SPI_MASTER_SS3_PIN) */

#endif /* (GLCD_SPI_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */

    /* Restore TX interrupt sources. */
    GLCD_SPI_SCB_SetTxInterruptMode(GLCD_SPI_SCB_IntrTxMask);
}


/*******************************************************************************
* Function Name: GLCD_SPI_SCB_SpiStop
****************************************************************************//**
*
*  Changes the HSIOM settings for the SPI master output pins 
*  (SCLK and/or SS0-SS3) to keep them inactive after the block is disabled. 
*  The output pins are controlled by the GPIO data register.
*
*******************************************************************************/
void GLCD_SPI_SCB_SpiStop(void)
{
#if(GLCD_SPI_SCB_SCB_MODE_UNCONFIG_CONST_CFG)

    if (GLCD_SPI_SCB_CHECK_SPI_MASTER)
    {
    #if (GLCD_SPI_SCB_SCLK_PIN)
        /* Set output pin state after block is disabled */
        GLCD_SPI_SCB_spi_sclk_Write(GLCD_SPI_SCB_GET_SPI_SCLK_INACTIVE);

        /* Set GPIO to drive output pin */
        GLCD_SPI_SCB_SET_HSIOM_SEL(GLCD_SPI_SCB_SCLK_HSIOM_REG, GLCD_SPI_SCB_SCLK_HSIOM_MASK,
                                       GLCD_SPI_SCB_SCLK_HSIOM_POS, GLCD_SPI_SCB_SCLK_HSIOM_SEL_GPIO);
    #endif /* (GLCD_SPI_SCB_spi_sclk_PIN) */

    #if (GLCD_SPI_SCB_SS0_PIN)
        /* Set output pin state after block is disabled */
        GLCD_SPI_SCB_spi_ss0_Write(GLCD_SPI_SCB_GET_SPI_SS0_INACTIVE);

        /* Set GPIO to drive output pin */
        GLCD_SPI_SCB_SET_HSIOM_SEL(GLCD_SPI_SCB_SS0_HSIOM_REG, GLCD_SPI_SCB_SS0_HSIOM_MASK,
                                       GLCD_SPI_SCB_SS0_HSIOM_POS, GLCD_SPI_SCB_SS0_HSIOM_SEL_GPIO);
    #endif /* (GLCD_SPI_SCB_spi_ss0_PIN) */

    #if (GLCD_SPI_SCB_SS1_PIN)
        /* Set output pin state after block is disabled */
        GLCD_SPI_SCB_spi_ss1_Write(GLCD_SPI_SCB_GET_SPI_SS1_INACTIVE);

        /* Set GPIO to drive output pin */
        GLCD_SPI_SCB_SET_HSIOM_SEL(GLCD_SPI_SCB_SS1_HSIOM_REG, GLCD_SPI_SCB_SS1_HSIOM_MASK,
                                       GLCD_SPI_SCB_SS1_HSIOM_POS, GLCD_SPI_SCB_SS1_HSIOM_SEL_GPIO);
    #endif /* (GLCD_SPI_SCB_SS1_PIN) */

    #if (GLCD_SPI_SCB_SS2_PIN)
        /* Set output pin state after block is disabled */
        GLCD_SPI_SCB_spi_ss2_Write(GLCD_SPI_SCB_GET_SPI_SS2_INACTIVE);

        /* Set GPIO to drive output pin */
        GLCD_SPI_SCB_SET_HSIOM_SEL(GLCD_SPI_SCB_SS2_HSIOM_REG, GLCD_SPI_SCB_SS2_HSIOM_MASK,
                                       GLCD_SPI_SCB_SS2_HSIOM_POS, GLCD_SPI_SCB_SS2_HSIOM_SEL_GPIO);
    #endif /* (GLCD_SPI_SCB_SS2_PIN) */

    #if (GLCD_SPI_SCB_SS3_PIN)
        /* Set output pin state after block is disabled */
        GLCD_SPI_SCB_spi_ss3_Write(GLCD_SPI_SCB_GET_SPI_SS3_INACTIVE);

        /* Set GPIO to drive output pin */
        GLCD_SPI_SCB_SET_HSIOM_SEL(GLCD_SPI_SCB_SS3_HSIOM_REG, GLCD_SPI_SCB_SS3_HSIOM_MASK,
                                       GLCD_SPI_SCB_SS3_HSIOM_POS, GLCD_SPI_SCB_SS3_HSIOM_SEL_GPIO);
    #endif /* (GLCD_SPI_SCB_SS3_PIN) */
    
        /* Store TX interrupt sources (exclude level triggered) for master. */
        GLCD_SPI_SCB_IntrTxMask = LO16(GLCD_SPI_SCB_GetTxInterruptMode() & GLCD_SPI_SCB_INTR_SPIM_TX_RESTORE);
    }
    else
    {
        /* Store TX interrupt sources (exclude level triggered) for slave. */
        GLCD_SPI_SCB_IntrTxMask = LO16(GLCD_SPI_SCB_GetTxInterruptMode() & GLCD_SPI_SCB_INTR_SPIS_TX_RESTORE);
    }

#else

#if (GLCD_SPI_SCB_SPI_MASTER_SCLK_PIN)
    /* Set output pin state after block is disabled */
    GLCD_SPI_SCB_sclk_m_Write(GLCD_SPI_SCB_GET_SPI_SCLK_INACTIVE);

    /* Set GPIO to drive output pin */
    GLCD_SPI_SCB_SET_HSIOM_SEL(GLCD_SPI_SCB_SCLK_M_HSIOM_REG, GLCD_SPI_SCB_SCLK_M_HSIOM_MASK,
                                   GLCD_SPI_SCB_SCLK_M_HSIOM_POS, GLCD_SPI_SCB_SCLK_M_HSIOM_SEL_GPIO);
#endif /* (GLCD_SPI_SCB_MISO_SDA_TX_PIN_PIN) */

#if (GLCD_SPI_SCB_SPI_MASTER_SS0_PIN)
    /* Set output pin state after block is disabled */
    GLCD_SPI_SCB_ss0_m_Write(GLCD_SPI_SCB_GET_SPI_SS0_INACTIVE);

    /* Set GPIO to drive output pin */
    GLCD_SPI_SCB_SET_HSIOM_SEL(GLCD_SPI_SCB_SS0_M_HSIOM_REG, GLCD_SPI_SCB_SS0_M_HSIOM_MASK,
                                   GLCD_SPI_SCB_SS0_M_HSIOM_POS, GLCD_SPI_SCB_SS0_M_HSIOM_SEL_GPIO);
#endif /* (GLCD_SPI_SCB_SPI_MASTER_SS0_PIN) */

#if (GLCD_SPI_SCB_SPI_MASTER_SS1_PIN)
    /* Set output pin state after block is disabled */
    GLCD_SPI_SCB_ss1_m_Write(GLCD_SPI_SCB_GET_SPI_SS1_INACTIVE);

    /* Set GPIO to drive output pin */
    GLCD_SPI_SCB_SET_HSIOM_SEL(GLCD_SPI_SCB_SS1_M_HSIOM_REG, GLCD_SPI_SCB_SS1_M_HSIOM_MASK,
                                   GLCD_SPI_SCB_SS1_M_HSIOM_POS, GLCD_SPI_SCB_SS1_M_HSIOM_SEL_GPIO);
#endif /* (GLCD_SPI_SCB_SPI_MASTER_SS1_PIN) */

#if (GLCD_SPI_SCB_SPI_MASTER_SS2_PIN)
    /* Set output pin state after block is disabled */
    GLCD_SPI_SCB_ss2_m_Write(GLCD_SPI_SCB_GET_SPI_SS2_INACTIVE);

    /* Set GPIO to drive output pin */
    GLCD_SPI_SCB_SET_HSIOM_SEL(GLCD_SPI_SCB_SS2_M_HSIOM_REG, GLCD_SPI_SCB_SS2_M_HSIOM_MASK,
                                   GLCD_SPI_SCB_SS2_M_HSIOM_POS, GLCD_SPI_SCB_SS2_M_HSIOM_SEL_GPIO);
#endif /* (GLCD_SPI_SCB_SPI_MASTER_SS2_PIN) */

#if (GLCD_SPI_SCB_SPI_MASTER_SS3_PIN)
    /* Set output pin state after block is disabled */
    GLCD_SPI_SCB_ss3_m_Write(GLCD_SPI_SCB_GET_SPI_SS3_INACTIVE);

    /* Set GPIO to drive output pin */
    GLCD_SPI_SCB_SET_HSIOM_SEL(GLCD_SPI_SCB_SS3_M_HSIOM_REG, GLCD_SPI_SCB_SS3_M_HSIOM_MASK,
                                   GLCD_SPI_SCB_SS3_M_HSIOM_POS, GLCD_SPI_SCB_SS3_M_HSIOM_SEL_GPIO);
#endif /* (GLCD_SPI_SCB_SPI_MASTER_SS3_PIN) */

    #if (GLCD_SPI_SCB_SPI_MASTER_CONST)
        /* Store TX interrupt sources (exclude level triggered). */
        GLCD_SPI_SCB_IntrTxMask = LO16(GLCD_SPI_SCB_GetTxInterruptMode() & GLCD_SPI_SCB_INTR_SPIM_TX_RESTORE);
    #else
        /* Store TX interrupt sources (exclude level triggered). */
        GLCD_SPI_SCB_IntrTxMask = LO16(GLCD_SPI_SCB_GetTxInterruptMode() & GLCD_SPI_SCB_INTR_SPIS_TX_RESTORE);
    #endif /* (GLCD_SPI_SCB_SPI_MASTER_CONST) */

#endif /* (GLCD_SPI_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */
}


#if (GLCD_SPI_SCB_SPI_MASTER_CONST)
    /*******************************************************************************
    * Function Name: GLCD_SPI_SCB_SetActiveSlaveSelect
    ****************************************************************************//**
    *
    *  Selects one of the four slave select lines to be active during the transfer.
    *  After initialization the active slave select line is 0.
    *  The component should be in one of the following states to change the active
    *  slave select signal source correctly:
    *   - The component is disabled.
    *   - The component has completed transfer (TX FIFO is empty and the
    *     SCB_INTR_MASTER_SPI_DONE status is set).
    *
    *  This function does not check that these conditions are met.
    *  This function is only applicable to SPI Master mode of operation.
    *
    *  \param slaveSelect: slave select line which will be active while the following
    *   transfer.
    *   - GLCD_SPI_SCB_SPI_SLAVE_SELECT0 - Slave select 0.
    *   - GLCD_SPI_SCB_SPI_SLAVE_SELECT1 - Slave select 1.
    *   - GLCD_SPI_SCB_SPI_SLAVE_SELECT2 - Slave select 2.
    *   - GLCD_SPI_SCB_SPI_SLAVE_SELECT3 - Slave select 3.
    *
    *******************************************************************************/
    void GLCD_SPI_SCB_SpiSetActiveSlaveSelect(uint32 slaveSelect)
    {
        uint32 spiCtrl;

        spiCtrl = GLCD_SPI_SCB_SPI_CTRL_REG;

        spiCtrl &= (uint32) ~GLCD_SPI_SCB_SPI_CTRL_SLAVE_SELECT_MASK;
        spiCtrl |= (uint32)  GLCD_SPI_SCB_GET_SPI_CTRL_SS(slaveSelect);

        GLCD_SPI_SCB_SPI_CTRL_REG = spiCtrl;
    }
#endif /* (GLCD_SPI_SCB_SPI_MASTER_CONST) */


#if !(GLCD_SPI_SCB_CY_SCBIP_V0 || GLCD_SPI_SCB_CY_SCBIP_V1)
    /*******************************************************************************
    * Function Name: GLCD_SPI_SCB_SpiSetSlaveSelectPolarity
    ****************************************************************************//**
    *
    *  Sets active polarity for slave select line.
    *  The component should be in one of the following states to change the active
    *  slave select signal source correctly:
    *   - The component is disabled.
    *   - The component has completed transfer.
    *  
    *  This function does not check that these conditions are met.
    *
    *  \param slaveSelect: slave select line to change active polarity.
    *   - GLCD_SPI_SCB_SPI_SLAVE_SELECT0 - Slave select 0.
    *   - GLCD_SPI_SCB_SPI_SLAVE_SELECT1 - Slave select 1.
    *   - GLCD_SPI_SCB_SPI_SLAVE_SELECT2 - Slave select 2.
    *   - GLCD_SPI_SCB_SPI_SLAVE_SELECT3 - Slave select 3.
    *
    *  \param polarity: active polarity of slave select line.
    *   - GLCD_SPI_SCB_SPI_SS_ACTIVE_LOW  - Slave select is active low.
    *   - GLCD_SPI_SCB_SPI_SS_ACTIVE_HIGH - Slave select is active high.
    *
    *******************************************************************************/
    void GLCD_SPI_SCB_SpiSetSlaveSelectPolarity(uint32 slaveSelect, uint32 polarity)
    {
        uint32 ssPolarity;

        /* Get position of the polarity bit associated with slave select line */
        ssPolarity = GLCD_SPI_SCB_GET_SPI_CTRL_SSEL_POLARITY((uint32) 1u << slaveSelect);

        if (0u != polarity)
        {
            GLCD_SPI_SCB_SPI_CTRL_REG |= (uint32)  ssPolarity;
        }
        else
        {
            GLCD_SPI_SCB_SPI_CTRL_REG &= (uint32) ~ssPolarity;
        }
    }
#endif /* !(GLCD_SPI_SCB_CY_SCBIP_V0 || GLCD_SPI_SCB_CY_SCBIP_V1) */


#if(GLCD_SPI_SCB_SPI_WAKE_ENABLE_CONST)
    /*******************************************************************************
    * Function Name: GLCD_SPI_SCB_SpiSaveConfig
    ****************************************************************************//**
    *
    *  Clears INTR_SPI_EC.WAKE_UP and enables it. This interrupt
    *  source triggers when the master assigns the SS line and wakes up the device.
    *
    *******************************************************************************/
    void GLCD_SPI_SCB_SpiSaveConfig(void)
    {
        /* Clear and enable SPI wakeup interrupt source */
        GLCD_SPI_SCB_ClearSpiExtClkInterruptSource(GLCD_SPI_SCB_INTR_SPI_EC_WAKE_UP);
        GLCD_SPI_SCB_SetSpiExtClkInterruptMode(GLCD_SPI_SCB_INTR_SPI_EC_WAKE_UP);
    }


    /*******************************************************************************
    * Function Name: GLCD_SPI_SCB_SpiRestoreConfig
    ****************************************************************************//**
    *
    *  Disables the INTR_SPI_EC.WAKE_UP interrupt source. After wakeup
    *  slave does not drive the MISO line and the master receives 0xFF.
    *
    *******************************************************************************/
    void GLCD_SPI_SCB_SpiRestoreConfig(void)
    {
        /* Disable SPI wakeup interrupt source */
        GLCD_SPI_SCB_SetSpiExtClkInterruptMode(GLCD_SPI_SCB_NO_INTR_SOURCES);
    }
#endif /* (GLCD_SPI_SCB_SPI_WAKE_ENABLE_CONST) */


/* [] END OF FILE */
