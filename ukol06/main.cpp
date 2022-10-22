/*
 * Copyright (c) 2006-2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"

InterruptIn button(USER_BUTTON);
Thread thread1;
EventQueue queue(32 * EVENTS_EVENT_SIZE);


void rise_handler(){
    queue.call(printf," Stisknuto tlacitko. \n");
}

int main(){

    thread1.start(callback(&queue, &EventQueue::dispatch_forever));
    
    button.rise(rise_handler);
    
    for( ; ; ){
    } 
}
