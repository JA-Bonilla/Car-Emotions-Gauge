//------------------------------------------------------------------------------
//OBD-II

    #include <ELMo.h>

    ELMo ELM;

    String Response = "";
    String Last = "";

    int PIDS[5];
    String CS[20];
    char fcodes[26];

    int looping = 0;
    int timer = 0;

    String Pid = "";

//-------------------------------------------------------------------------------------------
//Buttons

#define BUTTON_PIN1 15
#define BUTTON_PIN2 2

int reply;
int BS1;
int BS2;    

//------------------------------------------------------------------------------
//Wi-Fi

IPAddress server(192, 168, 0, 10);
WiFiClient client;

const char* ssid = "WiFi_OBDII";

//-------------------------------------------------------------------------------------------
//Display

String GIF_Hungry = "/Hungry.gif";
String GIF_Angry = "/Angry.gif";
String GIF_Blink = "/Blink.gif";
String GIF_Cold = "/Cold.gif";
String GIF_Look1 = "/Look1.gif";
String GIF_Look2 = "/Look2.gif";
String GIF_Melt = "/Melt.gif";
String GIF_Engine = "/Engine.gif";
String GIF_Happy = "/Happy.gif";

#include <Arduino_GFX_Library.h>

#define GFX_BL DF_GFX_BL // default backlight pin, you may replace DF_GFX_BL to actual backlight pin

Arduino_DataBus *bus = new Arduino_ESP32SPI(14 /* DC */, 5 /* CS */, 18 /* SCK */, 23 /* MOSI */, GFX_NOT_DEFINED /* MISO */, VSPI /* spi_num */);

Arduino_GFX *gfx = new Arduino_GC9A01(bus, 22 /* RST */, 0 /* rotation */, true /* IPS */);

#include <SPIFFS.h>

#include "GifClass.h"
static GifClass gifClass;

int x = 0;
int Blinking;

//-------------------------------------------------------------------------------------------
//Display

void Display(String GIF){
  
File gifFile = SPIFFS.open(GIF, "r");
  
  if (!gifFile || gifFile.isDirectory())
  {
    Serial.println(F("ERROR: open gifFile Failed!"));
    gfx->println(F("ERROR: open gifFile Failed!"));
  }
  else
  {
    // read GIF file header
    gd_GIF *gif = gifClass.gd_open_gif(&gifFile);
    if (!gif)
    {
      Serial.println(F("gd_open_gif() failed!"));
    }
    else
    {
      uint8_t *buf = (uint8_t *)malloc(gif->width * gif->height);
      if (!buf)
      {
        Serial.println(F("buf malloc failed!"));
      }
      else
      {
        int16_t x = (gfx->width() - gif->width) / 2;
        int16_t y = (gfx->height() - gif->height) / 2;

        Serial.println(F("GIF video start"));
        int32_t t_fstart, t_delay = 0, t_real_delay, delay_until;
        int32_t res = 1;
        int32_t duration = 0, remain = 0;
        while (res > 0)
        {
          t_fstart = millis();
          t_delay = gif->gce.delay * 10;
          res = gifClass.gd_get_frame(gif, buf);
          if (res < 0)
          {
            Serial.println(F("ERROR: gd_get_frame() failed!"));
            break;
          }
          else if (res > 0)
          {
            gfx->drawIndexedBitmap(x, y, buf, gif->palette->colors, gif->width, gif->height);

            t_real_delay = t_delay - (millis() - t_fstart);
            duration += t_delay;
            remain += t_real_delay;
            delay_until = millis() + t_real_delay;
            while (millis() < delay_until)
            {
              delay(1);
            }
          }
        }
        Serial.println(F("GIF video end"));
        Serial.print(F("duration: "));
        Serial.print(duration);
        Serial.print(F(", remain: "));
        Serial.print(remain);
        Serial.print(F(" ("));
        Serial.print(100.0 * remain / duration);
        Serial.println(F("%)"));

        gifClass.gd_close_gif(gif);
        free(buf);
      }
    }
  }
}

//-------------------------------------------------------------------------------------------
//Clear DTC's
void ClearMenu(){
  gfx->fillScreen(BLACK);
  
  gfx->setTextSize(3);
  gfx->setTextColor(ORANGE); 

  gfx->setCursor(35, 80);
  gfx->print("Clear DTC?");

  gfx->setCursor(55, 150);
  gfx->print("NO | YES");

  delay(1000);

  int response = buttonWait();

  if(response == 2){
    Clear_DTC();
  }

  gfx->fillScreen(BLACK);  
  
}

