#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <QTRSensors.h>
#include <math.h>
QTRSensors qtr;
#include "HuskyLogo.h"

#define HUSKYLOGO_WIDTH 120
#define HUSKYLOGO_HEIGHT 90



const uint8_t SensorCount = 6;
uint16_t sensorValues[SensorCount];




//#define Line_Width 6
//sensor....................
#define Number_of_sensor 6
int sensor[Number_of_sensor];
int min_sensor[6]={20,20,20,20,20,20};
int max_sensor[6]={800,800,800,800,800,800};
// === LCD Setup ===
Adafruit_ST7789 tft = Adafruit_ST7789(PB12, PB10, PB15, PB13, PB11); // CS, DC, MOSI, SCLK, RST




//user interface.............................................................
// === Menu State ===
int menuIndex = 0;
const char* menuItems[] = {
  "Start",
  "Sensor calibration",
  "Show Line",
  "Speed",
  "Sensor View",
  "PID"
};
const int numItems = sizeof(menuItems) / sizeof(menuItems[0]);




// === Button Pins ===
#define UP    PB4
#define BACK  PB5
#define ENTER PB6
#define DOWN  PB7

// === Button Pins ===
#define BTN_UP    PB4
#define BTN_BACK  PB5
#define BTN_ENTER PB6
#define BTN_DOWN  PB7


// === PID Variables ===
float Kp = 0.5;
float Ki = 0.0;
float Kd = 0.2;
int lastError = 0;
int sumError = 0;
int pidIndex = 0;

// === PID ===
//int lastError;
//int Kp=0.2;
//int Kd=0.2;
int BaseSpeed=60;
void setup() {
  motor_initiate();
  // LCD init....
  tft.init(135, 240);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE);
  // button interupt
  attachInterrupt(digitalPinToInterrupt(UP), Menu, LOW);
  attachInterrupt(digitalPinToInterrupt(BACK), Menu, LOW);
  attachInterrupt(digitalPinToInterrupt(ENTER), Menu, LOW);
  attachInterrupt(digitalPinToInterrupt(DOWN), Menu, LOW);
  tft.fillScreen(ST77XX_BLACK);
  Menu();
  //sensor.....
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){PA0, PA1, PA2, PA3, PA4, PA5}, SensorCount);
  qtr.setEmitterPin(PB3);
  qtr.releaseEmitterPins();
}

void showLogo() {
  int x = (240 - HUSKYLOGO_WIDTH) / 2;
  int y = (135 - HUSKYLOGO_HEIGHT) / 2;
  tft.drawBitmap(x, y, HuskyLogoBitmap, HUSKYLOGO_WIDTH, HUSKYLOGO_HEIGHT, ST77XX_BLACK, ST77XX_YELLOW);
}


void loop() {
 

}

// === PID Tuning Menu ===
void tunePID() {
  while (true) {
    tft.fillScreen(ST77XX_BLACK);
    tft.setCursor(10, 30);
    tft.setTextColor(ST77XX_WHITE);
    tft.print("Tuning: ");
    switch (pidIndex) {
      case 0: tft.print("Kp"); break;
      case 1: tft.print("Ki"); break;
      case 2: tft.print("Kd"); break;
    }

    tft.setCursor(10, 60); tft.print("Kp: "); tft.print(Kp, 2);
    tft.setCursor(10, 80); tft.print("Ki: "); tft.print(Ki, 2);
    tft.setCursor(10, 100); tft.print("Kd: "); tft.print(Kd, 2);

    int btn = waitForButton();
    if (btn == BTN_UP) {
      pidIndex = (pidIndex - 1 + 3) % 3;
    } else if (btn == BTN_DOWN) {
      pidIndex = (pidIndex + 1) % 3;
    } else if (btn == BTN_ENTER) {
      switch (pidIndex) {
        case 0: Kp += 0.1; break;
        case 1: Ki += 0.01; break;
        case 2: Kd += 0.1; break;
      }
    } else if (btn == BTN_BACK) {
      Menu();
      return;
    }
  }
}

