#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"
#include <SoftwareSerial.h>

// set the interval of gathering sensor values
const int INTERVALTIME = 5;
// set the filename of logger text file
const String FILENAME = "datalog";

const int chipSelect = 10;
const int MPU_addr = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

long artificalTime = 0;

RTC_DS1307 rtc;
SoftwareSerial BTserial(2, 3);

void setup() {
  // Open serial communications and wait for port to open:
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

  Serial.begin(9600);
  BTserial.begin(9600);

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }


  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  Serial.println("card, bluetooth, and rtc initialized.");
}

void loop() {
  DateTime now = rtc.now();
  DateTime future (now + TimeSpan(7,12,30,6));
  delay(INTERVALTIME);


  // make a string for assembling the data to log:
  String dataString = "";
  

  // read three sensors and append to the string:
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14); // request a total of 14 registers
  int16_t t = Wire.read();
  AcX = (t << 8) | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  t = Wire.read();
  AcY = (t << 8) | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  t = Wire.read();
  AcZ = (t << 8) | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  t = Wire.read();
  Tmp = (t << 8) | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  t = Wire.read();
  GyX = (t << 8) | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  t = Wire.read();
  GyY = (t << 8) | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  t = Wire.read();
  GyZ = (t << 8) | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  t = Wire.read();

  // add data to a variable to be stored on SD card
  dataString += INTERVALTIME;
  dataString += ",";
  dataString += future.hour();
  dataString += ",";
  dataString += future.minute();
  dataString += ",";
  dataString += future.second();
  dataString += ",";

  dataString += AcX;
  dataString += ",";
  BTserial.println(AcX);

  dataString += AcY;
  dataString += ",";
  BTserial.println(AcY);

  dataString += AcZ;
  dataString += ",";
  BTserial.println(AcZ);

  dataString += GyX;
  dataString += ",";
  dataString += GyY;
  dataString += ",";
  dataString += GyZ;



  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open(FILENAME + ".txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening " + FILENAME + ".txt");
  }
}