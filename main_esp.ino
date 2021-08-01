#include <LiquidCrystal_I2C.h>
int lcdColumns = 20;
int lcdRows = 4;
#include <Adafruit_Fingerprint.h>
#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
SoftwareSerial mySerial(12, 14);
#else
// On Leonardo/M0/etc, others with hardware serial, use hardware serial!
// #0 is green wire, #1 is white
#define mySerial Serial1
#endif
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
uint8_t id;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);


void Display(String message, int ms=0)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(message);
  delay(ms);
  Serial.println(message);
}

void setup()
{
  lcd.init();               
  lcd.backlight();
  Serial.begin(9600);
  delay(100);
  finger.begin(57600);
  if (finger.verifyPassword()) {
    Display("SYSTEM CHECK........",3000);
    Display("Found fingerprint sensor!", 3000);
  } else {
    Display("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }
lcd.clear();
}

String KeyboardRead(String message){}


///////////////////////////////////////
void loop()                     
{

Display(keyboardRead("ENTER NAME ->"), 3000);
//  lcd.setCursor(0,0);
//  lcd.print("PRESS A TO ADD NEW STUDENTl");
 // lcd.setCursor(3,0);
 // lcd.print("PRESS C TO CAPTURE ATTENDANCE");
  //String rx = KeyboardRead("-");

//  if (rx == "A")
//  {
//   int news =  Enroll();
//   if (news != 0)
//   {
//    String nameS = KeyboardRead("ENTER YOUR NAME:");
//    addStudent(nameS, news);// add to list of student
//   }
//  }
//  
//  else if (rx == "C")
//  {
//   uint8_t ids =  capture_attendance();
//   String std = id2name(ids);
//   if (std != "#")
//   {
//    Display("WELCOME " + std, 3000);
//    add2attd(std);
//   }
//
//  }
//  delay(50);
  
}
///////////////////////////////////////////////////////////////////////

String keyboardRead(String message) 
//just like input function in python, if parameter is "-" dont print anything to lcd at all, else print parameter to first row and inputed data in snd row
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(message);
  lcd.setCursor(0,2);
  lcd.print("                    ");
  lcd.setCursor(0,3);
  lcd.print("                    ");
  
  char all_rx[40] = { '\0' }; // to avoid rubbish characters getting in
  int c = 0;
  int a = 0;
  int r = 2;

  
  while(a<39)
  {
    if (Serial.available()>0)
    {
   
      char rx = Serial.read();
      if (rx == '#') return all_rx;
      else if (rx == '+')
      {
        c=c-1;
        a=a-1;
        lcd.setCursor(c,r);
        lcd.print(" ");
        //update all_rx
        all_rx[a]=' ';
      }
      else if (rx == 'A' || rx == 'B'|| rx == 'C'|| rx == 'D' ||rx == 'E'|| rx == 'F'||
      rx == 'G'|| rx == 'H'|| rx == 'I'|| rx == 'J'|| rx == 'K'|| rx == 'L'|| rx == 'M'||
      rx == 'N'|| rx == 'O'|| rx == 'P'|| rx == 'Q'|| rx == 'R'|| rx == 'S'|| rx == 'T'||
      rx == 'U'|| rx == 'V'|| rx == 'W'|| rx == 'X'|| rx == 'Y'|| rx == 'Z'|| rx == ','||
      rx == '-'|| rx == '@'|| rx == '*'|| rx == '0'|| rx == '1'|| rx == '2'|| rx == '3'|| 
      rx == '4'|| rx == '5'|| rx == '6'|| rx == '7'|| rx == '8'|| rx == '9'||rx == ' ' )
      {
        all_rx[a] = rx;
        lcd.setCursor(c,r);
        lcd.print(rx);
        a = a + 1;
        c = c + 1;
        if(c >= 20)
        {
          //move cursor to lower row
          r = 3;
          c = 0;
        }
        
      }
    }
  }
  return String(all_rx);
}

uint8_t capture_attendance()
{

  Display("PLACE FINGER ON SENSOR", 1500);
    uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      //Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      //Serial.println("Imaging error");
      return p;
    default:
      //Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      //Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      //Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      //Serial.println("Could not find fingerprint features");
      return p;
    default:
      //Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    //Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    //Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    //Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);

  return finger.fingerID;
}

String id2name(int id)
{
  if (id==1) return "Daniel";
  else 
  {
    Display( "SOMETHING WENT WRONG OR UNKNOWN PRINT", 3000);
    return ("#");
  }
  
  
 
}

void add2attd(String name)
{
  Serial.println("ADDED STUDENT TO ATTD");
}

void addStudent(String name, int id)
{
  Serial.println("ADDED NEW STUDENT");
}

int Enroll()
{
  
  Display("Ready to enroll a fingerprint!", 2000);
  Display("Please type in the ID # (from 3 to 127) you want to save this finger as...");
  id = readnumber();
  if (id == 0 or id == 2) 
  {
    Display(" ID NOT ALLOWED! TRY AGAIN ", 2500);
     return 0;
  }
  Display("Enrolling ID #", 2000);
  while (!  getFingerprintEnroll() );
  return id;
}

uint8_t readnumber(void) 
{
  uint8_t num = 0;

  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}


uint8_t getFingerprintEnroll() 
{
  int p = -1;
  Display("PLACE FINGER ON SENSOR" ); 
  while (p != FINGERPRINT_OK) 
  {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Display("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Display("PLACE FINGER ON SENSOR", 800 ); 
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Display("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Display("Imaging error");
      break;
    default:
      Display("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Display("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Display("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Display("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Display("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Display("Could not find fingerprint features");
      return p;
    default:
      Display("Unknown error");
      return p;
  }

  Display("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Display(String(id));
  p = -1;
  Display("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Display("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Display("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Display("Imaging error");
      break;
    default:
      Display("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Display("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Display("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Display("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Display("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Display("Could not find fingerprint features");
      return p;
    default:
      Display("Unknown error");
      return p;
  }

  // OK converted!
  Display("Creating model...", 2000);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Display("Prints matched!", 2000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Display("Communication error", 2000);
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Display("Fingerprints did not match", 2000);
    return p;
  } else {
    Display("Unknown error", 3000);
    return p;
  }

 Display("ID: " + String(id), 1500);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Display("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Display("Communication error", 3000);
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Display("Could not store in that location", 3000);
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Display("Error writing to flash", 3000);
    return p;
  } else {
    Display("Unknown error", 3000);
    return p;
  }

  return true;
}
