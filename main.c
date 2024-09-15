/*
 * Authors:
210020009 - Ganesh Panduranga Karamsetty
210020036 - Rishabh Pomaje
Program that toggles the red LED when the on-board switch is pressed via the GPIO interrupt.
*/
#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"

void PORT_F_init( void );
void GPIOF_SETUP( void );
void GPIOF_ISR( void );

int main ( void )
{
    PORT_F_init();
    GPIOF_SETUP();
    while(1){
        ; // The GPIO PORT F interrupts manage everything here.
        // Test change by Ganesh
    }
}

void PORT_F_init( void )
{
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF ;      // Enable clock to GPIO_F
    GPIO_PORTF_LOCK_R = 0x4C4F434B ;            // Unlock commit register
    GPIO_PORTF_CR_R = 0x1F ;                    // Make PORTF0 configurable
    GPIO_PORTF_DEN_R = 0x1F ;                   // Set PORTF pins 4 pin
    GPIO_PORTF_DIR_R = 0x0E ;                   // Set PORTF4 pin as input user switch pin
    GPIO_PORTF_PUR_R = 0x11 ;                   // Set the switches for Pull Up
    GPIO_PORTF_DATA_R = 0x00 ;                  // Initialize the LEDs to off state
}

void GPIOF_SETUP( void )
{
    // PORT F = ...|SW1|G|B|R|SW2|
    // Interrupt sense register :: Used to configure whether interrupt is level/ edge sensitive.
    // 1 => Level detection and 0 => Edge detection
    GPIO_PORTF_IS_R = 0x00 ;
    // Interrupt Event Register :: Used to configure whether event to be detected is high/low or rising/falling.
    // 1 => Rising Edge/ High and 0 => Falling Edge/ Low
    GPIO_PORTF_IEV_R = 0x00 ;
    // Interrupt Both Edges Register :: Used to configure whether both edges are to be detected as events for interrupt.
    GPIO_PORTF_IBE_R = 0x00 ;
    // Interrupt Mask Register :: To determine whether to allow the interrupt generated to be passed onto interrupt controller or not.
    // 1 == Send the interrupt
    // 0 == Do not send the interrupt
    GPIO_PORTF_IM_R = 0x11 ;
    // Interrupt Clear Register :: Used to clear any pending interrupts
    GPIO_PORTF_ICR_R = 0x11 ;
    NVIC_EN0_R |=  (1 << 30) ; // Enable interrupt for GPIO Port F
}

void GPIOF_ISR( void )
{
    // PORT F = ...|SW1|G|B|R|SW2|
    GPIO_PORTF_DATA_R ^= 0x02 ;
    GPIO_PORTF_ICR_R = 0x11 ;
    int counter = 10000 ;
    while(counter > 0){
        counter-- ;
        // Allows the Interrupt Register Time to clear.
    }
}