//-------------------------------------------------------------------------------------------
//Menu

void Menu()
{
  int x = 1;

  while(x == 1)
  {
  
    gfx->fillScreen(BLACK);
    gfx->setTextColor(WHITE);
    gfx->setTextSize(2);
    gfx->setCursor(95, 30);
    gfx->println("MENU");

    gfx->setCursor(95, 35);    
    gfx->println("____");
  
    gfx->setTextSize(3);
    gfx->setCursor(85, 75);
    gfx->println("HOME");

    gfx->setTextColor(ORANGE);
    gfx->setCursor(85, 85);
    gfx->println("____");

    gfx->setTextColor(WHITE);
    gfx->setTextSize(3);
    gfx->setCursor(75, 120);
    gfx->println("CODES");

    gfx->setTextColor(ORANGE);
    gfx->setTextSize(2);
    gfx->setCursor(95, 180);
    gfx->println("V | >"); 

    delay(750);

    int response = buttonWait();

    int y = 1;
    
    if (response == 1)
    {
      while (y == 1)
      {
      
        gfx->fillScreen(BLACK);
        gfx->setTextColor(WHITE);
        gfx->setTextSize(2);
        gfx->setCursor(95, 30);
        gfx->println("MENU");

        gfx->setCursor(95, 35);    
        gfx->println("____");
  
        gfx->setTextSize(3);
        gfx->setCursor(85, 75);
        gfx->println("HOME");

        gfx->setTextColor(WHITE);
        gfx->setTextSize(3);
        gfx->setCursor(75, 120);
        gfx->println("CODES");

        gfx->setTextColor(ORANGE);
        gfx->setCursor(75, 130);
        gfx->println("_____");

        gfx->setTextSize(2);
        gfx->setCursor(95, 180);
        gfx->println("V | >"); 

        delay(750);

        int response = buttonWait();

        int z = 1;

        if (response == 1)
        {
          while (z == 1){
           gfx->fillScreen(BLACK);
           gfx->setTextColor(WHITE);
           gfx->setTextSize(2);
           gfx->setCursor(95, 30);
           gfx->println("MENU");

           gfx->setCursor(95, 35);    
           gfx->println("____");
  
           gfx->setTextSize(3);
           gfx->setCursor(75, 75);
           gfx->println("CODES");

           gfx->setTextColor(WHITE);
           gfx->setTextSize(3);
           gfx->setCursor(40, 120);
           gfx->println("CLEAR DTC");

           gfx->setTextColor(ORANGE);
           gfx->setCursor(40, 130);
           gfx->println("_________");

           gfx->setTextSize(2);
           gfx->setCursor(95, 180);
           gfx->println("V | >"); 

            delay(750);

            response = buttonWait();

            if(response == 1){
              y = 2;
              z = 2;
            }

            if(response == 2){
              ClearMenu();
            }
        
          }
        }

        if(response == 2)
        {
          DDTC();
        }

      }

    }

    if(response == 2)
    {
      x=2;
      gfx->fillScreen(BLACK);
      Display(GIF_Blink);
    }
  }


}

//-------------------------------------------------------------------------------------------
//Display DTC's

