/*
 * -------------------------------------------
 *    MSP432 DriverLib - v4_00_00_11
 * -------------------------------------------
 *
 * --COPYRIGHT--,BSD,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/*******************************************************************************
 * MSP432 ADC14 - Multiple Channel Sample without Repeat
 *
 * Description: In this code example, the feature of being able to scan multiple
 * ADC channels is demonstrated by the user a the DriverLib APIs.  Conversion
 * memory registers ADC_MEM0 - ADC_MEM3 are configured to read conversion
 * results from A6, A12, A10, A8 respectively. Conversion is enabled and then sampling is
 * toggled using a software toggle. Repeat mode is not enabled and sampling only
 * occurs once (and it is expected that the user pauses the debugger to observe
 * the results). Once the final sample has been taken, the interrupt for
 * ADC_MEM3 is triggered and the result is stored in the resultsBuffer buffer.
 *
 *                MSP432P401
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST         P4.0  |<--- A13 (Analog Input, Measured)
 *            |            P6.1  |<--- A14 (Analog Input, Reference)
 *            |                  |
 *            |                  |
 *
 *
 *
 * Author: Timothy Logan
 * This was modified by Rob Frohne to do multiple ADC at 8 kHz sample rate,
 * and in concert with Energia.
 ******************************************************************************/
#include "adc14vna.h"
/* DriverLib Includes */
#include <driverlib.h>
#include "msp.h"

/* Standard Includes */
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdio.h>


/*
 * TA0 PWM Configuration
 * Timer used to Trigger ADC
 * SMCLK -> SMCLK_FREQUENCY
 * Sample Frequency -> SAMPLE_FREQUENCY
 */
const Timer_A_PWMConfig pwmConfig =
{
        TIMER_A_CLOCKSOURCE_SMCLK,              // 48Mhz
        TIMER_A_CLOCKSOURCE_DIVIDER_1,          // 48Mhz
        (SMCLK_FREQUENCY/SAMPLE_FREQUENCY),     // 60
        TIMER_A_CAPTURECOMPARE_REGISTER_1,      // CCR1
        TIMER_A_OUTPUTMODE_SET_RESET,
        (SMCLK_FREQUENCY/SAMPLE_FREQUENCY)/2    // 30
};

extern volatile uint16_t ref[SAMPLE_LENGTH];
extern volatile uint16_t meas[SAMPLE_LENGTH];
extern volatile bool doneADC = false;

static volatile int refSampleCount;
static volatile int measSampleCount;

/* DMA Control Table */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_ALIGN(MSP_EXP432P401RLP_DMAControlTable, 1024)
#elif defined(__IAR_SYSTEMS_ICC__)
#pragma data_alignment=1024
#elif defined(__GNUC__)
__attribute__ ((aligned (1024)))
#elif defined(__CC_ARM)
__align(1024)
#endif
static DMA_ControlTable MSP_EXP432P401RLP_DMAControlTable[16];

void startConversion(void)
{
    refSampleCount = 0;
    measSampleCount = 0;
    doneADC = false;
    MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);
}

