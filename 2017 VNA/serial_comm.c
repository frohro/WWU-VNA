/*
 * serial_comm.c
 *
 *  Created on: May 2, 2017
 *      Author: frohro
 */


/*
 * USCIA0 interrupt handler for backchannel UART.
 * For interrupts, don't forget to edit the startup...c file
 * or if you are using Energia or RTOS use th Hmi functions
 * to register the interrupt here!
 */
#include "serial_comm.h"

void EusciA0_ISR(void) // Might need to be called EUSCIA0_IRQHandler().
{
    static int i=0;
    uint32_t status = UART_getEnabledInterruptStatus(EUSCI_A0_BASE);
    uint_fast8_t errorStatus = UART_queryStatusFlags(EUSCI_A0_BASE,
            EUSCI_A_UART_FRAMING_ERROR|EUSCI_A_UART_RECEIVE_ERROR
            |EUSCI_A_UART_PARITY_ERROR|EUSCI_A_UART_OVERRUN_ERROR
            |EUSCI_A_UART_BUSY);

    UART_clearInterruptFlag(EUSCI_A0_BASE, status);
//    if((errorStatus & (EUSCI_A_UART_FRAMING_ERROR|EUSCI_A_UART_RECEIVE_ERROR
//          |EUSCI_A_UART_PARITY_ERROR|EUSCI_A_UART_OVERRUN_ERROR
//          |EUSCI_A_UART_BUSY)) != 0)
//  {
//      printf("errorStatus is: %x\n");
//  }
    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT)
    {
        uartRXData[i] = UART_receiveData(EUSCI_A0_BASE);
        if(uartRXData[i++]==0x0d)
        {
            uartEndOfLineFlag = true;
            uartRXData[i] = 0;  // To end the array.
            i=0; // Get ready for the next command.
        }
    }
}

void initializeClocks(void)
{
    /* Initialize main clock to 48MHz.  To make it 3 MHz change the 48 to 3
     * and the 16's to 1.*/
    PCM_setCoreVoltageLevel(PCM_VCORE1);
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48); // Full speed
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_16 );
    MAP_CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_16 );
    MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_16 );
}

/*  We may want to use P3.2 and P3.3 as a Bluetooth UART because the
 *  backchannel UART is a bit slow.  I only get every other character
 *  at 115200 baud.
 */
int initializeBackChannelUART(void){
    Hwi_Params params;
    initializeClocks();
    /* Selecting P1.2 and P1.3 in UART mode. */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
        GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Configuring UART Module */
    if(UART_initModule(EUSCI_A0_BASE, &uartConfig)==STATUS_FAIL)
        return (STATUS_FAIL);

    UART_selectDeglitchTime(EUSCI_A0_BASE,EUSCI_A_UART_DEGLITCH_TIME_200ns);

    /* Enable UART module */
    MAP_UART_enableModule(EUSCI_A0_BASE);

    /* Enable UART interrupts for backchannel UART
     * We may or may not need to do this.  The simple
     * printf() routine doesn't seem to use it.  */
    // Register interrupt
    Hwi_Params_init(&params);
    Hwi_create(INT_EUSCIA0, EusciA0_ISR, &params, NULL);
    UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
            /*EUSCI_A_SPI_TRANSMIT_INTERRUPT);*/
    Interrupt_enableInterrupt(INT_EUSCIA0);
    return 1;
}