void DDTC(){
  Display(GIF_Engine);
  
  gfx->fillScreen(BLACK);

  int DTCC = DTC();
  
  gfx->setTextSize(3);
  gfx->setCursor(25, 60);
  gfx->setTextColor(ORANGE);
  gfx->print(DTCC);
  gfx->setCursor(60, 60); 
  gfx->print("DTC Codes");

  gfx->setCursor(25, 110);
  gfx->print("View Codes?");

  gfx->setCursor(55, 180);
  gfx->print("NO | YES");

  delay (1000);

  int response = buttonWait();
  
  if(response == 2)
  {
    if(DTCC != 0){
    Read_DTC();
    Start_DTC();
    Add_DTC();
    }
    
    gfx->fillScreen(BLACK);

    int x = 1;

    while(x == 1)
    { 

    gfx->fillScreen(BLACK);

    int vert = 50;

    gfx->setCursor(85, 20);
    gfx->setTextColor(ORANGE);
    gfx->setTextSize(2);
    gfx->println("Codes:");

    gfx->setCursor(60, vert);

    if(DTCC != 0){

    gfx->setTextSize(4);
    gfx->setCursor(60, vert);
    Serial.println(CS[0]);
    if(DDTC != 0){ gfx->print(CS[0]); }
    vert += 40;

    gfx->setCursor(60, vert);
    Serial.println(CS[1]);
    if(DTCC != 0){ gfx->print(CS[1]); }
    vert += 40;

    gfx->setCursor(60, vert);
    Serial.println(CS[2]);
    if(DTCC != 0){ gfx->print(CS[2]); }
    vert += 40;    

    }

    gfx->setTextSize(2);
    gfx->setCursor(40, 180);
    gfx->println("HOME | Page 2"); 

    delay (1000);

    response = buttonWait();

    if(response == 1)
    {
      x = 2;
    }

    if(response == 2)
    {
      gfx->fillScreen(BLACK);
      
      int vert = 50;

      gfx->setCursor(85, 20);
      gfx->setTextColor(ORANGE);
      gfx->setTextSize(2);
      gfx->println("Codes:");

      gfx->setCursor(60, vert);

      if (DTCC > 3){

      gfx->setTextSize(4);
      gfx->setCursor(60, vert);
      if(DTCC != 0){ gfx->print(CS[3]); }
      vert += 40;

      gfx->setCursor(60, vert);
      if(DTCC != 0){ gfx->print(CS[4]); }
      vert += 40;

      gfx->setCursor(60, vert);
      if(DTCC != 0){ gfx->print(CS[5]); }
      vert += 40; 

      }

      gfx->setTextSize(2);
      gfx->setCursor(40, 180);
      gfx->println("HOME | Page 1"); 

      delay (1000);

      response = buttonWait();

      if(response == 1)
      {
        x = 2;
      }

      }
     }
  }

  gfx->fillScreen(BLACK);    

  delay (1000);
  
}

//-------------------------------------------------------------------------------------------
//Button Wait

int buttonWait(){
  
  while(1){
    
  int buttonState1 = digitalRead(BUTTON_PIN1);
  int buttonState2 = digitalRead(BUTTON_PIN2);

    if (buttonState1 == 0) {
      Serial.println("Button 1");
      return 1;
    }
    if (buttonState2 == 0) {
      Serial.println("Button 2");
      return 2;
    }
  }
}

//------------------------------------------------------------------------------
//Hex to Dec

int Hex_to_Dec(char x, int count){
    int y = x - '0';
    if(y>9){
        y = y - 7;
    }

    int a = y;
    if(count != 20) {
        a *= pow(16, count);
    }
    return a;
}

//------------------------------------------------------------------------------
//Hex to Bin

int Hex_to_Bin(char x, int Bit){
    int y = x - '0';
    if(y>9){
        y = y - 7;
    }

    int result = 0;

    if (Bit == 1 || Bit == 2 || Bit == 4) {
        result = (y > 7) ? 1 : 0;
    } else if (Bit == 3) {
        result = (y & 0x01) ? 1 : 0;
    }
    else if (Bit == 5){
       if(y == 2 || y == 3 || y == 6 || y == 7 || y == 10 || y == 11 || y == 14 || y == 15){
        result = 1;
       }
       else{
        result = 0;
       }
    }

    return result;
}

//------------------------------------------------------------------------------
//Check Supported Pids 0-20
 
void PID(){

    Last = Response;
    
    Response = ELM.send("01 00");

    while(Response == "NO DATA" || Response == Last){
        
        Response = ELM.send("01 00");

        if(WiFi.status() != WL_CONNECTED){
            ESP.restart();
        }
    }

    int STR_Len = Response.length() + 1;

    char pid[STR_Len];

    Response.toCharArray(pid, STR_Len);

    char value[] = {pid[4], pid[5], pid[6], pid[7]};

    int Bit = 1;
    int z = 0;
    int loop = 1;

    while (loop == 1){
        PIDS[z] = Hex_to_Bin(value[z], Bit);
        Bit++;
        z++;
        
        if(z == 4){
            loop = 0;
        }
    }
}

//------------------------------------------------------------------------------
//Check Supported Pids 21 - 40

