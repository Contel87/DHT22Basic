#include <Wire.h>

#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2); //Digital pins used for the LCD (rs, enable, d4, d5, d6, d7)

int DHT22_data_pin = 8;
//int vcc_pin=8; //Only if use pin 8 like Vcc
//int gnd_pin=10; //Only if use pin 10 like Gnd
boolean result[41]; //holds the result from reading the DHT22
unsigned int databytes[5]; // result converted into 5 bytes
unsigned int checksum;
int interval=1000; // Delay between readings
float temp; //Temperature in degrees Celcius
float humidity; //Humidity in %RH

void setup(void) {

  //Supply power to DHT11 (Uses upto 2.5mA)
   // pinMode(gnd_pin, OUTPUT);
    //pinMode(vcc_pin, OUTPUT);
    //digitalWrite(gnd_pin,LOW);
   // digitalWrite(vcc_pin,HIGH);
  
  //Initialise LCD Display
    lcd.begin(16, 2);
    lcd.clear();
    lcd.print("Starting ...");
    delay(1000);
    lcd.clear();
 }

void loop(void) {

   //Pause before taking measurement
    delay(interval); 
   
  //Trigger reading by holding data pin low for 18ms
    pinMode(DHT22_data_pin, OUTPUT);
    digitalWrite(DHT22_data_pin,LOW);
    delay(18);
    digitalWrite(DHT22_data_pin,HIGH);
    pinMode(DHT22_data_pin, INPUT_PULLUP); 
  
  //read 41 bits of signal
    for(int i=0;i<=40;i++){
      result[i]=(pulseIn(DHT22_data_pin, HIGH)>40);
    }

  //Convert result to bytes
    for (int j=0;j<5;j++){
      databytes[j]=0;
      for (int i=1;i<=8;i++){
        databytes[j]=databytes[j]<<1;
        if (result[j*8+i]) databytes[j]|=1;
      }
    }

  //Calculate Checksum
    checksum=0;
    for (int j=0;j<4;j++) checksum+=databytes[j];
    checksum&=0x00FF; //ignore exverything except lowest 8 bits

  //Check Checksum 
    if (checksum!=databytes[4]){ //Checksum Not Valid
     lcd.setCursor(0, 1);
     lcd.print("Bad Checksum"); //Optional
    }else{ //Checksum Valid -> We can Extract and Update ...
      
      //Extract Temperature and Humidity 
      humidity=(databytes[0]*256+databytes[1])/(float)10;  //Extract Humidity 
      temp=((databytes[2]&0x7F)*256+databytes[3])/(float)10; //Extract Temperature 
      if ((databytes[2]&0x80)>0) temp=-temp;   //MSB of Temperature gives it sign
  
      //Write Result to Serial Communication (Optional)
      lcd.setCursor(0, 1);
       lcd.print(String(temp,1)+" C   ");
        lcd.print(String(humidity,1)+" %");
 }  
}