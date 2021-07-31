#include <Keypad.h>
#include <SoftwareSerial.h>
const byte ROWS = 4; 
const byte COLS = 4; 

SoftwareSerial esp(2,3);

char hexaKeys[ROWS][COLS] = 
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 10, 11, 12};
byte colPins[COLS] =  {5, 6, 7, 8};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  Serial.begin(9600);
  esp.begin(9600);
}
  
void loop(){
  char customKey = customKeypad.getKey();
  
  if (customKey){
    Serial.println(customKey);
    esp.print(customKey);
  }
}
