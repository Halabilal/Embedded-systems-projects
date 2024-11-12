#include <platform.h>
#include <gpio.h>
#include <stdlib.h>
#include <stdint.h>
#include "leds.h"
#include "delay.h"

#define SWITCH_PIN PA_1  // Define a new pin for the switch

volatile int done = 0; // Volatile because it is modified in an ISR

// Array of possible LED colors
typedef enum {
    RED,
    GREEN,
    BLUE,
    ORANGE,
    CYAN,
    MAGENTA,
    WHITE,
    COLOR_COUNT
} led_color_t;

// LED state enum
typedef enum {
    LED_OFF_STATE,
    LED_ON_STATE
} led_state_t;

// Function prototypes
void button_press_isr(int status);
void set_led_color(led_color_t color, led_state_t state);
float measure_response_time(void);

void button_press_isr(int status) {
    done = 1; // Set flag to indicate ISR execution
}

int main(void) {
    // Initialize LEDs
    leds_init();
    
    // Set up on-board switch
    gpio_set_mode(SWITCH_PIN, PullUp);
    gpio_set_trigger(SWITCH_PIN, Rising);
    gpio_set_callback(SWITCH_PIN, button_press_isr);

    // Enable Global Interrupts
    __enable_irq();

    while (1) {
        float total_time = 0;
        int num_measurements = 10;
        
        for (int i = 0; i < num_measurements; i++) {
            // Turn off all LEDs
            set_led_color(RED, LED_OFF_STATE);

            // Wait a random amount of time (1-3 seconds)
            delay_ms(rand() % 2000 + 1000);

            // Randomly select a color
            led_color_t color = rand() % COLOR_COUNT;
            led_state_t state = (rand() % 2 == 0) ? LED_ON_STATE : LED_OFF_STATE;

            // Set LED color and state
            set_led_color(color, state);

            // Measure response time
            float response_time = measure_response_time();
            total_time += response_time;

            // Reset done flag
            done = 0;

            // Wait for approximately 5 seconds
            delay_ms(5000);
        }

        // Calculate average response time
        float average_time = total_time / num_measurements;
        
        // Here, you can log or display the average response time
        // For now, we'll just keep running the loop
    }
    return 0;  // This line will never be reached, but keeps the compiler happy
}

void set_led_color(led_color_t color, led_state_t state) {
    // Turn off all LEDs first
    leds_set(LED_OFF, LED_OFF, LED_OFF);
    
    if (state == LED_OFF_STATE) {
        return; // No need to set any color, just keep LEDs off
    }
    
    switch (color) {
        case RED:
            leds_set(LED_ON, LED_OFF, LED_OFF);
            break;
        case GREEN:
            leds_set(LED_OFF, LED_ON, LED_OFF);
            break;
        case BLUE:
            leds_set(LED_OFF, LED_OFF, LED_ON);
            break;
        case ORANGE:
            leds_set(LED_ON, LED_ON, LED_OFF);
            break;
        case CYAN:
            leds_set(LED_OFF, LED_ON, LED_ON);
            break;
        case MAGENTA:
            leds_set(LED_ON, LED_OFF, LED_ON);
            break;
        case WHITE:
            leds_set(LED_ON, LED_ON, LED_ON);
            break;
        default:
            leds_set(LED_OFF, LED_OFF, LED_OFF);
            break;
    }
}

float measure_response_time(void) {
    uint32_t count = 0;
    uint32_t clock_speed = 84000000; // Clock speed in Hz (84 MHz)

    // Repeat until ISR has been triggered
    while (!done) {
        count++; // Increment counter
    }

    // Convert count to time
    return (float)count * 1000 / clock_speed; // Time in milliseconds
}
