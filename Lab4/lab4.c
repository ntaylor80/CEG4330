/* --COPYRIGHT--,BSD
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
 * MSP432 GPIO - Input Interrupt
 *
 * Description: This example demonstrates a very simple use case of the
 * DriverLib GPIO APIs. P1.1 (which has a switch connected to it) is configured
 * as an input with interrupts enabled and P1.0 (which has an LED connected)
 * is configured as an output. When the switch is pressed, the LED output
 * is toggled.
 *
 *                MSP432P401
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST         P1.0  |---> P1.0 LED
 *            |                  |
 *            |            P1.1  |<--Toggle Switch
 *            |                  |
 *            |                  |
 *
 ******************************************************************************/
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>


int c_size = 3;
int r_size = 4;
unsigned int timer_count = 0;
int blink = 0;
uint_fast16_t c[]={GPIO_PIN5,GPIO_PIN6,GPIO_PIN7};//all on port 1
uint_fast16_t r[]={GPIO_PIN4,GPIO_PIN5,GPIO_PIN6,GPIO_PIN7};//all on port 2
int main(void){

    volatile uint32_t ii;

    /* Halting the Watchdog */
    MAP_WDT_A_holdTimer();

    /* Configuring P1.0 as output and P1.1 (switch) as input */
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1,GPIO_PIN0);


    /* Configuring P1.1 as an input and enabling interrupts */
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);

    int i = 0;
    for (i = 0; i < c_size;i++){
        MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, c[i]);
        MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, c[i]);
        MAP_GPIO_enableInterrupt(GPIO_PORT_P1, c[i]);
    }

    for (i = 0; i < r_size; i++){
        MAP_GPIO_setAsOutputPin(GPIO_PORT_P2,r[i]);
        MAP_GPIO_setOutputLowOnPin (GPIO_PORT_P2,r[i]);
    }


    MAP_Interrupt_enableInterrupt(INT_PORT1);

    /* Enabling SRAM Bank Retention */
    MAP_SysCtl_enableSRAMBankRetention(SYSCTL_SRAM_BANK1);

    /* Enabling MASTER interrupts */
    MAP_Interrupt_enableMaster();

    /* Going to LPM3 */
    while (1)
    {
        MAP_PCM_gotoLPM3();
    }
}



void start_timer(unsigned int time){
    timer_count = time * 2;
    uint_fast16_t TIMER_PERIOD = 46875;
    const Timer_A_UpModeConfig upConfig =
       {
       TIMER_A_CLOCKSOURCE_SMCLK, // SMCLK Clock Source
       TIMER_A_CLOCKSOURCE_DIVIDER_64, // SMCLK/1 = 3MHz
       TIMER_PERIOD, // 11,718 tick period
       TIMER_A_TAIE_INTERRUPT_DISABLE, // Disable Timer interrupt
       TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE , // Enable CCR0 interrupt
       TIMER_A_DO_CLEAR // Clear value
      };

    MAP_Timer_A_configureUpMode(TIMER_A1_BASE, &upConfig);
    MAP_Interrupt_enableInterrupt(INT_TA1_0);
    MAP_Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);
}

void TA1_0_IRQHandler(void)
 {
    timer_count--;
    MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE,
    TIMER_A_CAPTURECOMPARE_REGISTER_0);

    if (timer_count <= 0){
        MAP_Timer_A_stopTimer(TIMER_A0_BASE);
        MAP_Interrupt_disableInterrupt(INT_TA1_0);
        blink = 0;
    }
 }

/* GPIO ISR */
void PORT1_IRQHandler(void)
{
    uint32_t status;

    status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);//tells us the row triggered
    //now get the column
    int multiplyer=status;
    int base=1;
    int i=0;
    for(i=0;i<r_size;i++){
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2,r[i]);
        if(!(MAP_GPIO_getInputPinValue(GPIO_PORT_P1,status))){
            base=i;
            i=r_size;//might be safer than break, idk
        }
    }
    //gert column
    for(i=0;i<c_size;i++){
        if(c[i]==status){
         base = i;
         i=c_size;
        }
    }

    //multiplyer is the row, base is the column, rXc or {1,2,3,4} x {1,2,3} = {1,2,3}{4,5,6}{7,8,9} to find the key pressed
    //this makes the order of the pins in r[] and c[] determine which keys work
    int actual_number=multiplyer*base;



    start_timer(actual_number);//assuming timer waits 1/2 hz





    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, status);

//
//    //not sure what this does
//    if(status & c[0]){
//        if(~blink){
//        start_timer(3);
//        blink = 1;
//        }
//        }
//    /* Toggling the output on the LED */
//
//     MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
//

}
