/*******************************************************************************
* File Name: GLCD_SPI_SCB_miso_s.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_GLCD_SPI_SCB_miso_s_H) /* Pins GLCD_SPI_SCB_miso_s_H */
#define CY_PINS_GLCD_SPI_SCB_miso_s_H

#include "cytypes.h"
#include "cyfitter.h"
#include "GLCD_SPI_SCB_miso_s_aliases.h"


/***************************************
*     Data Struct Definitions
***************************************/

/**
* \addtogroup group_structures
* @{
*/
    
/* Structure for sleep mode support */
typedef struct
{
    uint32 pcState; /**< State of the port control register */
    uint32 sioState; /**< State of the SIO configuration */
    uint32 usbState; /**< State of the USBIO regulator */
} GLCD_SPI_SCB_miso_s_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   GLCD_SPI_SCB_miso_s_Read(void);
void    GLCD_SPI_SCB_miso_s_Write(uint8 value);
uint8   GLCD_SPI_SCB_miso_s_ReadDataReg(void);
#if defined(GLCD_SPI_SCB_miso_s__PC) || (CY_PSOC4_4200L) 
    void    GLCD_SPI_SCB_miso_s_SetDriveMode(uint8 mode);
#endif
void    GLCD_SPI_SCB_miso_s_SetInterruptMode(uint16 position, uint16 mode);
uint8   GLCD_SPI_SCB_miso_s_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void GLCD_SPI_SCB_miso_s_Sleep(void); 
void GLCD_SPI_SCB_miso_s_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(GLCD_SPI_SCB_miso_s__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define GLCD_SPI_SCB_miso_s_DRIVE_MODE_BITS        (3)
    #define GLCD_SPI_SCB_miso_s_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - GLCD_SPI_SCB_miso_s_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the GLCD_SPI_SCB_miso_s_SetDriveMode() function.
         *  @{
         */
        #define GLCD_SPI_SCB_miso_s_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define GLCD_SPI_SCB_miso_s_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define GLCD_SPI_SCB_miso_s_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define GLCD_SPI_SCB_miso_s_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define GLCD_SPI_SCB_miso_s_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define GLCD_SPI_SCB_miso_s_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define GLCD_SPI_SCB_miso_s_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define GLCD_SPI_SCB_miso_s_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define GLCD_SPI_SCB_miso_s_MASK               GLCD_SPI_SCB_miso_s__MASK
#define GLCD_SPI_SCB_miso_s_SHIFT              GLCD_SPI_SCB_miso_s__SHIFT
#define GLCD_SPI_SCB_miso_s_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in GLCD_SPI_SCB_miso_s_SetInterruptMode() function.
     *  @{
     */
        #define GLCD_SPI_SCB_miso_s_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define GLCD_SPI_SCB_miso_s_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define GLCD_SPI_SCB_miso_s_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define GLCD_SPI_SCB_miso_s_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(GLCD_SPI_SCB_miso_s__SIO)
    #define GLCD_SPI_SCB_miso_s_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(GLCD_SPI_SCB_miso_s__PC) && (CY_PSOC4_4200L)
    #define GLCD_SPI_SCB_miso_s_USBIO_ENABLE               ((uint32)0x80000000u)
    #define GLCD_SPI_SCB_miso_s_USBIO_DISABLE              ((uint32)(~GLCD_SPI_SCB_miso_s_USBIO_ENABLE))
    #define GLCD_SPI_SCB_miso_s_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define GLCD_SPI_SCB_miso_s_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define GLCD_SPI_SCB_miso_s_USBIO_ENTER_SLEEP          ((uint32)((1u << GLCD_SPI_SCB_miso_s_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << GLCD_SPI_SCB_miso_s_USBIO_SUSPEND_DEL_SHIFT)))
    #define GLCD_SPI_SCB_miso_s_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << GLCD_SPI_SCB_miso_s_USBIO_SUSPEND_SHIFT)))
    #define GLCD_SPI_SCB_miso_s_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << GLCD_SPI_SCB_miso_s_USBIO_SUSPEND_DEL_SHIFT)))
    #define GLCD_SPI_SCB_miso_s_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(GLCD_SPI_SCB_miso_s__PC)
    /* Port Configuration */
    #define GLCD_SPI_SCB_miso_s_PC                 (* (reg32 *) GLCD_SPI_SCB_miso_s__PC)
#endif
/* Pin State */
#define GLCD_SPI_SCB_miso_s_PS                     (* (reg32 *) GLCD_SPI_SCB_miso_s__PS)
/* Data Register */
#define GLCD_SPI_SCB_miso_s_DR                     (* (reg32 *) GLCD_SPI_SCB_miso_s__DR)
/* Input Buffer Disable Override */
#define GLCD_SPI_SCB_miso_s_INP_DIS                (* (reg32 *) GLCD_SPI_SCB_miso_s__PC2)

/* Interrupt configuration Registers */
#define GLCD_SPI_SCB_miso_s_INTCFG                 (* (reg32 *) GLCD_SPI_SCB_miso_s__INTCFG)
#define GLCD_SPI_SCB_miso_s_INTSTAT                (* (reg32 *) GLCD_SPI_SCB_miso_s__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define GLCD_SPI_SCB_miso_s_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(GLCD_SPI_SCB_miso_s__SIO)
    #define GLCD_SPI_SCB_miso_s_SIO_REG            (* (reg32 *) GLCD_SPI_SCB_miso_s__SIO)
#endif /* (GLCD_SPI_SCB_miso_s__SIO_CFG) */

/* USBIO registers */
#if !defined(GLCD_SPI_SCB_miso_s__PC) && (CY_PSOC4_4200L)
    #define GLCD_SPI_SCB_miso_s_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define GLCD_SPI_SCB_miso_s_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define GLCD_SPI_SCB_miso_s_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define GLCD_SPI_SCB_miso_s_DRIVE_MODE_SHIFT       (0x00u)
#define GLCD_SPI_SCB_miso_s_DRIVE_MODE_MASK        (0x07u << GLCD_SPI_SCB_miso_s_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins GLCD_SPI_SCB_miso_s_H */


/* [] END OF FILE */
