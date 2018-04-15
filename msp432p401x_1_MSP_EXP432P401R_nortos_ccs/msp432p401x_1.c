/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2013, Texas Instruments Incorporated
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
 *
 *******************************************************************************
 *
 *                       MSP432 CODE EXAMPLE DISCLAIMER
 *
 * MSP432 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see http://www.ti.com/tool/mspdriverlib for an API functional
 * library & https://dev.ti.com/pinmux/ for a GUI approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//******************************************************************************
//   MSP432P401 Demo - Software Toggle P1.0
//
//   Description: Toggle P1.0 by xor'ing P1.0 inside of a software loop.
//   ACLK = 32.768kHz, MCLK = SMCLK = default DCO~1MHz
//
//                MSP432P401x
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST              |
//            |                 |
//            |             P1.0|-->LED
//
//   William Goh
//   Texas Instruments Inc.
//   June 2016 (updated) | November 2013 (created)
//   Built with CCSv6.1, IAR, Keil, GCC
//******************************************************************************
#include "ti/devices/msp432p4xx/inc/msp.h"
#include <stdint.h>

//int main(void) {
////    volatile uint32_t i;
////
////    WDT_A->CTL = WDT_A_CTL_PW |             // Stop WDT
////                 WDT_A_CTL_HOLD;
////
////    P1->DIR |= BIT0;                        // P1.0 set as output
////
////    while (1)                               // continuous loop
////    {
////        P1->OUT ^= BIT0;                    // Blink P1.0 LED
////        for (i = 50000; i > 0; i--);        // Delay (originally, 20,000)
////    }
//    WSTCTL = WDTPW +WDTHOLD; //watchdog
//    P1DIR|= 0x01;
//    P1IE|= 0x10;
//    P1IE|=0x10;
//    P1IES|=0x10;
//    P1IFG &= ~0x10;
//
//
//    _BIS_SR(LPM4_bits + GIE);
//}
//
//_interrupt void Port_1(void)
//{
//  P1OUT ^=0x01;
//  P1IFG &= ~0x10;
//
//
//}
 #include "ti/devices/msp432p4xx/inc/msp.h"
 int main(void)
 {
 // Hold the watchdog

 WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

 // Configuring P1.0 as output and P1.1 (switch) as input with pull-up
 // resistor. Rest of pins are configured as output low.
 // Notice intentional '=' assignment since all P1 pins are being
 // deliberately configured



 P1->DIR = ~(uint8_t) BIT1;
 P1->OUT = BIT1;
 P1->REN = BIT1; // Enable pull-up resistor (P1.1 output high)

 P1->SEL0 = 0;
 P1->SEL1 = 0;
 P1->IES = BIT1;

 P1->IFG = 0; // Clear all P1 interrupt flags
 P1->IE = BIT1; // Enable interrupt for P1.1

 P6->DIR |= BIT5;
 P6->DIR |= BIT4;
 P4->DIR |= BIT6;

 P6->OUT |= BIT5;
 P6->OUT |= BIT4;
 P4->OUT |= BIT6;
 P1->OUT = BIT6;
 P1->REN = BIT6;
 P1->OUT = BIT7;
 P1->REN = BIT7;

//  P1->IES = BIT6;// Interrupt on high-to-low transition
// P1->IES = BIT7;
// P1->IE = BIT6;
// P1->IE = BIT7;

 // Enable Port 1 interrupt on the NVIC
 NVIC->ISER[1] = 1 << ((PORT1_IRQn) & 31);

 // Terminate all remaining pins on the device
 P2->DIR |= 0xFF; P2->OUT = 0;
 P3->DIR |= 0xFF; P3->OUT = 0;
 //P4->DIR |= 0xFF; P4->OUT = 0;
 P5->DIR |= 0xFF; P5->OUT = 0;
 //P6->DIR |= 0xFF; P6->OUT = 0;
 P7->DIR |= 0xFF; P7->OUT = 0;
 P8->DIR |= 0xFF; P8->OUT = 0;
 P9->DIR |= 0xFF; P9->OUT = 0;
 P10->DIR |= 0xFF; P10->OUT = 0;

 // Configure Port J
 PJ->DIR |= (BIT0| BIT1 | BIT2 | BIT3);
 PJ->OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3);

 // Enable PCM rude mode, which allows to device to enter LPM3 without waiting for
//peripherals
 PCM->CTL1 = PCM_CTL0_KEY_VAL | PCM_CTL1_FORCE_LPM_ENTRY;

 // Enable global interrupt
 __enable_irq();
//Page 2
//msp432p401x_p1_03.c

 // Setting the sleep deep bit
 SCB->SCR |= (SCB_SCR_SLEEPDEEP_Msk);

 // Do not wake up on exit from ISR
 SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;

 // Ensures SLEEPONEXIT takes effect immediately
 __DSB();

 // Go to LPM4
 __sleep();
 }

 /* Port1 ISR */
 void PORT1_IRQHandler(void)
 {
 volatile uint32_t i;

 // Toggling the output on the LED
 if(P1->IFG & (BIT1)//|BIT6|BIT7))
 P1->OUT ^= BIT0;

 // Delay for switch debounce
 for(i = 0; i < 10000; i++)

 P1->IFG =0;
 }



