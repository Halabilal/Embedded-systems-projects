#include <stdio.h>

__asm int my_sqrt(int x)
{
    MOV     r1, #0        // Set a = 0
    LDR     r2, =0x10000  // Set b = square root of largest possible argument (e.g. ~216)
    MOV     r3, #-1       // Set c = -1
    MOV     r4, #0        // Set done = 0

loop
    CMP     r4, #1        // Check if done
    BEQ     end_loop      // Exit loop if done
    
    MOV     r5, r1        // Load a into r5
    ADD     r5, r5, r2    // a + b
    ASRS    r5, r5, #1    // (a + b) / 2
    CMP     r3, r5        // Compare c with new c
    BEQ     set_done      // Set done if c == (a + b) / 2
    MOV     r3, r5        // c = (a + b) / 2
    
    MUL     r6, r3, r3    // c * c
    CMP     r6, r0        // Compare c * c with x
    BEQ     set_done      // Set done if c * c == x
    
    CMP     r6, r0        // Compare c * c with x
    BLS     less_than_x   // If c * c < x, jump to less_than_x
    
greater_than_x
    MOV     r2, r3        // b = c
    B       loop          // Repeat loop
    
less_than_x
    MOV     r1, r3        // a = c
    B       loop          // Repeat loop
    
set_done
    MOV     r4, #1        // done = 1
    B       loop          // Repeat loop

end_loop
    MOV     r0, r3        // Move the result to r0 (return value)
    BX      lr            // Return c
}

int main(void)
{
    volatile int r, j = 0;
    int i;
    
    // Test cases
    r = my_sqrt(0);   // Should be 0
   
    
    r = my_sqrt(25);  // Should be 5
    
    
    r = my_sqrt(133); // Should be 11
 
    r=0;
    // Additional test cases
    for (i = 0; i < 10000; i++) {
        r = my_sqrt(i);
        j += r;
    }
   
    while (1);
}

