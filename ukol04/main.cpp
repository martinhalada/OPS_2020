#include "mbed.h"
#include "stm32746g_discovery_lcd.h"

InterruptIn button(USER_BUTTON);
Thread thread1;
EventQueue queue(32 * EVENTS_EVENT_SIZE);

void printText(){
    queue.call(BSP_LCD_Clear,LCD_COLOR_BLACK);
    queue.call(BSP_LCD_SetBackColor, LCD_COLOR_BLACK);
    queue.call(BSP_LCD_SetTextColor,LCD_COLOR_WHITE);
    queue.call(BSP_LCD_DisplayStringAt, 0, LINE(5), (uint8_t *)"Stiskni tlacitko", CENTER_MODE);
}

void drawCircle(){
    queue.call(BSP_LCD_Clear,LCD_COLOR_BLACK);
    queue.call(BSP_LCD_SetTextColor,LCD_COLOR_WHITE);
    queue.call(BSP_LCD_FillCircle, 100, 100, 80);       
}

void rise_handler(){
    drawCircle();
}

void fall_handler(){   
    printText();
}

void initDisplay(){
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, LCD_FB_START_ADDRESS);
    BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);
    BSP_LCD_Clear(LCD_COLOR_BLACK);        
}

int main(){
    initDisplay();
    
    thread1.start(callback(&queue, &EventQueue::dispatch_forever));
    
    button.rise(rise_handler);
    button.fall(fall_handler);
    
    for( ; ; ){
    } 
}

