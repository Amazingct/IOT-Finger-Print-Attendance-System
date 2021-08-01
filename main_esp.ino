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


void Display(String message, int ms=700)
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
    Display("SYSTEM CHECK........",2000);
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

  String command = keyboardHome();
  if (command == "C") // add attendance
  {
    int p = -1;
    while (p != FINGERPRINT_OK) 
    {
      p = finger.getImage();
      switch (p) 
      {
        case FINGERPRINT_OK:
          Display("Image taken");
          break;
        case FINGERPRINT_NOFINGER:
          Display("PLACE FINGER");
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
     int id = checkPrint();
     if (id != -1) 
     {
      String std = id2name(id);
      if (std !="Nill")
      {
        Display("WELCOME " + std, 3000);
        add2attd(std);
      }
      else
      {
        Display("TRY AGAIN",1000);
      }
     }
  }
  
  else if (command == "A") // add new student
  {
    finger.getTemplateCount();
    id = finger.templateCount + 1;
    Enroll(id);
    String names = keyboardRead("ENTER NAME:");
    addStudent(names,id);
  }

  else if (command == "B")
  {
    if (keyboardRead("DELETE ALL PRINTS?") == "Y") 
    {
      finger.emptyDatabase();
      Display("DONE!",2000);
    }
    
  }
  delay(50);
  
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
String keyboardHome() 
//just like input function in python, if parameter is "-" dont print anything to lcd at all, else print parameter to first row and inputed data in snd row
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("A - ADD NEW STUDENT ");
  lcd.setCursor(0,1);
  lcd.print("C - ADD ATTENDANCE  ");
  lcd.setCursor(0,2);
  lcd.print("B - CLEAR DATABASE  ");
  lcd.setCursor(0,3);
  lcd.print("                    ");
  
  char all_rx[40] = { '\0' }; // to avoid rubbish characters getting in
  int c = 0;
  int a = 0;
  int r = 3;

  
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
  else if (id==2) return "Femi";
  
  else 
  {
    Display( "SOMETHING WENT WRONG OR UNKNOWN PRINT", 3000);
    return ("Nill");
  }
  
  
 
}

void add2attd(String name)
{
  Display("ADDED STUDENT TO   ATTEDANCE", 3000);
}

void addStudent(String name, int id)
{
  Display("ADDED", 3000);
}


uint8_t checkPrint() 
{
    
    uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Display("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Display("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Display("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Display("Imaging error");
      return p;
    default:
      Display("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
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
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Display("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Display("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Display("Did not find a match");
    return p;
  } else {
    Display("Unknown error");
    return p;
  }

  // found a match!
  Display("FOUND PRINT", 2000);
  return finger.fingerID;
}


int Enroll(int id)
{
  int p = -1;
  Display("PLACE FINGER", 500);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Display("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Display("PLACE FINGER");
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

  Display("Remove finger",2000);

  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  
  p = -1;
  Display("Place  finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Display("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Display("place finger again",500);
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


  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Display("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Display("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Display("Fingerprints did not match");
    return p;
  } else {
    Display("Unknown error");
    return p;
  }


  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Display("Stored!", 3000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Display("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Display("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Display("Error writing to flash");
    return p;
  } else {
    Display("Unknown error");
    return p;
  }

  return true;
}

