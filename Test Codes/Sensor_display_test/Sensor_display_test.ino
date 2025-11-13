#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>


#define Line_Width 6
#define Number_of_sensor 6
int sensor[Number_of_sensor];
// === LCD Setup ===
Adafruit_ST7789 tft = Adafruit_ST7789(PB12, PB10, PB15, PB13, PB11); // CS, DC, MOSI, SCLK, RST


// === Button Pins ===
#define BTN_UP    PB4
#define BTN_BACK  PB5
#define BTN_ENTER PB6
#define BTN_DOWN  PB7

void setup() {
   // Define SPI settings for another device (e.g., 10MHz, MSBFIRST, Mode 0)
  // LCD init
  tft.init(135, 240);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);
  //tft.setTextWrap(false);
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE);

}

void loop() {
 display_sensor();
}

void read_sensor() //read the sensor 
{
  sensor[0]=analogRead(PA0);
  sensor[1]=analogRead(PA1);
  sensor[2]=analogRead(PA2);
  sensor[3]=analogRead(PA3);
  sensor[4]=analogRead(PA4);
  sensor[5]=analogRead(PA5);
}
void display_sensor() // display the raw analog values for testing
{
  read_sensor();
  tft.setCursor(0, 0);
  tft.setTextColor(ST77XX_RED);
  tft.print(sensor[0]);
  tft.print("|");
  tft.print(sensor[1]);
  tft.print("|");
  tft.print(sensor[2]);
  tft.print("|");
  tft.print(sensor[3]);
  
  // ERASING THE TEXT FROM THE SCREEN
  tft.setCursor(0, 0);
  tft.setTextColor(ST77XX_BLACK);
  tft.print(sensor[0]);
  tft.print("|");
  tft.print(sensor[1]);
  tft.print("|");
  tft.print(sensor[2]);
  tft.print("|");
  tft.print(sensor[3]);



}