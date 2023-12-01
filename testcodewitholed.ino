//0-Cap, 1-Res10k, 2 - res45k, 3-Voltage, 4-DHT
int obs = 1;
int nbs = 1;
int fl=0;
//oled setups
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_ADDR   0x3C
Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT);
//capacitance setups
#define analogPin A1          
#define chargePin 12       
#define dischargePin 11        
#define resistorValue 10000.0F  
unsigned long startTime;
unsigned long elapsedTime;
float microFarads;                
float nanoFarads;
//resistance setups
int analogPinres = A0;
int raw = 0;
int Vin = 5;
float Vout = 0;
float R1 = 10000;
float R2 = 0;
float buffer = 0;
//voltage setup
float voltage=0.0;
int vpin = A2;
//dht
#include <dht11.h>
#define DHT11PIN 3
dht11 DHT11;
void setup(){
//pushbuttons
Serial.begin(9600);
pinMode(2,INPUT_PULLUP);
//oled
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay(); 
//capacitance
  pinMode(chargePin, OUTPUT);     
  digitalWrite(chargePin, LOW);  
}
void disp(String mode, float val, String unit){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(20, 0);
  display.println(mode);
  display.drawLine(0, 10, 128, 10, WHITE);
  display.setTextSize(1);
  display.setCursor(10, 27);
  display.setTextSize(2);
  display.print(val);
  display.println(unit);
  display.display(); 
      delay(50);
}
void r1(){
    R1 = 1000;
    raw = analogRead(analogPinres);
  if(raw){
    buffer = raw * Vin;
    Vout = (buffer)/1024.0;
    buffer = (Vin/Vout) - 1;
    R2= R1 * buffer;
  if (R2<12000){disp("100-7500 ohm",R2,"ohm");}
  else{disp("100-7500 ohm",0.0,"null");}
    delay(50);
}}
void r10(){

    R1 = 10000;
    raw = analogRead(analogPinres);
  if(raw){
    buffer = raw * Vin;
    Vout = (buffer)/1024.0;
    buffer = (Vin/Vout) - 1;
    R2= R1 * buffer;
  if (R2<30000){disp("7500-20k ohm",R2," ohm");}
  else{disp("7500-20k ohm",0.0,"null");}
    delay(50);
}}
void r45(){
    R1 = 45000;
    raw = analogRead(analogPinres);
  if(raw){
    buffer = raw * Vin;
    Vout = (buffer)/1024.0;
    buffer = (Vin/Vout) - 1;
    R2= R1 * buffer;
  if (R2<200000 && R2>20000){disp("20k-150k ohm",R2," ohm");}
  else{disp("20k-150k ohm",0.0,"null");}
    delay(50);
}
}
void cap(){
  delay(50); 
  digitalWrite(chargePin, HIGH);  
  startTime = millis();
  while(analogRead(analogPin) < 648){}

  elapsedTime= millis() - startTime;
  microFarads = ((float)elapsedTime / resistorValue) * 1000;   

  if (microFarads > 1){
      // Serial.print((long)microFarads);       
      // Serial.println(" microFarads");   
      if(microFarads>0){disp("Capacitance",microFarads," uF"); }     
  }

  else{
    nanoFarads = microFarads * 1000.0;      
    // Serial.print((long)nanoFarads);         
    // Serial.println(" nanoFarads");
    if(nanoFarads>10){          
    disp("Capacitance",nanoFarads," nF");
    }
  }

  digitalWrite(chargePin, LOW);            
  pinMode(dischargePin, OUTPUT);            
  digitalWrite(dischargePin, LOW);          
  while(analogRead(analogPin) > 0){}

  pinMode(dischargePin, INPUT);


}
void volt(){
voltage = analogRead(vpin) * (5.0/1023) * ((55750.0)/9750);
disp("Voltage",voltage," V");
}
void temp(){
    
  int signal = DHT11.read(DHT11PIN);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(20, 0);
  
  display.println("DHT");
  display.drawLine(0, 10, 128, 10, WHITE);
  display.setTextSize(1);
  
 
  display.setCursor(7, 27);
  display.setTextSize(2);
  display.print((float)DHT11.temperature);
  
  display.println(" C");
    display.print((float)DHT11.humidity);
  
  display.println(" %");
  display.display();
    
  delay(50);
  
}

void loop(){
  nbs = digitalRead(2);
  Serial.println(nbs);
    if(nbs==0 && obs ==1){
    if(fl!=5){fl++;}
    else{fl=0;}
    }

switch(fl){
  case 0: Serial.println("Resistance 1k");r1();break;
  case 1:Serial.println("Resistance 10k");r10();break;
  case 2:Serial.println("Resistance 45k");r45();break;
  case 3:Serial.println("Voltage");volt();break;
  case 4:Serial.println("Capacitance"); cap(); break;
  case 5:Serial.println("DHT");temp();break;
  default: Serial.println("Invalid");

}
obs=nbs;
Serial.println(fl);

}