// === Button Wait Helper ===
int waitForButton() {
  while (true) {
    if (digitalRead(BTN_UP) == LOW) {
      while (digitalRead(BTN_UP) == LOW);
      return BTN_UP;
    }
    if (digitalRead(BTN_DOWN) == LOW) {
      while (digitalRead(BTN_DOWN) == LOW);
      return BTN_DOWN;
    }
    if (digitalRead(BTN_ENTER) == LOW) {
      while (digitalRead(BTN_ENTER) == LOW);
      return BTN_ENTER;
    }
    if (digitalRead(BTN_BACK) == LOW) {
      while (digitalRead(BTN_BACK) == LOW);
      return BTN_BACK;
    }
  }
}

// === Speed Adjustment Menu ===
void adjustSpeed() {
  while (true) {
    tft.fillScreen(ST77XX_BLACK);
    tft.setCursor(10, 50);
    tft.setTextColor(ST77XX_WHITE);
    tft.print("Speed: ");
    tft.print(BaseSpeed);


    int btn = waitForButton();
    if (btn == BTN_UP) {
      BaseSpeed = min(BaseSpeed + 10, 255);
    } else if (btn == BTN_DOWN) {
      BaseSpeed = max(BaseSpeed - 10, 180);
    } else if (btn == BTN_BACK) {
      Menu();
      return;
    }
  }
}


