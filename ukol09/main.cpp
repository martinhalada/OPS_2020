#include "mbed.h"
#include "stm32746g_discovery_lcd.h"
#include "EthernetInterface.h"
#include <string>

//InterruptIn button(USER_BUTTON);
DigitalIn button(USER_BUTTON);
EthernetInterface net;
string api_key = "7BGKHY6ER3G9WR3M";
string field1 = "18000074";
Thread buttonThread;

void inicializuj(){
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, LCD_FB_START_ADDRESS);
    BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);
    BSP_LCD_Clear(LCD_COLOR_BLACK);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
}

void vypisText(uint8_t *text, int y){
    BSP_LCD_DisplayStringAt(0, y, text, LEFT_MODE);
}

void odeslat(){
    BSP_LCD_Clear(LCD_COLOR_BLACK);

    net.connect();    
    SocketAddress adresa;
    net.get_ip_address(&adresa);
    
    if(adresa.get_ip_address()){
        vypisText((uint8_t *)"IP adresa OK", 0);
    }else{
        vypisText((uint8_t *)"IP adresa nenalezena", 0);
        return;
    }
    
    nsapi_error_t err = net.gethostbyname("api.thingspeak.com", &adresa);
    if(err == NSAPI_ERROR_OK){
        vypisText((uint8_t *)"Pripojeni ke cloudu OK", 30);
    }else{
        vypisText((uint8_t *)"Do cloudu se nejde pripojit", 30);
        return;
    }
    
    net.gethostbyname("api.thingspeak.com",&adresa);
    
    TCPSocket socket;    
    socket.open(&net);    
    adresa.set_port(80);
    socket.connect(adresa);
    
    time_t cas = time(NULL);
    char buffer[32];
    strftime(buffer, 32, "%Y-%m-%d %H:%M:%S\n", localtime(&cas));
    string field2 = buffer;
    string param = api_key + "&field1=" + field1 + "&field2=" + field2 + "\r\n\r\n"; 
    string delka = to_string(param.length());
    string request = "POST /update HTTP/1.1\nHost: api.thingspeak.com\nConnection: close\nX-THINGSPEAKAPIKEY: "+api_key+"\nContent-Type: application/x-www-form-urlencoded\nContent-Length: "+to_string(param.length())+"\n\n"+param;
    
    char postBuffer[request.length()];
    strcpy(postBuffer, request.c_str());
    
    vypisText((uint8_t *)"Posilam data", 60);
    int scount = socket.send(postBuffer, sizeof postBuffer);

    socket.close();
    net.disconnect();
    vypisText((uint8_t *)"Spojeni ukonceno v poradku", 90);    
}

void rise_handler(){
    bool bPressed = false;
    for(;;){
        while(button == true){
            if(bPressed == false){
                bPressed = true;
                Thread *vlakno = new Thread();
                vlakno->start(odeslat);  
                vlakno->join();  
            }              
        }
        if(bPressed == true){
            bPressed = false;  
        }         
    }
}

int main()
{
    inicializuj();
    string text = "Tlacitkem odesli data";
    vypisText((uint8_t *)text.c_str(), 140);
    set_time(1607257420);
    
    //button.rise(rise_handler);
    buttonThread.start(rise_handler);
    
    for(;;){
    }
}