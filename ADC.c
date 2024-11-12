/*#include <platform.h>
#include <adc.h>
#include <gpio.h>

#define R1 (1e6)
#define R2 (1e6)
#define SCALE_FACTOR ((R1 + R2) / R2)
#define VREF (3.3)  // Reference voltage
#define T_LED_ON (1000)   // LED on time in microseconds
#define T_LED_OFF (1000)  // LED off time in microseconds
#define NUM_MEASUREMENTS (20)  // Number of measurements to average

//#define IR_LED_PIN (PA_8)
#define IR_LED_PIN (PC_7)
#define RGB_RED_PIN (PA_5)
#define RGB_GREEN_PIN (PA_6)
#define RGB_BLUE_PIN (PA_7)

float threshold[] = {0.30, 0.40, 0.50, 0.60, 0.7};
//0.015  0.3 
// Function to create a delay in microseconds
void delay_us(int us) {
    int count = us * (SystemCoreClock / 1000000) / 5;  // Calculate loop count for delay
    while (count--);  // Busy-wait loop for delay
}

// Function to control the RGB LEDs based on the average IR difference
void pick_leds(float avg_diff) {
    gpio_set(RGB_RED_PIN, 0);  // Turn off the Red LED
    gpio_set(RGB_GREEN_PIN, 0);  // Turn off the Green LED
    gpio_set(RGB_BLUE_PIN, 0);  // Turn off the Blue LED

    if (avg_diff < threshold[0]) {
        gpio_set(RGB_RED_PIN, 1);  // Turn on Red LED
        gpio_set(RGB_GREEN_PIN, 1);  // Turn on Green LED
        gpio_set(RGB_BLUE_PIN, 1);  // Turn on Blue LED (White light)
    } else if (avg_diff < threshold[1]) {
        gpio_set(RGB_RED_PIN, 1);  // Turn on Red LED
        gpio_set(RGB_GREEN_PIN, 0);  // Turn off Green LED
        gpio_set(RGB_BLUE_PIN, 1);  // Turn on Blue LED (Magenta light)
    } else if (avg_diff < threshold[2]) {
        gpio_set(RGB_RED_PIN, 1);  // Turn on Red LED
        gpio_set(RGB_GREEN_PIN, 0);  // Turn off Green LED
        gpio_set(RGB_BLUE_PIN, 0);  // Turn off Blue LED (Red light)
    } else if (avg_diff < threshold[3]) {
        gpio_set(RGB_RED_PIN, 1);  // Turn on Red LED
        gpio_set(RGB_GREEN_PIN, 1);  // Turn on Green LED
        gpio_set(RGB_BLUE_PIN, 0);  // Turn off Blue LED (Yellow light)
    } else if (avg_diff < threshold[4]) {
        gpio_set(RGB_RED_PIN, 0);  // Turn off Red LED
        gpio_set(RGB_GREEN_PIN, 0);  // Turn off Green LED
        gpio_set(RGB_BLUE_PIN, 1);  // Turn on Blue LED (Blue light)
    } else {
        gpio_set(RGB_RED_PIN, 0);  // Turn off Red LED
        gpio_set(RGB_GREEN_PIN, 1);  // Turn on Green LED
        gpio_set(RGB_BLUE_PIN, 0);  // Turn off Blue LED (Green light)
    }
}

// Function to measure the difference in IR light when the IR LED is on and off
float measure_ir(void) {
    float ir_off = 0, ir_on = 0;  // Variables to store average ADC values
    for (int i = 0; i < NUM_MEASUREMENTS; i++) {
        // Measure with LED off
        gpio_set(IR_LED_PIN, 0);  // Turn off IR LED
        delay_us(T_LED_OFF);  // Wait for the specified off time
        int adc_value_off = adc_read(P_ADC);  // Read ADC value
        ir_off += (float)adc_value_off * SCALE_FACTOR * VREF / ADC_MASK;  // Scale and accumulate

        // Measure with LED on
        gpio_set(IR_LED_PIN, 1);  // Turn on IR LED
        delay_us(T_LED_ON);  // Wait for the specified on time
        int adc_value_on = adc_read(P_ADC);  // Read ADC value
        ir_on += (float)adc_value_on * SCALE_FACTOR * VREF / ADC_MASK;  // Scale and accumulate
    }
    ir_off /= NUM_MEASUREMENTS;  // Calculate the average for LED off
    ir_on /= NUM_MEASUREMENTS;  // Calculate the average for LED on

    return ir_on - ir_off;  // Return the difference
}

int main(void) {
    adc_init(P_ADC);  // Initialize the ADC
    gpio_set_mode(IR_LED_PIN, Output);  // Set IR LED pin as output
    gpio_set_mode(RGB_RED_PIN, Output);  // Set RGB Red pin as output
    gpio_set_mode(RGB_GREEN_PIN, Output);  // Set RGB Green pin as output
    gpio_set_mode(RGB_BLUE_PIN, Output);  // Set RGB Blue pin as output

    float avg_diff = 0;
    
    while (1) {
        avg_diff = measure_ir();  // Measure the IR difference
        pick_leds(avg_diff);  // Update the RGB LEDs based on the difference
         
			  avg_diff =0;
    }
}
*/
#include "stm32f401xe.h"
#include "platform.h"
#include "adc.h"
#include "gpio.h"