void PID2(){

    Last = Response;
    
    Response = ELM.send("01 20");

    while(Response == "NO DATA" || Response == Last){
        
        Response = ELM.send("01 20");

        if(WiFi.status() != WL_CONNECTED){
            ESP.restart();
        }
    }

    int STR_Len = Response.length() + 1;

    char pid[STR_Len];

    Response.toCharArray(pid, STR_Len);

    char value[] = {pid[4], pid[5], pid[6], pid[7]};

    int Bit = 5;

    PIDS[4] = Hex_to_Bin(value[3], Bit);
}

//------------------------------------------------------------------------------
//Read # of DTC's

int DTC(){

    if(looping == 0){
      Last = Response;
    }
    else{
      Last = "123";
    }

    Response = ELM.send("01 01");

    while(Response == "NO DATA" || Response == Last){
        Response = ELM.send("01 01");

        if(WiFi.status() != WL_CONNECTED){
            ESP.restart();
        }
    }

    int STR_Len = Response.length() + 1;

    char dtc[STR_Len];

    Response.toCharArray(dtc, STR_Len);

    char value[] = {dtc[4],dtc[5]};
    int store[1];

    store[0] = Hex_to_Dec(value[0], 1);
    store[1] = Hex_to_Dec(value[1], 0);

    float result = store[0] + store[1];

    return result;
}

//-------------------------------------------------------------------------------------------
//Read DTC's

void Read_DTC(){
  ELM.send("03");
  Response = ELM.send("03");
  
  while(Response == "NO DATA" || Response == Last){
    Response = ELM.send("03");

    if(WiFi.status() != WL_CONNECTED){
            ESP.restart();
        }
  }

  Last = Response;
  
  Response.remove(0,10);
  Response.remove(8,3);
  Response.remove(22,3);
  Response.remove(24,21);

  int str_len = Response.length() + 1;

  char dtcr[str_len];

  Response.toCharArray(dtcr, str_len);
  
  for(int x = 0; x < str_len; x++){
    fcodes[x] = dtcr[x];
  }

  Response = Last;
}

//-------------------------------------------------------------------------------------------
//Convert DTC's

void Start_DTC(){
  String First[] = {"P0", "P1", "P2", "P3", "C0", "C1", "C2", "C3", "B0", "B1", "B2", "B3", "U0", "U1", "U2", "U3"};

  int Start[5];

  Start[0] = Hex_to_Dec(fcodes[0], 20);
  Start[1] = Hex_to_Dec(fcodes[4], 20);
  Start[2] = Hex_to_Dec(fcodes[8], 20);
  Start[3] = Hex_to_Dec(fcodes[12], 20);
  Start[4] = Hex_to_Dec(fcodes[16], 20);
  Start[5] = Hex_to_Dec(fcodes[20],20);

  CS[0] = First[Start[0]];
  Serial.println(CS[0]);
  CS[1] = First[Start[1]];
  Serial.println(CS[1]);
  CS[2] = First[Start[2]];
  Serial.println(CS[2]);
  CS[3] = First[Start[3]];
  Serial.println(CS[3]);
  CS[4] = First[Start[4]];
  Serial.println(CS[4]);
  CS[5] = First[Start[5]];
  Serial.println(CS[5]);
}

//-------------------------------------------------------------------------------------------
//String DTC's

void Add_DTC(){
  CS[0] += fcodes[1];
  CS[0] += fcodes[2];
  CS[0] += fcodes[3];
  
  CS[1] += fcodes[6];
  CS[1] += fcodes[7];
  CS[1] += fcodes[8];

  CS[2] += fcodes[11];
  CS[2] += fcodes[12];
  CS[2] += fcodes[13];

  CS[3] += fcodes[16];
  CS[3] += fcodes[17];
  CS[3] += fcodes[18];

  CS[4] += fcodes[21];
  CS[4] += fcodes[22];
  CS[4] += fcodes[23];

  CS[5] += fcodes[26];
  CS[5] += fcodes[27];
  CS[5] += fcodes[28];
}

//------------------------------------------------------------------------------
//Clear DTC's

void Clear_DTC(){
    
    while(DTC() != 0){
        looping = 1;
        ELM.send("04");

        if(WiFi.status() != WL_CONNECTED){
            ESP.restart();
        }
    }
}


//------------------------------------------------------------------------------
//Temperature Reading

