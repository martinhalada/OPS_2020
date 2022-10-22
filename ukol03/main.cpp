/*
 * Copyright (c) 2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed.h"
#include "stm32746g_discovery_lcd.h"
#include <string>

DigitalOut led1(LED1);
DigitalIn button(USER_BUTTON);

Thread thread1;
Thread thread2;
Thread thread3;
Thread thread4;
string textVlakno1;
string textVlakno2;
int pocitadlo;
string cisloNaDisplej;

void vykresliNaDisplej(int pos, uint8_t *text, Text_AlignModeTypdef Mode, int cas){
    BSP_LCD_DisplayStringAt(0, LINE(pos), text, Mode);
    ThisThread::sleep_for(cas);
    BSP_LCD_ClearStringLine(pos);
    ThisThread::sleep_for(cas);
}

void led1_thread()
{
    while (true) {
        led1 = !led1;
        ThisThread::sleep_for(200);
    }
}

void lcd1_thread(){
    while(true){
        vykresliNaDisplej(0, (uint8_t *)textVlakno1.c_str(), LEFT_MODE, 300);
    }    
}

void lcd2_thread(){
    while(true){       
        vykresliNaDisplej(5, (uint8_t *) textVlakno2.c_str(), CENTER_MODE, 500);
    }    
}

void button_thread(){
    while(true){
        if(button){         
            cisloNaDisplej = to_string(pocitadlo);
            vykresliNaDisplej(10, (uint8_t *) cisloNaDisplej.c_str(), RIGHT_MODE, 400);
            pocitadlo++;
        }
        if(pocitadlo>10){
            pocitadlo=0;    
        }
    }
}

int main()
{
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, LCD_FB_START_ADDRESS);
    BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);
    BSP_LCD_Clear(LCD_COLOR_BLACK);
    
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    
    textVlakno1 = "Vlakno 1";
    textVlakno2 = "Vlakno 2";
    pocitadlo=0;
    
    while(true){   
        thread1.start(lcd1_thread);
        thread2.start(lcd2_thread);
        thread3.start(led1_thread);
        thread4.start(button_thread);
    }

}
