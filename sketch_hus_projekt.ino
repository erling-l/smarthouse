/*
 * ” Smarthus App” 
 * Studenten ska skapa en app som ska kommunicera med ett smarthus (Arduino ska med 
 * hjälp av olika komponenter simulera lampor, dörrar, alarm, mm på ett hus). 
 * 
 * För betyg 3:  Studenten bestämmer själv utseendet på Android appen.  Appen ska 
 * kunna göra följande på Arduinon: o Tända och släcka lampor i 3 rum i huset. o Appen 
 * ska visa temperatur, som simuleras av att använda potentiometer på Arduino. (Om 
 * studenten har en temperatur sensor går det bra att använda den istället)  Studenten 
 * ska hitta relevant vetenskaplig artikel. Dessa frågor ska bl a besvaras i rapporten: 
 * o Vad handlar din artikel om? o Diskutera vilken metod användes i artikeln, för att 
 * komma fram till resultatet. o Vad blev resultat? o På vilket sätt kopplas artikeln 
 * till projektuppgiften?
 * 
 * För betyg 4:  Alla krav för betyg 3 ska vara uppfyllda.  På Arduino, använd knappar 
 * för att simulera följande: o Fönster öppen/stängd o Larm på/av  Uppdatera Android 
 * appen med de olika signalvärden som man får från Arduino, dvs. visa i appen (med text) 
 * att fönster är öppen/stängd, eller att larm är på/av. 
 *
 * För betyg 5:  Alla krav för betyg 4 ska vara uppfyllda.  Istället för knappar på 
 * Android appen, ska rösten användas till att styra vad som ska hända i huset.  När 
 * du får tillbaka meddelande från Arduino, ska telefonen också vibrera.
 */
#include <OneWire.h>
// constants won't change. Used here to set a pin number :
const int windowPin1 =  6;     // the number of the window 1 pin
const int windowPin2 =  7;     // the number of the window 2 pin
const int windowPin3 =  8;     // the number of the window 3 pin
const int alarmPin =    9;     // the number of the Alarm pin
const int DS18B20Pin = 10;     // the number of the DS18B20 (a 4.7K resistor is necessary)
const int lampPin1 =   11;     // the number of the light in room 1 pin
const int lampPin2 =   12;     // the number of the light in room 2 pin
const int lampPin3 =   13;     // the number of the light in room 3 pin
const int roomTempPin2 =   1;     // the number of the temperature in room 2 pin
const int roomTempPin3 =   2;     // the number of the temperature in room 3 pin
const long interval1 = 100;           // interval at which to check state change(milliseconds)
const long interval2 = 1000;           // interval at which to upate temp reading (milliseconds)
const char lamp1on = '1';
const char lamp2on = '2';
const char lamp3on = '3';
const char alarmon = '4';
const char window1on = '5';
const char window2on = '6';
const char window3on = '7';
const char lamp1off = 'a';
const char lamp2off = 'b';
const char lamp3off = 'c';
const char alarmoff = 'd';
const char window1off = 'e';
const char window2off = 'f';
const char window3off = 'g';
const char androidStarted = 's';
// Variables will change :
int ledState1 = LOW;             // ledState used to set the LED
int ledState2 = LOW;             // ledState used to set the LED
char command = '0';
int alarmstate = alarmoff;
int window1state = LOW;
int window2state = LOW;
int window3state = LOW;
int state = LOW;
int roomRead = 0;
int roomtemp[3];
char result0[2] = "uu";
char result1[3] = "uuu";
char result2[4] = "truu";
char *commandstring[3];
 int room1temp = 0;
int room2temp = 0;
int room3temp = 0;
int cr = 13;
// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis1 = 0;        // will store last time LED was updated
unsigned long previousMillis2 = 0;        // will store last time LED was updated


OneWire  ds(DS18B20Pin);  // DS18B20 (a 4.7K resistor is necessary)

