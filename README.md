# Embedded-systems-projects
**In this README, you will find descriptions of three projects.**

---

# 1. Square Root Approximation with Bisection Method in ARM Assembly
---
## Overview
This project demonstrates an integer-based approach to approximating the square root of a number using the bisection method, written in ARM assembly for the STM32F401RE microcontroller (ARM Cortex-M4). The subroutine `my_sqrt` approximates the square root of an integer in the range 0 to \(2^{31} - 1\), returning the truncated integer result.

The project includes:
- ARM assembly subroutine using the bisection method
- Main C function that calls the assembly subroutine for testing
- Inline assembly following ARM calling conventions for compatibility with the C function

## Requirements
- Approximate the integer square root of an input between 0 and \(2^{31} - 1\).
- Implemented in ARM inline assembly.
- Complies with ARM calling conventions.

## Software Design
The code follows the bisection method for square root approximation:
1. Initialize range values `a = 0` and `b` to the approximate square root of the maximum value.
2. Iteratively compute the midpoint `c = (a + b) / 2`.
3. If `c * c` matches the target, exit with `c` as the result.
4. If `c * c < x`, update `a = c`; otherwise, update `b = c`.
5. Repeat until convergence.

The following pseudocode outlines the bisection method:
```c
done = 0
a = 0
b = sqrt(max)
c = -1
do {
    c_old = c
    c = (a + b) / 2
    if (c * c == x) {
        done = 1
    } else if (c * c < x) {
        a = c
    } else {
        b = c
    }
} while (!done && (c != c_old))
return c
```

## File Structure
- **`my_sqrt.c`**: Contains the main function to test the `my_sqrt` function with various test cases and Contains the assembly code for `my_sqrt` using the ARM M0+ inline assembly.


## Explanation of Key Operations
1. **MOV, LDR**: Initializes variables for `a`, `b`, `c`, and `done`.
2. **Loop Logic**: Each iteration updates `a`, `b`, or `c` based on the result of `c * c` compared to `x`.
3. **Branching**: Conditional branches (`BLS`, `BEQ`) ensure correct logic flow and exit criteria.
4. **Return**: The final result is stored in `r0` and returned.
---

# 2. Human Response Timer - Interrupt Project
---
## Assignment Overview
This project implements a "Human Response Timer" using an STM32F401RE board, aimed at measuring human response time to an LED lighting event. The project leverages interrupts to accurately record the time taken for a user to press a button after an LED is lit. The project allows for an analysis of processor activity within a given human response time.


### Introduction

#### Overview
This project is designed to gauge how fast a person can respond to an LED event by pressing a switch. The response timer utilizes interrupts to log response time and provides insights into the time taken for a human reaction relative to the processing capability of the STM32F401RE microcontroller.


### Requirements

For this project, the following hardware and software are required:

- **STM32F401RE Development Board**
- **KEIL µVision5 MDK IDE**  

### Project Details

#### Hardware

The hardware setup includes:
- **SW1**: A switch to control the system’s response timer.
- **RGB LEDs**: Output devices for visual feedback.

#### Software

The main code performs the following operations:
1. **Peripheral Initialization**: Initialize LEDs, switch, and timers.
2. **Main Loop**:
   - Turn off all LEDs.
   - Clear the response counter.
   - Wait for a random period (1-3 seconds).
   - Turn on one LED.
   - Increment the counter until the interrupt service routine (ISR) is triggered.
   - Save the counter value in memory.
   - Wait for 5 seconds before restarting the loop.

3. **Interrupt Service Routine (ISR)**:
   - Set a flag to indicate the ISR has been executed.


### Testing

To test the response timer:
1. Run the program in **KEIL µVision5**.
2. Set a **breakpoint** in the main function after the button press is detected.
3. Examine the `counter` variable in the **watch window** to view the recorded iterations.


---

# 3. ADC project: Infrared Proximity Sensor  

---

## Introduction  

### Overview  
This project involves the creation of a device that uses reflected infrared (IR) light to detect nearby objects. The proximity sensor system uses an IR emitter (LED) and a phototransistor detector to measure the IR light reflected from objects and determine proximity.


## Requirements  

This project requires the following hardware and software:  
### Hardware:
- Nucleo-F401RE board  
- IR Emitter (LED)  
- IR Detector (Phototransistor)  
- Logic Analyzer or Oscilloscope  
- RGB LED  
- Resistors (150 Ω, 10 kΩ)  

### Software:
- KEIL µVision5 MDK IDE  


## Details  

### Hardware  
The proximity sensor works by emitting infrared (IR) light through the IR emitter (LED), which then reflects off nearby objects. The reflected IR light is detected by the phototransistor, and the software analyzes the IR brightness levels to determine proximity. 


### Components:
- **R1 (150 Ω)**: Current-limiting resistor for IR LED.  
- **R2 (10 kΩ)**: Adjusts sensitivity of the phototransistor.  
- **D1 (IR LED)**: Emits IR light.  
- **Q1 (Phototransistor)**: Detects the reflected IR light.


## Software  

The software includes functions to initialize the system, measure IR brightness, and control the RGB LED based on the detected proximity.  
Key functions include:
- **Initialization**: Configures GPIO pins and ADC input.  
- **Delay**: Implements delay for measurement timing.  
- **Measure IR**: Measures the difference in IR brightness and controls the RGB LED based on proximity.
- **Pick LEDs**: Lights the RGB LED based on the IR brightness level.


## Procedure  

### Basic Steps  
1. Load the project into the KEIL IDE.  
2. Build the project and download it to the Nucleo-F401RE board in debug mode.  
3. Add necessary variables to the watch window for live monitoring.

### Analysis  
Perform the following analysis to calibrate and test the proximity sensor:

#### Ambient Light Analysis  
- Disconnect the IR emitter and observe the IR brightness reading. How steady is it?  

#### Time Delay Analysis  
- Analyze the impact of delay constants (T_LED_ON, T_LED_OFF) on sensor sensitivity.  

#### Range Analysis  
- Test the sensor's range with different objects (e.g., paper, hand, arm) at varying distances.

#### Calibration  
- Adjust the threshold values in the software to change the RGB LED color based on object proximity.


## Calibration  
To calibrate the sensor for proximity detection, adjust the threshold array in `main.c`. The RGB LED will display colors according to the following distances:

- **Green**: No object within 20 cm  
- **Blue**: 16-20 cm  
- **Yellow**: 12-16 cm  
- **Red**: 8-12 cm  
- **Magenta**: 4-8 cm  
- **White**: 0-4 cm  


## Conclusion  
This project demonstrates the design and implementation of an infrared proximity sensor, with practical applications in object detection and distance measurement using embedded systems.