void read_sensor() //read the sensor
{
  int avg[6]={0,0,0,0,0,0};
  for(int i=0;i<20;i++)
  {
    avg[0]+=analogRead(PA0);
    avg[1]+=analogRead(PA1);
    avg[2]+=analogRead(PA2);
    avg[3]+=analogRead(PA3);
    avg[4]+=analogRead(PA4);
    avg[5]+=analogRead(PA5);
  }
   sensor[0]=map(avg[0]/20,min_sensor[0],max_sensor[0],0,100);
   sensor[1]=map(avg[1]/20,min_sensor[1],max_sensor[1],0,100);
   sensor[2]=map(avg[2]/20,min_sensor[2],max_sensor[2],0,100);
   sensor[3]=map(avg[3]/20,min_sensor[3],max_sensor[3],0,100);
   sensor[4]=map(avg[4]/20,min_sensor[4],max_sensor[4],0,100);
   sensor[5]=map(avg[5]/20,min_sensor[5],max_sensor[5],0,100);
  if (sensor[0] + sensor[1] + sensor[2] + sensor[3] + sensor[4] + sensor[5]< 250)
  {
   sensor[0]=map(sensor[0],0,100,100,0);
   sensor[1]=map(sensor[1],0,100,100,0);
   sensor[2]=map(sensor[2],0,100,100,0);
   sensor[3]=map(sensor[3],0,100,100,0);
   sensor[4]=map(sensor[4],0,100,100,0);
   sensor[5]=map(sensor[5],0,100,100,0);
  }
}
void display_sensor() // display the raw analog values for testing
{
  tft.fillScreen(ST77XX_BLACK);
  tft.setRotation(0);
  while(digitalRead(BACK)==HIGH)
  {
  read_sensor(); //reading the sensor
  //printing the sensor value realtime
  tft.setTextColor(ST77XX_YELLOW);
  for(int i=0;i<6;i++)
  {
  tft.setCursor(0, 30*i);
  tft.print("sesnor");
  tft.print(i);
  tft.print(" =");
  tft.print(sensor[i]);
  }
  // ERASING THE TEXT FROM THE SCREEN
  tft.setTextColor(ST77XX_BLACK);
  for(int i=0;i<6;i++)
  {
  tft.setCursor(0, 30*i);
  tft.print("sesnor");
  tft.print(i);
  tft.print(" =");
  tft.print(sensor[i]);
  }
  }
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);
  Menu();
}
void motor(int left, int right) //motor control funtion
{
  if (left==0 && right==0) // stoping the bot!!
  {
    analogWrite(PA8,0);
    analogWrite(PA9,0);
    analogWrite(PA11,0);
    analogWrite(PA10,0);
  }
  if(left>0)
  {
    analogWrite(PA8,left);
    analogWrite(PA9,0);
  }else
  {
    analogWrite(PA8,0);
    analogWrite(PA9,left*-1);//converting the negetive number to positive
  }
  if(right>0)
  {
    analogWrite(PA11,right);
    analogWrite(PA10,0);
  }else
  {
    analogWrite(PA11,0);
    analogWrite(PA10,right*-1);//converting the negetive number to positive
  }




}
void motor_initiate() // initialing the motors
{
  //setting the motor driver 1 and 2 pin to output
  pinMode(PA8, OUTPUT);  // motor A_1
  pinMode(PA9, OUTPUT);  // motor A_2
  pinMode(PA11, OUTPUT); // motor B_1
  pinMode(PA10, OUTPUT); // motor B_2
  // setting the motor drivers in Hi-Z
  digitalWrite(PA8,LOW);
  digitalWrite(PA9,LOW);
  digitalWrite(PA10,LOW);
  digitalWrite(PA11,LOW);




}
void calibration_sensor_manual()
{
  detachInterrupt(digitalPinToInterrupt(UP));
  detachInterrupt(digitalPinToInterrupt(DOWN));
  detachInterrupt(digitalPinToInterrupt(ENTER));
  detachInterrupt(digitalPinToInterrupt(BACK));
  tft.fillScreen(ST77XX_BLACK);
  motor(0,0);
  tft.setCursor(10, 20);
  tft.setTextColor(ST77XX_YELLOW);
  tft.print("<-move-left&right->");
  tft.setCursor(0, 40);
  tft.print("Press ENTER to START");
  tft.setCursor(0, 60);
  tft.print("Press BACK for MENU");
  while(digitalRead(ENTER)==HIGH && digitalRead(BACK)==HIGH){};
  if(digitalRead(ENTER)==LOW)
  {
    while(digitalRead(ENTER)==LOW){};
    for(int i=0;i<1000;i++)
    {
    //sensor-1
    if(analogRead(PA0)<min_sensor[0])min_sensor[0]=analogRead(PA0);
    else if(analogRead(PA0)>max_sensor[0] )max_sensor[0]=analogRead(PA0);
    //sensor-2
    if(analogRead(PA1)<min_sensor[1])min_sensor[1]=analogRead(PA1);
    else if(analogRead(PA1)>max_sensor[1])max_sensor[1]=analogRead(PA1);
    //sensor-3
    if(analogRead(PA2)<min_sensor[2])min_sensor[2]=analogRead(PA2);
    else if(analogRead(PA2)>max_sensor[2])max_sensor[2]=analogRead(PA2);
    //sensor-4
    if(analogRead(PA3)<min_sensor[3])min_sensor[3]=analogRead(PA3);
    else if(analogRead(PA3)>max_sensor[3])max_sensor[3]=analogRead(PA3);
    //sensor-5
    if(analogRead(PA4)<min_sensor[4])min_sensor[4]=analogRead(PA4);
    else if(analogRead(PA4)>max_sensor[4])max_sensor[4]=analogRead(PA4);
    //sensor-6
    if(analogRead(PA5)<min_sensor[5])min_sensor[5]=analogRead(PA5);
    else if(analogRead(PA5)>max_sensor[5])max_sensor[5]=analogRead(PA5);
    delay(10);
    }
  } else Menu();
  tft.fillScreen(ST77XX_BLACK);
  tft.setRotation(0);
  for(int i=0;i<6;i++)
  {
  tft.setCursor(0, 30*i);
  tft.print("m=");
  tft.print(min_sensor[i]);
  tft.print(",mx=");
  tft.print(max_sensor[i]);
  }
  tft.setCursor(0, 30*6);
  tft.print("press back for menu");
  while(digitalRead(BACK)==HIGH){};
  while(digitalRead(BACK)==LOW){};
  attachInterrupt(digitalPinToInterrupt(UP), Menu, LOW);
  attachInterrupt(digitalPinToInterrupt(BACK), Menu, LOW);
  attachInterrupt(digitalPinToInterrupt(ENTER), Menu, LOW);
  attachInterrupt(digitalPinToInterrupt(DOWN), Menu, LOW);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);
  Menu();
}
void Menu() {
  if(digitalRead(UP) == LOW || digitalRead(DOWN) == LOW || digitalRead(ENTER) == LOW || digitalRead(BACK) == LOW)
  {
     if (digitalRead(DOWN) == LOW) {
     tft.setCursor(0, menuIndex * 23);
     tft.setTextColor(ST77XX_BLACK);
     tft.print("> ");
     menuIndex++;
     if(menuIndex==6) menuIndex=0;
     tft.setCursor(0, menuIndex * 23);
     tft.setTextColor(ST77XX_YELLOW);
     tft.print("> ");
     while(digitalRead(DOWN) == LOW){};
     Menu();
     }

    if (digitalRead(UP) == LOW) {
     tft.setCursor(0, menuIndex * 23);
     tft.setTextColor(ST77XX_BLACK);
     tft.print("> ");
     menuIndex--;
     if(menuIndex<0) menuIndex=5;
     tft.setCursor(0, menuIndex * 23);
     tft.setTextColor(ST77XX_YELLOW);
     tft.print("> ");
     while(digitalRead(UP) == LOW){};
     Menu();
     }
  /*if (buttonPressed(DOWN)) {
    menuIndex = (menuIndex + 1) % numItems;
    drawMenu();
  }*/
    if (digitalRead(ENTER) == LOW) {
     while(digitalRead(ENTER) == LOW){};
     if(menuIndex==1) calibration_sensor_manual();
     if(menuIndex==4) display_sensor();
     if(menuIndex==2) display_pos();
     if(menuIndex==0) folowLine();
     if(menuIndex==5) tunePID();
     if(menuIndex==3) adjustSpeed();
     }

  }
  else
  {
   for (int i = 0; i < numItems; i++) {
     tft.setCursor(0, i * 23);
     if (i == menuIndex) {
       tft.setTextColor(ST77XX_YELLOW);
       tft.print("> ");
     } else {
       tft.setTextColor(ST77XX_WHITE);
       tft.print("  ");
     }
     tft.print(menuItems[i]);
   }
  }
}
float position()
{
    float whiteThresh = 85.0f;
    float blackThresh = 15.0f;
    read_sensor();
    // ---------- Exponential weighting ----------
   int N=6; float k = 0.05f; double wsum = 0.0, num = 0.0, total = 0.0, positions[6]={0, 1000, 2000, 3000, 4000, 5000};
   bool *lineDetected = nullptr;




   for (int i = 0; i < N; ++i) {
    double w = exp(- (double)k * (double)sensor[i]);
    num += w * (double)positions[i];
    wsum += w;
  }




   /*float avg = total / N;
   if (avg > whiteThresh || avg < blackThresh)  // lost or full-black condition
     return -2; // return invalid
   
   if (lineDetected) *lineDetected = true;
     return (float)(num / wsum);*/
    // if (wsum <= 1e-12) return NAN;
    //else
    return (float)(num / wsum);
     
    // return ir;
}
void folowLine()
{
  tft.fillScreen(ST77XX_BLACK);
  showLogo();
 
  while(digitalRead(BACK)==HIGH){
  int pos=position();
  /*while(sensor[0]<30  && digitalRead(BACK)==HIGH)
  {
    motor(-100,100); 
    read_sensor();
  }*/
  if (pos < 1900)
  {
    // We are far to the right of the line: turn left.
    // Set the right motor to 100 and the left motor to zero,
    // to do a sharp turn to the left.  Note that the maximum
    // value of either motor speed is 400, so we are driving
    // it at just about 25% of the max.
    motor(70, -20);




  }
  else if (pos < 3000)
  {
    // We are somewhat close to being centered on the line:
    // drive straight.
    motor(75, 75);




  }
  else
  {
    // We are far to the left of the line: turn right.
    motor(-20, 70);
  }
  }
  motor(0, 0);
  tft.fillScreen(ST77XX_BLACK);
}



void display_pos()
{
  tft.fillScreen(ST77XX_BLACK);
 
  while(digitalRead(BACK)==HIGH){
     float pos=position();
     if(pos<0)
     {
      tft.setCursor(0, 40);
      tft.setTextColor(ST77XX_YELLOW);
      tft.print("NULL");
     }
     else
     {
      tft.setCursor(0, 0);
      tft.setTextColor(ST77XX_YELLOW);
      tft.print(pos);
      tft.setCursor(0, 0);
      tft.setTextColor(ST77XX_BLACK);
      tft.print(pos);
    }
  }
}