void setup() {
  // put your setup code here, to run once:
  pinMode(alarmPin, INPUT_PULLUP);
  pinMode(windowPin1, INPUT_PULLUP);
  pinMode(windowPin2, INPUT_PULLUP);
  pinMode(windowPin3, INPUT_PULLUP);
  pinMode(lampPin1, OUTPUT);
  pinMode(lampPin2, OUTPUT);
  pinMode(lampPin3, OUTPUT);
 digitalWrite(lampPin1, LOW);
  digitalWrite(lampPin2, LOW);
  digitalWrite(lampPin3, LOW);
  digitalWrite(alarmPin, HIGH);
  
  if (digitalRead(alarmPin) == LOW){
    alarmstate = alarmon;
  } else {
    alarmstate = alarmoff;
  }
  window1state = digitalRead(windowPin1);
  if (window1state == HIGH){
    window1state = LOW;
  } else {
    window1state = HIGH;
  }

  window2state = digitalRead(windowPin2);
  if (window2state == HIGH){
    window2state = LOW;
  } else {
    window2state = HIGH;
  }

  window3state = digitalRead(windowPin3);
  if (window3state == HIGH){
    window3state = LOW;
  } else {
    window3state = HIGH;
  }

  commandstring[0] =result0;
  commandstring[1] =result1;
  commandstring[2] =result2;
  Serial.begin(9600);
  Serial3.begin(9600);        // Connected to Bluetooth module HC-06 with a levelshifter 
                              // to convert 5v out from Arduino to 3,3 v in on HC-06
  if (Serial3.available()) {
     Serial3.read();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  // command interpreter
  if (Serial3.available()) {     // If anything comes in Seria3(pins 0 & 1)
    char cmd1 = Serial3.read();
        Serial.write(cmd1);
    switch (cmd1) { // read it and set command
      case lamp1on:
      
        command = lamp1on;
        break;
      case lamp2on:
        command = lamp2on;
        break;
      case lamp3on:
        command = lamp3on;
        break;
      case lamp1off:
        command = lamp1off;
        break;
      case lamp2off:
        command = lamp2off;
        break;
      case lamp3off:
        command = lamp3off;
        break;
      case androidStarted:
        command = androidStarted;
        break;
      default:
        ;
    }
  }
  // command interpreter
  switch (command) {
      case lamp1on:
        lampOn(lampPin1); 
        break;
      case lamp2on:
        lampOn(lampPin2); 
        break;
      case lamp3on:
        lampOn(lampPin3); 
        break;
      case lamp1off:
        lampOff(lampPin1); 
        break;
      case lamp2off:
        lampOff(lampPin2); 
        break;
      case lamp3off:
        lampOff(lampPin3); 
       break;
      case androidStarted:
        appStarted(); 
        command='0';
       break;
      default:
        ;

  }
//Check for changes in window or alarm state, as only one state change can be transmitted each time a state 
// change check is made each 100 ms  if no state change is detected one of the temperatures is transmitted 
stateCheck();
// Update temperature every second
readTemp();
}
void lampOn(int  pinNumber){
    digitalWrite(pinNumber, HIGH);
}
void lampOff(int pinNumber){
  digitalWrite(pinNumber, LOW);
}
void stateCheck(){
  unsigned long currentMillis1 = millis();
  char test;
  if (currentMillis1 - previousMillis1 >= interval1) {
    // save the last time you checked
    previousMillis1 = currentMillis1;
        Serial.write("sC");
//        Serial.print(digitalRead(windowPin1));

    // if the state has changed store the new state and notify the app
    if (alarmstate != digitalRead(alarmPin)) {
      alarmstate = digitalRead(alarmPin);
      if (alarmstate == HIGH) {
        test = alarmon;
        Serial.write(test);
        Serial3.write(test);
       Serial3.write(cr);
      } else{
        test = alarmoff;
        Serial.write(test);
        Serial3.write(test);
        Serial3.write(cr);
     }
    } else if ( window1state != digitalRead(windowPin1)) {
      window1state = digitalRead(windowPin1);
      if ( window1state == HIGH) {
        Serial.write(window1on);
        Serial3.write(window1on);
       Serial3.write(cr);
      } else{
        Serial.write(window1off);
        Serial3.write(window1off);
       Serial3.write(cr);
      }
    
    } else if ( window2state != digitalRead(windowPin2)) {
      window2state = digitalRead(windowPin2);
      if ( window2state == HIGH) {
        Serial.write(window2on);
        Serial3.write(window2on);
       Serial3.write(cr);
      } else{
        Serial.write(window2off);
        Serial3.write(window2off);
       Serial3.write(cr);
      }
    } else if ( window3state != digitalRead(windowPin3)) {
       window3state = digitalRead(windowPin3);
      if ( window3state == HIGH) {
        Serial.write(window3on);
        Serial3.write(window3on);
       Serial3.write(cr);
      } else{
        Serial.write(window3off);
        Serial3.write(window3off);
       Serial3.write(cr);
      }
    } else {
      // Read one temeperature and send it
      Serial3.write(roomtemp[roomRead]);
      Serial3.print(commandstring[roomRead]);
       Serial3.write(cr);
//      Serial.print(roomtemp[roomRead],HEX);
//      Serial.print(commandstring[roomRead]);
      roomRead = roomRead +1;
      if (roomRead == 3){
        roomRead =0; 
      }
    }
//            Serial.write(" Cs");
  }
}
void appStarted(){
      char test;
      alarmstate = digitalRead(alarmPin);
      if (alarmstate == HIGH) {
        test = alarmon;
      } else{
        test = alarmoff;
      }
      Serial.write(test);
      Serial3.write(test);
       Serial3.write(cr);
      delay(100);
     state = digitalRead(windowPin1);
      if (state == HIGH) {
        test = window1on;
      } else{
        test = window1off;
      }
      Serial.write(test);
      Serial3.write(test);
       Serial3.write(cr);
      delay(100);    
      state = digitalRead(windowPin2);
      if (state == HIGH) {
        test = window2on;
      } else{
        test = window2off;
      }
      Serial.write(test);
      Serial3.write(test);
      Serial3.write(cr);
      delay(100);
      state = digitalRead(windowPin3);
      if (state == HIGH) {
        test = window3on;
      } else{
        test = window3off;
      }
      Serial.write(test);
      Serial.write(state);
       Serial3.write(test);
      Serial3.write(cr);
      delay(100);
      state = digitalRead(lampPin1);
      if (state == HIGH) {
        test = lamp1on;
      } else{
        test = lamp1off;
      }
      Serial.write(test);
      Serial.write(state);
      Serial3.write(test);
      Serial3.write(cr);
      delay(100);
      state = digitalRead(lampPin2);
      if (state == HIGH) {
        test = lamp2on;
      } else{
        test = lamp2off;
      }
      Serial.write(test);
      Serial.write(state);
       Serial3.write(test);
       Serial3.write(cr);
      delay(100);
      state = digitalRead(lampPin3);
      if (state == HIGH) {
        test = lamp3on;
      } else{
        test = lamp3off;
      }
      Serial.write(test);
      Serial.write(state);
       Serial3.write(test);
       Serial3.write(cr);
      delay(100);
 }
      
void readTemp(){
  unsigned long currentMillis2 = millis();
  
  if (currentMillis2 - previousMillis2 >= interval2) {
    // save the last time temperature was read
    previousMillis2 = currentMillis2;

    // read temp
    roomtemp[0] = (int) temp();
    roomtemp[1] = readPot(roomTempPin2);
    roomtemp[2] = readPot(roomTempPin3);
//    Serial.print(roomtemp[1],HEX);
//    Serial.print(roomtemp[2],HEX);
  }
}
int readPot(int  pinNumber){
    int sensorValue = analogRead(pinNumber);
      int outputValue = map(sensorValue, 0, 1023, 0, 255);
      return outputValue;
}
/*
 * temp() function to read the temperature of DS18B20 OneWire termometre
 */
float temp(){
    byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;
  
  if ( !ds.search(addr)) {
//    Serial.println("No more addresses.");
//    Serial.println();
    ds.reset_search();
    delay(250);
    return;
  }
  
//  Serial.print("ROM =");
//  Serial1.print("ROM =");
  for( i = 0; i < 8; i++) {
//    Serial.write(' ');
//    Serial.print(addr[i], HEX);
  }

  if (OneWire::crc8(addr, 7) != addr[7]) {
//      Serial.println("CRC is not valid!");
      return;
  }
  Serial.println();
 
  // the first ROM byte indicates which chip
  switch (addr[0]) {
    case 0x10:
//      Serial.println("  Chip = DS18S20");  // or old DS1820
      type_s = 1;
      break;
    case 0x28:
//      Serial.println("  Chip = DS18B20");
      type_s = 0;
      break;
    case 0x22:
//      Serial.println("  Chip = DS1822");
      type_s = 0;
      break;
    default:
//      Serial.println("Device is not a DS18x20 family device.");
      return;
  } 

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  
  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

//  Serial.print("  Data = ");
//  Serial.print(present, HEX);
//  Serial.print(" ");
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
//    Serial.print(data[i], HEX);
//    Serial.print(" ");
  }
//  Serial.print(" CRC=");
//  Serial.print(OneWire::crc8(data, 8), HEX);
//  Serial.println();

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0;
//  Serial.print("  Temperature = ");
//  Serial.print(celsius);
//  Serial.print(" Celsius, ");
//  Serial.print(fahrenheit);
//  Serial.println(" Fahrenheit");
 return celsius;
}

