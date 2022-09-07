#include "mbed.h"
// This program will blink LED1, LED2 and LED4
// using ARM assembly language for LED1,
// C mbed LPC1768 I/O register names (a predefined pointer to sturcture) for LED2,
// and normal mbed API functions for LED4
// declare external assembly language function (in a *.s file)
extern "C" int my_asm(int value);
// declare LED outputs - (uses constructor set I/O function and direction of pins)
DigitalOut myled1(LED1);
DigitalOut myled4(LED4);
DigitalOut myled2(LED2);

int main() {
    bool  value = false;
    uint32_t myled1_bit_mask=0;
    // loop forever
    while (1) {
        //mbed API function with operator overloading to control LED4
        myled4 = value;
        //call an assembly language function (my_asm.s) to control LED1
        my_asm(value);        
        //control LED2 using mbed C/C++ hardware I/O register names
        //from LPC1768 data manual the LED2 pin goes to GPIO1 port bit 20
        myled1_bit_mask = 0x00100000; // 0x0100000 = 1<<20 all "0"s with a "1" in bit 20
        if (value==0) {
            LPC_GPIO1->FIOCLR = myled1_bit_mask; //Write to I/O register that clears bits (FIOCLR for GPIO1)
        } else {
            LPC_GPIO1->FIOSET =  myled1_bit_mask;//Write to I/O register that sets bits (FIOSET for GPIO1)
        }        
        // flip value and wait
        value = ! value;
        wait(0.2);
    }
}
