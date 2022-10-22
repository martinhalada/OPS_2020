#include "mbed.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
#include <string>

Thread touchscreenThread;
int cas;
InterruptIn button(USER_BUTTON);
LowPowerTicker alarm_event;

void inicializuj(){
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, LCD_FB_START_ADDRESS);
    BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);
    BSP_LCD_Clear(LCD_COLOR_BLACK);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
}

void vykresli(){
    BSP_LCD_FillRect(380, 50, 40, 10);
    BSP_LCD_FillRect(380, 140, 40, 10);
    BSP_LCD_FillRect(395, 125, 10, 40);
    BSP_LCD_DisplayStringAt(380, 220, (uint8_t *)"SET", LEFT_MODE);
}

void vypisCas(){
    BSP_LCD_Clear(LCD_COLOR_BLACK);
    vykresli();
    int minuty = (int)cas/60;
    int sec = cas%60;
    string casString = to_string(minuty) + ":" + to_string(sec);
    BSP_LCD_DisplayStringAt(50, 20, (uint8_t *)"Cas do alarmu", LEFT_MODE);
    BSP_LCD_DisplayStringAt(140, 120, (uint8_t *)casString.c_str(), LEFT_MODE);

}

void odecti(int hodnota){
    cas-=hodnota;
    if (cas < 10) cas = 10;
    vypisCas();
}

void pricti(int hodnota){
    cas+=hodnota;
    vypisCas();
}

void spustit(){
    cas-=1;
    if(cas<1) BSP_LCD_Clear(LCD_COLOR_RED);
    else vypisCas(); 
}

void akce(uint16_t x,uint16_t y,uint16_t lastX,uint16_t lastY){
    if (x!=lastX && y!=lastY) {
        if(x>370 && x<430 && y>30 && y<70){
            odecti(10);
        }
        if(x>370 && x<430 && y>120 && y<160){
            pricti(10);
        }  
        if(x>370 && x<430 && y>200 && y<240){
            alarm_event.attach(&spustit, 1s);
        }
    }       
}

void touchScreenRead(){
    TS_StateTypeDef TS_State;
    BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
    uint16_t x=0, y=0, lastX=0, lastY=0;
    for(;;) {
        BSP_TS_GetState(&TS_State);
        if (TS_State.touchDetected) {
            x = TS_State.touchX[0];
            y = TS_State.touchY[0];    
            akce(x,y,lastX,lastY);   
            lastX=x;
            lastY=y;     
        }
    }
}

void rise_handler(){
    alarm_event.detach(); 
    cas = 120;
    vypisCas();     
}

int main()
{
    cas=120;
    inicializuj();
    vypisCas();
    touchscreenThread.start(touchScreenRead);
    button.rise(rise_handler);
    for(;;){
    }
}
