#include "mbed.h"

DigitalOut myled(LED1);

int main() { 
    int pocitadlo=0;
     
    while(1) {
        
        if(pocitadlo == 3){
            myled = 1; // LED is ON
            wait(1.0); // 1000 ms
            myled = 0; // LED is OFF
            wait(0.2); // 200 ms
            
            pocitadlo=0;
        }else{
            myled = 1; // LED is ON
            wait(0.2); // 200 ms
            myled = 0; // LED is OFF
            wait(0.2); // 200 ms
        }
        pocitadlo++;        
    }
}