int Temp(){

    if(looping == 0){
      Last = Response;
    }
    else{
      Last = "123";
    }

    Response = ELM.send("01 05");

    while(Response == "NO DATA" || Response == Last){
        Response = ELM.send("01 05");

        if(WiFi.status() != WL_CONNECTED){
            ESP.restart();
        }
    }

    Serial.print("Last Temp Response: ");
    Serial.println(Response);

    int STR_Len = Response.length() + 1;

    char tmp[STR_Len];

    Response.toCharArray(tmp, STR_Len);

    char value[] = {tmp[4],tmp[5]};

    int store[1];

    store[0] = Hex_to_Dec(value[0], 1);
    store[1] = Hex_to_Dec(value[1], 0);

    int result = store[0] + store[1] - 40;

    delay(20);

    return result;
}

//------------------------------------------------------------------------------
//RPM Reading

int RPM(){

    if(looping == 0){
      Last = Response;
    }
    else{
      Last = "123";
    }
  
    Response = ELM.send("01 0C");

    while(Response == "NO DATA" || Response == Last){
        Response = ELM.send("01 0C");

        if(WiFi.status() != WL_CONNECTED){
            ESP.restart();
        }
    }

    int STR_Len = Response.length() + 1;

    char tmp[STR_Len];

    Response.toCharArray(tmp, STR_Len);

    char value[] = {tmp[4],tmp[5],tmp[6],tmp[7]};

    int store[3];

    store[0] = Hex_to_Dec(value[0], 3);
    store[1] = Hex_to_Dec(value[1], 2);
    store[2] = Hex_to_Dec(value[2], 1);
    store[3] = Hex_to_Dec(value[3], 0);

    int result = (store[0] + store[1] + store[2] + store[3]) / 4;

    delay(20);
    
    return result;
}

//------------------------------------------------------------------------------
//Speed Reading

int Speed(){
    
    if(looping == 0){
      Last = Response;
    }
    else{
      Last = "123";
    }
    
    Response = ELM.send("01 0D");

    while(Response == "NO DATA" || Response == Last){
        Response = ELM.send("01 0D");

        if(WiFi.status() != WL_CONNECTED){
            ESP.restart();
        }
    }

    int STR_Len = Response.length() + 1;

    char tmp[STR_Len];

    Response.toCharArray(tmp, STR_Len);

    char value[] = {tmp[4],tmp[5]};

    int store[3];

    store[0] = Hex_to_Dec(value[0], 1);
    store[1] = Hex_to_Dec(value[1], 0);

    int result = (store[0] + store[1]) * 0.621;

    delay(20);

    return result;
}

//------------------------------------------------------------------------------
//Fuel Reading

int Fuel(){

    if(looping == 0){
      Last = Response;
    }
    else{
      Last = "123";
    }
    
    Response = ELM.send("01 2F");

    while(Response == "NO DATA" || Response == Last){
        Response = ELM.send("01 2F");

        if(WiFi.status() != WL_CONNECTED){
            ESP.restart();
        }
    }

    int STR_Len = Response.length() + 1;

    char tmp[STR_Len];

    Response.toCharArray(tmp, STR_Len);

    char value[] = {tmp[4],tmp[5]};

    int store[3];

    store[0] = Hex_to_Dec(value[0], 1);
    store[1] = Hex_to_Dec(value[1], 0);

    int result = (store[0] + store[1]) * 0.392;

    Serial.println(result);

    delay(20);

    return result;
}

//------------------------------------------------------------------------------
//Setup

void setup(){

    Serial.begin(38400);

    ELM.setDebug(true);

    // Init Display
    gfx->begin();

    #ifdef GFX_BL
      pinMode(GFX_BL, OUTPUT);
      digitalWrite(GFX_BL, HIGH);
    #endif

    if (!SPIFFS.begin())
    {
      Serial.println(F("ERROR: File System Mount Failed!"));
      gfx->println(F("ERROR: File System Mount Failed!"));
      exit(0);
    }

    gfx->fillScreen(BLACK);

    Display(GIF_Blink);
    Display(GIF_Blink);
    delay(50);

    //-----------------------------------------------------------------------------------------
    //Wi-Fi

    WiFi.mode(WIFI_AP);
    WiFi.begin(ssid);

    int y = 0;

    while (WiFi.status() != WL_CONNECTED)
    {
    
        delay(500);
        Serial.print(".");
        if(y == 6){
            ESP.restart();
        }
        y++;
    }

    Serial.println("");
    Serial.println("Connected to Wifi");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    if (client.connect(server, 35000))
        Serial.println("connected");
    else
    {
        Serial.println("connection failed");
        while(1);
    }
  
    //-----------------------------------------------------------------------------------------
    //OBD-II

    ELM.initialize();

    PID();
    PID2();

    for(int x = 0; x < 6; x++){
      Pid += PIDS[x];
    }

    Serial.println(Temp());
    Serial.println(Fuel());

    gfx->setTextColor(WHITE);
    gfx->setTextSize(2);
    gfx->setCursor(95, 30);
    gfx->println(Pid);

    delay(5000);

}

