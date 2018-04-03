#include <p24Fxxxx.h>
#include "xc.h"
#include "step_delays.h"
void delay(int delay_in_ms){
    //delay function, uses assembly defined delay
    int i = 0;
    for( i = delay_in_ms; i>0; i--){
        step_delay();
    }
}
