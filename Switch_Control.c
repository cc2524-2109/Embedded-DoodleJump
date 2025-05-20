#include <MKL46Z4.h>
#include "UART_Control.h"

#include "lcd.h"


/* ----------------------------------------------------------------------
 Note the different characters around the library names.
 The <> characters are used to include system libraries
 The "" characters are used to include your own libraries
 ------------------------------------------------------------------------*/


const int RED_LED_PIN = 29;
const int GREEN_LED_PIN = 5;
const int SWITCH_1_PIN = 3;
const int SWITCH_2_PIN = 12;

SIM_Type* global_SIM = SIM;
PORT_Type* global_PORTE = PORTE;
GPIO_Type* global_PTE = PTE;
PORT_Type* global_PORTC = PORTC;
GPIO_Type* global_PTC = PTC;
PORT_Type* global_PORTD = PORTD;
GPIO_Type* global_PTD = PTD;

volatile int count_r = 0;
volatile int count_l = 0;


// declare functions used to run the infinite loop for the switch
void operate_switch_polling();
void operate_switch_interrupts();

void displayLetter(int disp_num, char letter) {
    clearDigit(disp_num);  // Clear the digit before displaying a new one
    switch(letter) {
        case 'D':
            turnOnSegment(disp_num, 'A');
            turnOnSegment(disp_num, 'B');
            turnOnSegment(disp_num, 'C');
            turnOnSegment(disp_num, 'D');
            turnOnSegment(disp_num, 'E');
            turnOnSegment(disp_num, 'G');
            break;
        case 'O':
            turnOnSegment(disp_num, 'A');
            turnOnSegment(disp_num, 'B');
            turnOnSegment(disp_num, 'C');
            turnOnSegment(disp_num, 'D');
            turnOnSegment(disp_num, 'E');
            turnOnSegment(disp_num, 'F');
            break;
        case 'L':
            turnOnSegment(disp_num, 'D');
            turnOnSegment(disp_num, 'E');
            turnOnSegment(disp_num, 'F');
            break;
        case 'E':
            turnOnSegment(disp_num, 'A');
            turnOnSegment(disp_num, 'D');
            turnOnSegment(disp_num, 'E');
            turnOnSegment(disp_num, 'F');
            turnOnSegment(disp_num, 'G');
            break;
        case 'J':
            turnOnSegment(disp_num, 'B');
            turnOnSegment(disp_num, 'C');
            turnOnSegment(disp_num, 'D');
            turnOnSegment(disp_num, 'E');
            break;
        case 'U':
            turnOnSegment(disp_num, 'B');
            turnOnSegment(disp_num, 'C');
            turnOnSegment(disp_num, 'D');
            turnOnSegment(disp_num, 'E');
            turnOnSegment(disp_num, 'F');
            break;
        case 'P':
            turnOnSegment(disp_num, 'A');
            turnOnSegment(disp_num, 'B');
            turnOnSegment(disp_num, 'E');
            turnOnSegment(disp_num, 'F');
            turnOnSegment(disp_num, 'G');
            break;
        default:
            break;
    }
}

void displayWordLEFT() {
    clearDisplay();
    // L
    turnOnSegment(1, 'D');
    turnOnSegment(1, 'E');
    turnOnSegment(1, 'F');

    // E
    turnOnSegment(2, 'A');
    turnOnSegment(2, 'D');
    turnOnSegment(2, 'E');
    turnOnSegment(2, 'F');
    turnOnSegment(2, 'G');

    // F
    turnOnSegment(3, 'A');
    turnOnSegment(3, 'E');
    turnOnSegment(3, 'F');
    turnOnSegment(3, 'G');

    // T
    turnOnSegment(4, 'A');
//    turnOnSegment(4, 'D');

    turnOnSegment(4, 'E');
    turnOnSegment(4, 'F');
    LCD_TimeDelay(0x1FFFFFU);
    clearDisplay();

}

