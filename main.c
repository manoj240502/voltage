#include <xc.h>  // Include header for the specific microcontroller

#define _XTAL_FREQ 20000000  // Define oscillator frequency for delay functions

// Function prototypes
void ADC_Init();
unsigned int ADC_Read(unsigned char channel);

void ADC_Init(){
    ADCON0 = 0x41;  // Turn ON ADC, select channel 0, and set Fosc/8 as the conversion clock
    ADCON1 = 0x80;  // Configure ADC voltage reference and result format (right justified)
}

unsigned int ADC_Read(unsigned char channel){
    if(channel > 7) return 0;  // ADC has 8 channels, 0-7
    ADCON0 &= 0xC5;  // Clear existing channel selection bits
    ADCON0 |= (channel << 3);  // Set new channel
    __delay_ms(2);  // Acquisition time to charge the hold capacitor
    GO_nDONE = 1;  // Start ADC conversion
    while(GO_nDONE);  // Wait for conversion to complete
    return ((ADRESH << 8) + ADRESL);  // Return result
}

void main(){
    unsigned int adc_value;
    float voltage, input_voltage;

    TRISA = 0xFF;  // Configure PORTA as input
    ADC_Init();  // Initialize ADC

    while(1){
        adc_value = ADC_Read(0);  // Read ADC value from channel 0
        voltage = (adc_value * 5.0) / 1023.0;  // Convert ADC value to voltage
        input_voltage = voltage * ((100.0 + 3.3) / 3.3);  // Convert to input voltage

        // Use input_voltage as needed (for example, output to a display or communicate via UART)
        __delay_ms(500);  // Delay for a while
    }
}