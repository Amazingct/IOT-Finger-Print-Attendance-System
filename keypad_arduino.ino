#include <Keypad.h>
#include <SoftwareSerial.h>
#include <String.h>
const byte ROWS = 4; 
const byte COLS = 4; 
int wait = 1100;

SoftwareSerial esp(2,3);

char hexaKeys[ROWS][COLS] = 
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', '+'}
};

byte rowPins[ROWS] = {9, 10, 11, 12};
byte colPins[COLS] =  {5, 6, 7, 8};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

String mul(char key)
{
  String com;
  if(key=='1') {com = "-,@"; return com;}
  else if(key=='2'){ com =  "ABC"; return com;}
  else if(key=='3') {com =  "DEF"; return com;}
  else if(key=='4') {com =  "GHI"; return com;}
  else if(key=='5'){ com =  "JKL"; return com;}
  else if(key=='6'){ com =  "MNO"; return com;}
  else if(key=='7'){ com =  "PQR"; return com;}
  else if(key=='8') {com =  "STU"; return com;}
  else if(key=='9'){ com =  "VWX"; return com;}
  else if(key=='0') {com =  " YZ"; return com;}
  else if(key=='A') {com =  "AAA"; return com;}
  else if(key=='B') {com =  "BBB"; return com;}
  else if(key=='C') {com =  "CCC"; return com;}
  else if(key=='+') {com =  "+++"; return com;}
  else if(key=='*') {com =  "***"; return com;}
  else if(key=='#') {com =  "###"; return com;}
  
}

char get_key()
{
    char c = '!';
    int pos = 0;
    char customKey = customKeypad.getKey();
    char tx[3];
    if (customKey)
      {
        c=customKey; 
        float ct= millis();
        while(ct+wait > millis())
        {
          char k = customKeypad.getKey();
          if(k)
          {
            if (k==customKey) 
            {
              mul(k).toCharArray(tx, 4);
              c = tx[pos];
              pos = pos+1;
              if (pos>3) pos=0;
              
            }
            
          }
        }
      }
      return c;
}

void setup()

{
  Serial.begin(9600);
  esp.begin(9600);
}
  
void loop()
{   

char rr = get_key();
if (rr != '!')
{
esp.print(rr);
}
}
