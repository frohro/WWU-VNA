/*
 * serial_comm.h
 *
 *  Created on: May 2, 2017
 *      Author: frohro
 */

#ifndef SERIAL_COMM_H_
#define SERIAL_COMM_H_
#include <ti/devices/msp432p4xx/inc/msp432.h>
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/sysbios/family/arm/m3/Hwi.h>
#include <stdbool.h>

#define STATUS_FAIL false

/* UART Configuration Parameter. These are the configuration parameters to
 * make the eUSCI A UART module to operate with a 115200 baud rate. These
 * values were calculated using the online calculator that TI provides
 * at:
 * http://processors.wiki.ti.com/index.php/
 *               USCI_UART_Baud_Rate_Gen_Mode_Selection
 *
 * On Linux use /dev/ttyACM0, 115200, 8N1.
 *               CuteCom works well.
 */
const eUSCI_UART_Config uartConfig =
{
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        78,                                      // BRDIV = 26 for 115200 baud
        0,                                       // UCxBRF = 0 for 115200 baud
        0x10,                                       // UCxBRS = 0 for 115200 baud
        EUSCI_A_UART_NO_PARITY,                  // No Parity
        EUSCI_A_UART_LSB_FIRST,                  // MSB First
        EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
        EUSCI_A_UART_MODE,                       // UART mode
//      EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION // Oversampling Mode
        EUSCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION  // Low Frequency Mode
};
char uartRXData[80];
static bool uartEndOfLineFlag = false;

void initializeClocks(void);
int initializeBackChannelUART(void);

#endif /* SERIAL_COMM_H_ */
