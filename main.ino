//Sample using LiquidCrystal library
#include <LiquidCrystal.h>
#include <Wire.h> 
#include <SPI.h>
#include <SD.h>
#include <Adafruit_Fingerprint.h> 
#define mySerial Serial1 //fingerprint
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial); 

#include <DS3231.h>

// Init the DS3231 using the hardware interface
DS3231 rtc(SDA, SCL);
 
uint8_t id;
File myFile;
int Status = 0;
const int chipSelect = 53;
String Date;
String Time_in;
String Name = " ";
String FileName;

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
String STAFF_name;
// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
#define power 48
// read the buttons
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      // read the value from the sensor 
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 // For V1.1 us this threshold
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 250)  return btnUP; 
 if (adc_key_in < 450)  return btnDOWN; 
 if (adc_key_in < 650)  return btnLEFT; 
 if (adc_key_in < 850)  return btnSELECT;  

 return btnNONE;  // when all others fail, return this...
}



void setup()
{
  lcd.setCursor(0, 0);
 lcd.print("LOADING........");
  rtc.begin();
  Serial.begin(9600);
 pinMode(chipSelect,OUTPUT);
 pinMode(power, OUTPUT);
 lcd.begin(16, 2);              // start the library
 lcd.setCursor(0, 0);
 lcd.clear();
 lcd.print("AMAZING SYSTEMS");
 lcd.setCursor(0,1);
 lcd.print("08051230116");
 delay(3000);
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("STAFF ATTENDANCE"); // print a simple message
 lcd.clear();
   lcd.setCursor(0, 0);
 lcd.print("LOADING........");
 finger.begin(57600); 


 if (finger.verifyPassword()) {  
    Serial.println("Found fingerprint sensor!");  
  } else {  
    Serial.println("Did not find fingerprint sensor :(");
    lcd.clear();
      lcd.setCursor(0, 0);
 lcd.print("SENSOR ERROR........"); 
    while (1) { delay(1); }  
  }   


  Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
        lcd.clear();
      lcd.setCursor(0, 0);
 lcd.print("SD CARD ERROR........"); 
    while (1);
  }
  Serial.println("card initialized.");
  lcd.clear();
}


 
void loop()
{
  Status = 0;
  lcd.setCursor(0,0);
  
  lcd.print("STAFF ATTENDANCE");
  
//if the registration button has been pressed
  if (read_LCD_buttons()  == 3){
    delay(1500);
   if (read_LCD_buttons()  == 3) {

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Registration....");
    delay(2000);

    finger.getTemplateCount();  
    Serial.print("Sensor contains "); 
    Serial.print(finger.templateCount); 
    Serial.println(" templates");  
    id = finger.templateCount + 1;
    Serial.println(id); 
    getFingerprintEnroll();
   }
  

  //if the attendance button has been pressed  
  else if(read_LCD_buttons() == 5){

    uint8_t userId = -1;
        lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("Attendance....");


    Serial.println("Waiting for valid finger...");
    for(int i=0;i<1;i++)
    {
      Status = 0;
      lcd.clear();
      lcd.print("Place Finger");
      delay(2000);

      userId = getFingerprintIDez();
      if(userId >=1 && Status != 1)
      {
        lcd.clear();
        lcd.setCursor(3,0);
        lcd.print("Present");
        delay(2000);

        // Storing data in SD card
        //get_name(userid)

        
        String ID = String(userId) +"," + rtc.getDOWStr()+":"+rtc.getDateStr()+ ":" + rtc.getTimeStr() ;
        
          FileName = rtc.getDateStr();
  FileName.replace(".", "-");
  FileName.remove(5,5);
  FileName = FileName + ".csv";
  

        myFile = SD.open(FileName, FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to file...");
    myFile.print(" ");
    myFile.println(ID);
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening file");
  }
      }
    }
  }
}
}
        

// #################### Fingerprint Enroll Function ############################
uint8_t getFingerprintEnroll() {
 
  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #");
  Serial.println(id);
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("Place the"); 
  lcd.setCursor(5,1);
  lcd.print("finger "); 
  
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  Serial.println("Remove finger");
  lcd.clear();
  lcd.print(" Remove finger "); 
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("Place same"); 
  lcd.setCursor(3,1);
  lcd.print("finger again"); 
  
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
        // rx name
    Serial.println("Enter your name.");  
    while (Serial.available() == 0)   
    {  }  
    Name = Serial.readString(); 
    Serial.println(Name);
// save to list of STAFFs
  myFile = SD.open("staff.csv", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.println("Adding new STAFF");
    myFile.print(Name);
    myFile.println(","+String(id));
    }
    myFile.close();
    
    Serial.println("Stored!");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Stored!"); 
    lcd.setCursor(0,1);
    lcd.print("ID ");lcd.print(id);
    delay(3000);
    
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }
  lcd.clear();   
}

// ##################  Fingerprint Search   ###########################

int getFingerprintIDez() {
    
    uint8_t p = finger.getImage();
    if (p != FINGERPRINT_OK){
      lcd.clear();
      lcd.setCursor(3,0);
      lcd.print("No Finger ");
      lcd.setCursor(3,1);
      lcd.print("found");
      delay(2000);
      lcd.clear();
      Status = 1;
      return -1;  
    }
        

    p = finger.image2Tz();
    if (p != FINGERPRINT_OK){
      lcd.clear();
      lcd.setCursor(3,0);
      lcd.print("No Finger ");
      lcd.setCursor(3,1);
      lcd.print("found");
      delay(2000);      
      Status = 1;
      return -1;
    }

    p = finger.fingerFastSearch();
    if (p != FINGERPRINT_OK){
      Serial.println("Error Finger");
      lcd.clear();
      lcd.setCursor(3,0);
      lcd.print("No Finger ");
      lcd.setCursor(3,1);
      lcd.print("found");
      delay(2000);
      Status = 1;
      return -1;
      lcd.clear();
    }

   
 
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID; 
}