void displayWordRGHT() {
	  clearDisplay();
    // R
    turnOnSegment(1, 'A');
    turnOnSegment(1, 'C');
    turnOnSegment(1, 'B');
    turnOnSegment(1, 'E');
    turnOnSegment(1, 'F');
    turnOnSegment(1, 'G');

    // G
    turnOnSegment(2, 'A');
    turnOnSegment(2, 'C');

    turnOnSegment(2, 'D');
    turnOnSegment(2, 'E');
    turnOnSegment(2, 'F');
    turnOnSegment(2, 'G');

    // H
    turnOnSegment(3, 'B');
    turnOnSegment(3, 'C');
    turnOnSegment(3, 'E');
    turnOnSegment(3, 'F');
    turnOnSegment(3, 'G');

    // T
    turnOnSegment(4, 'A');
    turnOnSegment(4, 'E');
    turnOnSegment(4, 'F');
    LCD_TimeDelay(0x1FFFFFU);
    clearDisplay();
}


void displayTwoPartLetter(int left_digit_num, int right_digit_num, char letter) {
    clearDigit(left_digit_num);
    clearDigit(right_digit_num);
    switch(letter) {
        case 'M':
            // Left part of M (mirrored right part)
            turnOnSegment(left_digit_num, 'A');
            turnOnSegment(left_digit_num, 'B');
            turnOnSegment(left_digit_num, 'C');
            turnOnSegment(left_digit_num, 'E');
            turnOnSegment(left_digit_num, 'F');

            // Right part of M (like an 'A' but without the middle line)
            turnOnSegment(right_digit_num, 'A');
            turnOnSegment(right_digit_num, 'B');
            turnOnSegment(right_digit_num, 'C');
            turnOnSegment(right_digit_num, 'E');
            turnOnSegment(right_digit_num, 'F');
            break;
        // Add other cases as needed
    }
}

void shiftDisplayDoodleJump() {
    char word[] = "DOODLE JUMP!";  // Text to scroll
    int wordLength = sizeof(word) - 1;  // Exclude null terminator
    int displayLength = 4;  // Number of visible characters on your display
    int start = 0;  // Initialize start index for scrolling

    while (start < wordLength) {
        clearDisplay();  // Clear the display before showing new content
        int disp_num = 1;  // Start with the first display position
        int i = start;  // Index for accessing characters in the word

        while (disp_num <= displayLength && i < wordLength) {
            if (word[i] == 'M') {
                if (disp_num <= displayLength - 1) {  // Ensure 'M' can fit fully
                    displayTwoPartLetter(disp_num, disp_num + 1, 'M');
                    disp_num += 2;  // 'M' occupies two slots
                    i++;
                } else {
                    // If 'M' can't fit, stop adding more characters
                    break;
                }
            } else {
                // Display a single character
                displayLetter(disp_num, word[i]);
                disp_num++;
                i++;
            }
        }

        LCD_TimeDelay(0x3FFFFFU);  // Adjust delay to control the scroll speed
        start++;  // Increment start to scroll to the next position
    }
}


/*
 Main program: entry point
 */