int adc14_main(void)
{
	int i;

    MAP_Interrupt_disableMaster();

    /* Zero-filling buffer */
	// Initialize results arrays and done flag.
    memset(ref,0x0000,SAMPLE_LENGTH);
    memset(meas,0x0000,SAMPLE_LENGTH);
	doneADC = false;

	/*
	 * Revision C silicon supports wait states of 1 at 48Mhz
	 */
	MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);
	MAP_FlashCtl_setWaitState(FLASH_BANK0, 1);
	MAP_FlashCtl_setWaitState(FLASH_BANK1, 1);

	/* Configuring DMA module */
	MAP_DMA_enableModule();
	MAP_DMA_setControlBase(MSP_EXP432P401RLP_DMAControlTable);

    /* Setting Control Indexes. In this case we will set the source of the
     * DMA transfer to ADC14 Memory 6
     *  and the destination to the
     * destination data array. */
    MAP_DMA_setChannelControl(UDMA_PRI_SELECT | DMA_CH0_RESERVED0,
        UDMA_SIZE_16 | UDMA_SRC_INC_NONE | UDMA_DST_INC_16 | UDMA_ARB_1);
    MAP_DMA_setChannelTransfer(UDMA_PRI_SELECT | DMA_CH0_RESERVED0,
        UDMA_MODE_PINGPONG, (void*) &ADC14->MEM[6],
        ref, 1024);

    MAP_DMA_setChannelControl(UDMA_ALT_SELECT | DMA_CH0_RESERVED0,
        UDMA_SIZE_16 | UDMA_SRC_INC_NONE | UDMA_DST_INC_16 | UDMA_ARB_1);
    MAP_DMA_setChannelTransfer(UDMA_ALT_SELECT | DMA_CH0_RESERVED0,
        UDMA_MODE_PINGPONG, (void*) &ADC14->MEM[6],
        (void*)&ref[1024], 1024);

    /* Setting Control Indexes. In this case we will set the source of the
     * DMA transfer to ADC14 Memory 7
     *  and the destination to the
     * destination data array. */
    MAP_DMA_setChannelControl(UDMA_PRI_SELECT | DMA_CH1_RESERVED0,
        UDMA_SIZE_16 | UDMA_SRC_INC_NONE | UDMA_DST_INC_16 | UDMA_ARB_1);
    MAP_DMA_setChannelTransfer(UDMA_PRI_SELECT | DMA_CH1_RESERVED0,
        UDMA_MODE_PINGPONG, (void*) &ADC14->MEM[7],
        meas, 1024);

    MAP_DMA_setChannelControl(UDMA_ALT_SELECT | DMA_CH1_RESERVED0,
        UDMA_SIZE_16 | UDMA_SRC_INC_NONE | UDMA_DST_INC_16 | UDMA_ARB_1);
    MAP_DMA_setChannelTransfer(UDMA_ALT_SELECT | DMA_CH1_RESERVED0,
        UDMA_MODE_PINGPONG, (void*) &ADC14->MEM[7],
        (void*)&meas[1024], 1024);

    /* Assigning/Enabling Interrupts */
    MAP_DMA_assignInterrupt(DMA_INT1, 0);
    MAP_DMA_assignInterrupt(DMA_INT2, 1);

    MAP_DMA_clearInterruptFlag(0);
    MAP_DMA_clearInterruptFlag(1);

    MAP_Interrupt_enableInterrupt(INT_DMA_INT1);
    MAP_Interrupt_enableInterrupt(INT_DMA_INT2);
    MAP_DMA_enableChannel(0);
    MAP_DMA_enableChannel(1);

    /* Setting reference voltage to 2.5 and enabling reference */
    MAP_REF_A_setReferenceVoltage(REF_A_VREF2_5V);
    MAP_REF_A_enableReferenceVoltage();

    /* Initializing ADC (MCLK/1/1) */
    MAP_ADC14_enableModule();
    MAP_ADC14_initModule(ADC_CLOCKSOURCE_ADCOSC, ADC_PREDIVIDER_1, ADC_DIVIDER_1,
            0);

    /*
	 * Configuring GPIOs (4.0 A13, 6.1 A14)
	 */
	MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN0,
		GPIO_TERTIARY_MODULE_FUNCTION);
	MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN1,
		GPIO_TERTIARY_MODULE_FUNCTION);
	/*
	 * Debug: set TA0.1 as output to see ADC trigger signal
	 */
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4,
		GPIO_PRIMARY_MODULE_FUNCTION);

	/* Configuring ADC Memory (ADC_MEM6 - ADC_MEM7 (A6 - A7)  without repeat)
	 * with internal 2.5v reference */
	MAP_ADC14_configureMultiSequenceMode(ADC_MEM6, ADC_MEM7, false);
	MAP_ADC14_configureConversionMemory(ADC_MEM6,
			ADC_VREFPOS_INTBUF_VREFNEG_VSS,
			ADC_INPUT_A14, ADC_NONDIFFERENTIAL_INPUTS);
	MAP_ADC14_configureConversionMemory(ADC_MEM7,
			ADC_VREFPOS_INTBUF_VREFNEG_VSS,
			ADC_INPUT_A13, ADC_NONDIFFERENTIAL_INPUTS);

    /*
     * Configuring the sample trigger to be sourced from Timer_A0 CCR1
     * and setting it to automatic iteration after it is triggered
     */
    MAP_ADC14_setSampleHoldTrigger(ADC_TRIGGER_SOURCE1, false);

    /*
     * Setting up the sample timer to automatically step through the sequence
     * convert.
     */
    MAP_ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);

    /*
	 * Enabling the interrupt when a conversion on channel 7
	 * and enabling conversions
	 */
	MAP_ADC14_enableInterrupt(ADC_INT7);
    MAP_ADC14_enableConversion();
    /*
     * Clear IFGs before enabling interrupt
     */
    MAP_ADC14_clearInterruptFlag(0xFFFFFFFFFFFFFFFF);
    MAP_Interrupt_enableInterrupt(INT_ADC14);

    // Register interrupt (sets up IRQ vectors) using TI-RTOS which Energia uses.
    Hwi_Params params;
    Hwi_Params_init(&params);
    Hwi_create(INT_ADC14, ADC14_IRQHandler, &params, 0);
    Hwi_create(DMA_INT1, DMA_INT1_IRQHandler, &params, 0);
    Hwi_create(DMA_INT2, DMA_INT2_IRQHandler, &params, 0);

	Hwi_setPriority(DMA_INT1, 58);
	Hwi_setPriority(DMA_INT2, 59);
    Hwi_setPriority(INT_ADC14, 60);

    MAP_Interrupt_enableMaster();
}


