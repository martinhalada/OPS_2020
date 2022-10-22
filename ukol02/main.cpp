#include "mbed.h"
#include "stm32746g_discovery_lcd.h"

void vypisText(char jmeno[]){
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

    BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)jmeno, LEFT_MODE);
}

int main()
{
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, LCD_FB_START_ADDRESS);
    BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);
    BSP_LCD_Clear(LCD_COLOR_BLACK);
    char jmeno[] = "Martin Halada";
    
    while (1) {        
        vypisText(jmeno);
    }
}
