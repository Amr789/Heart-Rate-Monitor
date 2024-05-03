#include <msp430.h>


    unsigned int count = 0;
    unsigned int bpm = 0;  // Initializing count and bpm

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P1DIR |= BIT0; // Setting P1.0 to output
    P1OUT &= ~BIT0; // Turning it off
    P4DIR |= BIT7; // Setting P4.7 to output
    P4OUT &= ~BIT7; // Turning it off


    // Setting 7 segment screen to display 0 0 as soon as program starts
    P3DIR = 0xff; // Setting port 3 to output
    P6DIR = 0xff; // Setting port 6 to output
    P3OUT = 0x00; // Setting port 3 to low
    P6OUT = 0x00; // Setting port 6 to low
    P6OUT |= BIT0; // Setting P6.0 to high
    P3OUT |= BIT4; // Setting P3.4 to high

    // Setting timer to count 16 seconds
    TA0CTL |= TASSEL_1 + MC_2 + ID_3; // ACLK, Continuous Mode, Divide by 8
    TA0CCTL0 |= CCIE; // Enabling Interrupt

    // ADC
    P7SEL |= 0x01;

    ADC12CTL0 = ADC12SHT02 + ADC12ON;
    ADC12CTL1 = ADC12SHP;
    ADC12IE = 0x01;
    ADC12CTL0 |= ADC12ENC;


    P1DIR &= ~BIT6; // Setting P1.6 to input
    P1IES &= ~BIT6; // Selecting low-to-high transition triggering for interrupt configured on P1.6
    P1IE  |= BIT6; // Enabling interrupt
    P1IFG &= ~BIT6; // Clearing the interrupt flag for this pin

    __bis_SR_register(GIE); // Enabling interrupts
    // while loop to keep code running
    while(1){
       ADC12CTL0 |= ADC12SC;
       __bis_SR_register(LPM0_bits + GIE);
    }
    return 0;
}



// LCD 1
void display_number1(int num)
{
 switch(num)
 {
   case 0:
       P6OUT = 0x00;
       P6OUT |= BIT0;
       break;
   case 1:
       P6OUT = 0xff;
       P6OUT &= ~BIT3;
       P6OUT &= ~BIT6;
       break;
   case 2:
       P6OUT = 0x00;
       P6OUT |= BIT1;
       P6OUT |= BIT6;
       break;
   case 3:
       P6OUT = 0x00;
       P6OUT |= BIT1;
       P6OUT |= BIT4;
       break;
   case 4:
       P6OUT = 0x00;
       P6OUT |= BIT2;
       P6OUT |= BIT5;
       P6OUT |= BIT4;
       break;
   case 5:
       P6OUT = 0x00;
       P6OUT |= BIT3;
       P6OUT |= BIT4;
       break;
   case 6:
       P6OUT = 0x00;
       P6OUT |= BIT3;
       break;
   case 7:
       P6OUT = 0xff;
       P6OUT &= ~BIT2;
       P6OUT &= ~BIT3;
       P6OUT &= ~BIT6;
       break;
   case 8:
       P6OUT = 0x00;
       break;
   case 9:
       P6OUT = 0x00;
       P6OUT |= BIT4;
       P6OUT |= BIT5;
       break;
   case 10:
       P6OUT = 0x00;
       P6OUT |= BIT5;
       break;
   case 11:
       P6OUT = 0x00;
       P6OUT |= BIT2;
       P6OUT |= BIT3;
       break;
   case 12:
       P6OUT = 0x00;
       P6OUT |= BIT0;
       P6OUT |= BIT3;
       P6OUT |= BIT6;
       break;
   case 13:
      P6OUT = 0x00;
      P6OUT |= BIT2;
      P6OUT |= BIT1;
      break;
   case 14:
      P6OUT = 0x00;
      P6OUT |= BIT3;
      P6OUT |= BIT6;
      break;
   case 15:
      P6OUT = 0x00;
      P6OUT |= BIT3;
      P6OUT |= BIT6;
      P6OUT |= BIT5;
      break;
   default:
       P6OUT = 0x00;
       P6OUT |= BIT0;
       break;
 }
}

// LCD 2
void display_number2(int num)
{
 switch(num)
 {
   case 0:
       P3OUT = 0x00;
       P3OUT |= BIT4;
       break;
   case 1:
       P3OUT = 0xff;
       P3OUT &= ~BIT6;
       P3OUT &= ~BIT1;
       break;
   case 2:
       P3OUT = 0x00;
       P3OUT |= BIT3;
       P3OUT |= BIT1;
       break;
   case 3:
       P3OUT = 0x00;
       P3OUT |= BIT3;
       P3OUT |= BIT5;
       break;
   case 4:
       P3OUT = 0x00;
       P3OUT |= BIT2;
       P3OUT |= BIT5;
       P3OUT |= BIT7;
       break;
   case 5:
       P3OUT = 0x00;
       P3OUT |= BIT6;
       P3OUT |= BIT5;
       break;
   case 6:
       P3OUT = 0x00;
       P3OUT |= BIT6;
       break;
   case 7:
       P3OUT = 0xff;
       P3OUT &= ~BIT2;
       P3OUT &= ~BIT6;
       P3OUT &= ~BIT1;
       break;
   case 8:
       P3OUT = 0x00;
       break;
   case 9:
       P3OUT = 0x00;
       P3OUT |= BIT5;
       P3OUT |= BIT7;
       break;
   default:
       P3OUT = 0x00;
       P3OUT |= BIT4;
       break;
 }
}


    #pragma vector = PORT1_VECTOR
    __interrupt void Port_1(void)
    {
        count += 1;   // Incrementing count on each rising edge
        P1IFG &= (~BIT6); // Clearing flag
        P1OUT ^= BIT0; // Toggling LED on each rising edge
    }

    int timer = 0;

    #pragma vector = TIMER0_A0_VECTOR
    __interrupt void TIMER0A_ISR(void) {
        P4OUT ^= BIT7;           // Toggling LED every 16 seconds
        if (timer != 4){
            timer += 1;
        }
        else {
            bpm = 3.75 * count; // Calculating heart rate
            int str1 = bpm/10;
            int str2 = bpm%10;
            display_number1(str1);
            display_number2(str2);
            bpm = 0; // Resetting variables
            count = 0;
        }
        }

    }


    #pragma vector = ADC12_VECTOR
    __interrupt void ADC12_ISR(void) {
        if (ADC12MEM0 >= 0xb96)
            P1OUT ^= BIT0;
        else
            P1OUT &= ~BIT0;
        __bic_SR_register_on_exit(LPM0_bits);
    }
