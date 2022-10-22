#include "mbed.h"
#include "stm32746g_discovery_lcd.h"
#include "ResetReason.h"
#include <string>
using namespace std;

static BufferedSerial pc(USBTX, USBRX);

void inicializuj(){
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, LCD_FB_START_ADDRESS);
    BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);
    BSP_LCD_Clear(LCD_COLOR_BLACK);
}

string reset_reason(reset_reason_t reason)
{
    switch (reason) {
        case RESET_REASON_PIN_RESET:
            return "Reset tlacitkem";
        case RESET_REASON_SOFTWARE:
            return "Reset seriovou linkou";
        case RESET_REASON_WATCHDOG:
            return "Reset pres Watchdog";
        default:
            return "Reset";
    }
}

void vypisText(reset_reason_t reason){
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

    BSP_LCD_DisplayStringAt(0, 120, (uint8_t *)reset_reason(reason).c_str(), CENTER_MODE);
}

int main()
{
    inicializuj();   
    reset_reason_t reason = ResetReason::get();
    vypisText(reason);    

    Watchdog &watchdog = Watchdog::get_instance();
    watchdog.start(5000);

    char c;
    for(;;) {
        if (pc.readable()){
            pc.read(&c, sizeof(uint8_t));
            if(c == 'r'){
                NVIC_SystemReset();
            }
        }
    }
    
}
