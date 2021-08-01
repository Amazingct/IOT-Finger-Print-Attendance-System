# IOT-Finger-Print-Attendance-System
FingerPrint Attendance systems with LCD

The Devices makes use of the following hardware:
- ESP8266 (node mcu v0.1)
- Arduino nano
- Fingerprint sensor
- 2004 LCD with i2c interface
- 4 by 4 arduino keyboard

Arduino captures key presses and send to ESP, upload arduino.ino to arduino and esp_mai.ino to esp.
Ensure you use only the i2c lcd library provided in this repo.

Connection:
- Arduino d2 = esp rx (disconnect while uploading code to esp)
- arduino d3 = esp tx

- fingerprint sensor rx = esp d5
- fingerprint sensor tx =  esp d6

- lcd sda = esp d1
- lcd scl = esp d2