//------------------------------------------------------------------------------
//Main loop

void loop(){
  //-----------------------------------------------------------------------------------------
  //Happy
  
  if (PIDS[2] == 1 && PIDS[3] == 1){
    while(Speed() < 5 && RPM() > 2500){
      looping = 1;
      Display(GIF_Happy);    
    }
    looping = 0;
  }

  delay(20);

  Serial.println("Happy");

  //-----------------------------------------------------------------------------------------
  //Menu

  BS1 = digitalRead(BUTTON_PIN1);

  if(BS1 == 0)
  {
    Menu();
  }

  //-----------------------------------------------------------------------------------------
  //Blink

  Blinking = random(100);
  if(Blinking < 25)
  {
    Blinking = random(1,3);
    for(int bl = 0; bl < Blinking; bl++){
      Display(GIF_Blink);
    }
  }

  //-----------------------------------------------------------------------------------------
  //Cold

  if (PIDS[1] == 1){
    while(Temp() < 54){
      looping = 1;
      Display(GIF_Cold);    
    }
    looping = 0;
  }

  delay(20);

  //-----------------------------------------------------------------------------------------
  //Angry

  if (PIDS[2] == 1 && PIDS[3] == 1){
    while(Speed() > 5 && RPM() > 4000){
      looping = 1;
      Display(GIF_Angry);    
    }
    looping = 0;
  }

  delay(20);

  //-----------------------------------------------------------------------------------------
  //Menu

  BS1 = digitalRead(BUTTON_PIN1);

  if(BS1 == 0)
  {
    Menu();
  }

  //-----------------------------------------------------------------------------------------
  //Blink

  Blinking = random(100);
  if(Blinking < 25)
  {
    Blinking = random(1,3);
    for(int bl = 0; bl < Blinking; bl++){
      Display(GIF_Blink);
    }
  }

  //-----------------------------------------------------------------------------------------
  //Hot

  if (PIDS[1] == 1){
    while(Temp() > 108){
      looping = 1;
      Display(GIF_Melt);    
    }
    looping = 0;
  }

  delay(20);

  //-----------------------------------------------------------------------------------------
  //Menu

  BS1 = digitalRead(BUTTON_PIN1);

  if(BS1 == 0)
  {
    Menu();
  }

  //-----------------------------------------------------------------------------------------
  //Fuel

  if (PIDS[4] == 1){
    while(timer < 4){
      looping = 1;
      if(Fuel() < 30){
        Display(GIF_Hungry);
      }
      timer++;
    }
    looping = 0;    
  }

    delay(20);
    if(timer == 4){
      looping = 1;
    while(Fuel() < 5){
      Display(GIF_Hungry);
    }
    looping = 0;
    }

  delay(20);

  //-----------------------------------------------------------------------------------------
  //Blink

  Blinking = random(100);
  if(Blinking < 25)
  {
    Blinking = random(1,3);
    for(int bl = 0; bl < Blinking; bl++){
      Display(GIF_Blink);
    }
  }
  
  //-----------------------------------------------------------------------------------------
  //Look
  
  int look = random(100);
  if(look < 10)
  {
    Display(GIF_Look1);
    Display(GIF_Look2);
  }

  //-----------------------------------------------------------------------------------------
  //Menu

  BS1 = digitalRead(BUTTON_PIN1);

  if(BS1 == 0)
  {
    Menu();
  }

  //-----------------------------------------------------------------------------------------
  //Wifi Connection

  if(WiFi.status() != WL_CONNECTED){
    ESP.restart();
  }
  
}