/*
 * This interrupt is fired whenever the sequence is completed
 *
 */
__attribute__((ramfunc))
void ADC14_IRQHandler(void)
{
	uint64_t status;
	// Turn on LED

	//MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
//    BITBAND_PERI(P1->OUT, 0) = 1;

	status = MAP_ADC14_getEnabledInterruptStatus();
	MAP_ADC14_clearInterruptFlag(status);
	if(status & ADC_INT7)
	{
		//MAP_ADC14_disableConversion();
		BITBAND_PERI(ADC14->CTL0, ADC14_CTL0_ENC_OFS) = 0;
		/* Forcing a software transfer on DMA Channel 0 */
		//MAP_DMA_requestSoftwareTransfer(0);
		BITBAND_PERI(DMA_Channel->SW_CHTRIG, DMA_SW_CHTRIG_CH0_OFS) = 1;
		/* Forcing a software transfer on DMA Channel 1 */
		//MAP_DMA_requestSoftwareTransfer(1);
		BITBAND_PERI(DMA_Channel->SW_CHTRIG, DMA_SW_CHTRIG_CH1_OFS) = 1;
		//MAP_ADC14_enableConversion();
		BITBAND_PERI(ADC14->CTL0, ADC14_CTL0_ENC_OFS) = 1;
	}
}


/* Completion interrupt for ADC14 MEM6 */
__attribute__((ramfunc))
void DMA_INT1_IRQHandler(void)
{
    refSampleCount++;
    /* Switch between primary and alternate bufferes with DMA's PingPong mode */
    if (MAP_DMA_getChannelAttribute(0) & UDMA_ATTR_ALTSELECT)
    {
        if (refSampleCount < SAMPLE_LENGTH / 1024)
            MAP_DMA_setChannelTransfer(UDMA_PRI_SELECT | DMA_CH0_RESERVED0,
                UDMA_MODE_PINGPONG, (void*) &ADC14->MEM[6],
                (void*)&ref[1024 * refSampleCount], 1024);
        MSP_EXP432P401RLP_DMAControlTable[0].control =
                (MSP_EXP432P401RLP_DMAControlTable[0].control & 0xff000000 ) |
                (((1024)-1)<<4) | UDMA_MODE_PINGPONG;
    }
    else
    {
        if (refSampleCount < SAMPLE_LENGTH / 1024)
            MAP_DMA_setChannelTransfer(UDMA_ALT_SELECT | DMA_CH0_RESERVED0,
                UDMA_MODE_PINGPONG, (void*) &ADC14->MEM[6],
                (void*)&ref[1024 * refSampleCount], 1024);
        MSP_EXP432P401RLP_DMAControlTable[8].control =
                (MSP_EXP432P401RLP_DMAControlTable[8].control & 0xff000000 ) |
                (((1024)-1)<<4) | UDMA_MODE_PINGPONG;
    }
}

/* Completion interrupt for ADC14 MEM7 */
__attribute__((ramfunc))
void DMA_INT2_IRQHandler(void)
{
    measSampleCount++;
    /* Switch between primary and alternate bufferes with DMA's PingPong mode */
    if (MAP_DMA_getChannelAttribute(1) & UDMA_ATTR_ALTSELECT)
    {
        if (measSampleCount < SAMPLE_LENGTH / 1024)
            MAP_DMA_setChannelTransfer(UDMA_PRI_SELECT | DMA_CH1_RESERVED0,
                UDMA_MODE_PINGPONG, (void*) &ADC14->MEM[7],
                (void*)&meas[1024 * measSampleCount], 1024);
        MSP_EXP432P401RLP_DMAControlTable[1].control =
                (MSP_EXP432P401RLP_DMAControlTable[1].control & 0xff000000 ) |
                (((1024)-1)<<4) | UDMA_MODE_PINGPONG;
    }
    else
    {
        if (measSampleCount < SAMPLE_LENGTH / 1024)
            MAP_DMA_setChannelTransfer(UDMA_ALT_SELECT | DMA_CH1_RESERVED0,
                UDMA_MODE_PINGPONG, (void*) &ADC14->MEM[7],
                (void*)&meas[1024 * measSampleCount], 1024);
        MSP_EXP432P401RLP_DMAControlTable[9].control =
                (MSP_EXP432P401RLP_DMAControlTable[9].control & 0xff000000 ) |
                (((1024)-1)<<4) | UDMA_MODE_PINGPONG;
    }
    if (measSampleCount == SAMPLE_LENGTH / 1024 + 1)
    {
        MAP_Timer_A_stopTimer(TIMER_A0_BASE);
        doneADC = true;
        return;
    }
}