#define R1 1e6
#define R2 1e6
#define SCALE_FACTOR ((R1 + R2) / R2)
#define VREF 3.3
#define NUM_MEASUREMENTS 20

#define P_IR_LED PC_7
#define PIN_ADC_CHANNEL P_ADC

#define PIN_RED   PA_5
#define PIN_GREEN PA_6
#define PIN_BLUE  PA_7

//const int threshold[] = {0, 4, 8, 12, 16, 20};
//const int threshold[] = {15, 12, 9, 6, 5, 4};
const int threshold[] = {24, 22, 20, 18, 16, 14};
void gpio_init(void);
void set_rgb_led(int distance);
void delay_ms(int ms);
void gpio_write(Pin pin, int value);

void initialize_system() {
    adc_init(PIN_ADC_CHANNEL);
    gpio_init();
}

void delay_ms(int ms) {
    for (int i = 0; i < ms * 1000; i++) {
        __NOP(); 
    }
}

int measure_ir() {
    int background_ir = 0, ir_on = 0;
    int diff;

    for (int i = 0; i < NUM_MEASUREMENTS; i++) {
        background_ir += adc_read(PIN_ADC_CHANNEL);
        delay_ms(1);
    }
    background_ir /= NUM_MEASUREMENTS;

    gpio_write(P_IR_LED, 1);

    for (int i = 0; i < NUM_MEASUREMENTS; i++) {
        ir_on += adc_read(PIN_ADC_CHANNEL);
        delay_ms(1);
    }
    ir_on /= NUM_MEASUREMENTS;

    gpio_write(P_IR_LED, 0);

    diff = ir_on - background_ir;
    return diff;
}

void set_rgb_led(int distance) {
    if (distance > threshold[1]) {
        gpio_write(PIN_RED, 1);
        gpio_write(PIN_GREEN, 1);
        gpio_write(PIN_BLUE, 1); // White
    } else if (distance > threshold[2]) {
        gpio_write(PIN_RED, 1);
        gpio_write(PIN_GREEN, 0);
        gpio_write(PIN_BLUE, 1); // Magenta
    } else if (distance > threshold[3]) {
        gpio_write(PIN_RED, 1);
        gpio_write(PIN_GREEN, 0);
        gpio_write(PIN_BLUE, 0); // Red
    } else if (distance > threshold[4]) {
        gpio_write(PIN_RED, 1);
        gpio_write(PIN_GREEN, 1);
        gpio_write(PIN_BLUE, 0); // Yellow
    } else if (distance > threshold[5]) {
        gpio_write(PIN_RED, 0);
        gpio_write(PIN_GREEN, 0);
        gpio_write(PIN_BLUE, 1); // Blue
    } else {
        gpio_write(PIN_RED, 0);
        gpio_write(PIN_GREEN, 1);
        gpio_write(PIN_BLUE, 0); // Green
    }
}

int main(void) {
    initialize_system();

    while (1) {
        int ir_difference = measure_ir();
        set_rgb_led(ir_difference);
        delay_ms(100); 
    }
}

void gpio_init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN; 

    GPIOA->MODER |= (GPIO_MODER_MODE5_0 | GPIO_MODER_MODE6_0 | GPIO_MODER_MODE7_0); 
    GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR5 | GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7); 

    GPIOC->MODER |= GPIO_MODER_MODE7_0; 
    GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR7; 
}

void gpio_write(Pin pin, int value) {
    GPIO_TypeDef* gpio_port = GET_PORT(pin);
    uint16_t gpio_pin = GET_PIN_INDEX(pin);

    if (value) {
        gpio_port->ODR |= (1 << gpio_pin);
    } else {
        gpio_port->ODR &= ~(1 << gpio_pin);
    }
}