int main(void) {
	// setup variables so we can see them in debugger
	// if you get rid of this it seems the compiler just optimizes the variables away
	// this is for educational purposes
    init_lcd();
    shiftDisplayDoodleJump();

	global_SIM = global_SIM;
	global_PORTE = global_PORTE;
	global_PTE = global_PTE;
	global_PORTC = global_PORTC;
	global_PTC = global_PTC;

	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK | SIM_SCGC5_PORTD_MASK;

	// setup red led
	PORTE->PCR[RED_LED_PIN] = PORT_PCR_MUX(0b001); //Set up PTE29 as GPIO
	PTE->PDDR |= GPIO_PDDR_PDD(1 << RED_LED_PIN); // make it output
	PTE->PSOR |= GPIO_PSOR_PTSO(1 << RED_LED_PIN); // turn off LED

	// setup green led
	PORTD->PCR[GREEN_LED_PIN] = PORT_PCR_MUX(0b001); //Set up PTE5 as GPIO
	PTD->PDDR |= GPIO_PDDR_PDD(1 << GREEN_LED_PIN); // make it output
	PTD->PSOR |= GPIO_PSOR_PTSO(1 << GREEN_LED_PIN); // turn off LED

	// setup switch 1
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK; //Enable the clock to port C
	PORTC->PCR[SWITCH_1_PIN] &= ~PORT_PCR_MUX(0b111); // Clear PCR Mux bits for PTC3
	PORTC->PCR[SWITCH_1_PIN] |= PORT_PCR_MUX(0b001); // Set up PTC3 as GPIO
	PTC->PDDR &= ~GPIO_PDDR_PDD(1 << SWITCH_1_PIN); // make it input
	PORTC->PCR[SWITCH_1_PIN] |= PORT_PCR_PE(1); // Turn on the pull enable
	PORTC->PCR[SWITCH_1_PIN] |= PORT_PCR_PS(1); // Enable the pullup resistor
	PORTC->PCR[SWITCH_1_PIN] &= ~PORT_PCR_IRQC(0b1111); // Clear IRQC bits for PTC3
	PORTC->PCR[SWITCH_1_PIN] |= PORT_PCR_IRQC(0b1011); // Set up the IRQC to interrupt on either edge (i.e. from high to low or low to high)

	// setup switch 3
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK; //Enable the clock to port C
	PORTC->PCR[SWITCH_2_PIN] &= ~PORT_PCR_MUX(0b111); // Clear PCR Mux bits for PTC3
	PORTC->PCR[SWITCH_2_PIN] |= PORT_PCR_MUX(0b001); // Set up PTC3 as GPIO
	PTC->PDDR &= ~GPIO_PDDR_PDD(1 << SWITCH_2_PIN); // make it input
	PORTC->PCR[SWITCH_2_PIN] |= PORT_PCR_PE(1); // Turn on the pull enable
	PORTC->PCR[SWITCH_2_PIN] |= PORT_PCR_PS(1); // Enable the pullup resistor
	PORTC->PCR[SWITCH_2_PIN] &= ~PORT_PCR_IRQC(0b1111); // Clear IRQC bits for PTC3
	PORTC->PCR[SWITCH_2_PIN] |= PORT_PCR_IRQC(0b1011); // Set up the IRQC to interrupt on either edge (i.e. from high to low or low to high)

	init_uart();
	//operate_switch_polling();
	operate_switch_interrupts();

	return 0;
}

void operate_switch_polling() {
	while(1) {
		if((PORTC->PCR[SWITCH_1_PIN] & PORT_PCR_ISF(1)) != 0) {
			PTE->PTOR = GPIO_PTOR_PTTO(1 << RED_LED_PIN); // button pressed, toggle LED
			PORTC->PCR[SWITCH_1_PIN] |= PORT_PCR_ISF(1);  // clear the interrupt status flag by writing 1 to it
		}
		if((PORTC->PCR[SWITCH_2_PIN] & PORT_PCR_ISF(1)) != 0) {
			PTD->PTOR = GPIO_PTOR_PTTO(1 << GREEN_LED_PIN); // button pressed, toggle LED
			PORTC->PCR[SWITCH_2_PIN] |= PORT_PCR_ISF(1);  // clear the interrupt status flag by writing 1 to it
		}
	}
}

void operate_switch_interrupts() {
	NVIC_EnableIRQ(PORTC_PORTD_IRQn); // configure NVIC so that interrupt is enabled
	while(1){}
}


void PORTC_PORTD_IRQHandler(void) {
	if((PORTC->PCR[SWITCH_1_PIN] & PORT_PCR_ISF(1)) != 0) {
		count_r = count_r + 1;
		PTE->PTOR = GPIO_PTOR_PTTO(1 << RED_LED_PIN); // button pressed, toggle LED
		PORTC->PCR[SWITCH_1_PIN] |= PORT_PCR_ISF(1);  // clear the interrupt status flag by writing 1 to it
		if (count_r == 2){
			count_r = 0;
			uart_puts("r");
		}
		displayWordRGHT();

	}
	if((PORTC->PCR[SWITCH_2_PIN] & PORT_PCR_ISF(1)) != 0) {
		count_l = count_l + 1;
		PTD->PTOR = GPIO_PTOR_PTTO(1 << GREEN_LED_PIN); // button pressed, toggle LED
		PORTC->PCR[SWITCH_2_PIN] |= PORT_PCR_ISF(1);  // clear the interrupt status flag by writing 1 to it
		if (count_l == 2){
			count_l = 0;
			uart_puts("l");
		}
	    displayWordLEFT();

	}
}
