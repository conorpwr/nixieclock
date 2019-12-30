// Nixie Clock
// Conor Power

// Code to control the DS1302 came from https://github.com/msparks/arduino-ds1302

#include <SparkFunSX1509.h>
#include <Wire.h>
#include <DS1302.h>
#include <stdio.h>


// Assumes a default I2C address for the Sparkfun SX1509
const byte SX1509_ADDRESS = 0x3E;  // SX1509 I2C address (00)
SX1509 io; // Create an SX1509 object

// Set up structures for hours and minutes.
int hour = 00;
int hour_tens;
int hour_ones;

int minute = 00;
int minute_tens;
int minute_ones;

// Old code
/*
uint8_t currentValue = 0;
#define A 10
#define B 11
#define C 12
#define D 13
*/
// Define pin layout (left/right is physical location of the nixie for that element)
#define HOURLEFT_A 0
#define HOURLEFT_B 1
#define HOURLEFT_C 2
#define HOURLEFT_D 3

#define HOURRIGHT_A 4
#define HOURRIGHT_B 5
#define HOURRIGHT_C 6
#define HOURRIGHT_D 7

#define MINUTELEFT_A 8
#define MINUTELEFT_B 9
#define MINUTELEFT_C 10
#define MINUTELEFT_D 11

#define MINUTERIGHT_A 12
#define MINUTERIGHT_B 13
#define MINUTERIGHT_C 14
#define MINUTERIGHT_D 15

namespace {

// Set the appropriate digital I/O pin connections. These are the pin
// assignments for the Arduino as well for as the DS1302 chip. See the DS1302
// datasheet:
//
//   http://datasheets.maximintegrated.com/en/ds/DS1302.pdf
const int kCePin   = 5;  // Chip Enable
const int kIoPin   = 6;  // Input/Output
const int kSclkPin = 7;  // Serial Clock

// Create a DS1302 object.
DS1302 rtc(kCePin, kIoPin, kSclkPin);

String dayAsString(const Time::Day day) {
  switch (day) {
    case Time::kSunday: return "Sunday";
    case Time::kMonday: return "Monday";
    case Time::kTuesday: return "Tuesday";
    case Time::kWednesday: return "Wednesday";
    case Time::kThursday: return "Thursday";
    case Time::kFriday: return "Friday";
    case Time::kSaturday: return "Saturday";
  }
  return "(unknown day)";
}

void printTime() {
  // Get the current time and date from the chip.
  Time t = rtc.time();

  // Name the day of the week.
  const String day = dayAsString(t.day);

  // Format the time and date and insert into the temporary buffer.
  char buf[50];
  snprintf(buf, sizeof(buf), "%s %04d-%02d-%02d %02d:%02d:%02d",
           day.c_str(),
           t.yr, t.mon, t.date,
           t.hr, t.min, t.sec);

  // Print the formatted string to serial so we can see the time.
  Serial.println(buf);
}

}  // namespace


void setup() {
  // Old
  /*
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  //nixieWrite(A, B, C, D, 0);
  // End old
  */
  
  Serial.begin(9600);
  if(!Serial) {
    // shrug? I'm sure there is a better way of error checking here.
  }
  
  if (!io.begin(SX1509_ADDRESS))
  {
    Serial.println("Unable to contact SX1509");
    //while(1);;
  } else {
    // Set pins 0 - 15 to be analog output. If the cabling changes then this must change too.
    for (int i=0; i<16; i++) {
      io.pinMode(i, ANALOG_OUTPUT);
    }
  }

  /*
  rtc.writeProtect(false);
  rtc.halt(false);
  
  Time t(2019, 12, 12, 20, 47, 40, Time::kThursday);
  rtc.time(t);

  rtc.writeProtect(true);
  */
    
}

void loop() {
  // put your main code here, to run repeatedly:
  /*
  nixieWrite_1(currentValue);
  nixieWrite_2(currentValue);
  nixieWrite_3(currentValue);
  nixieWrite_4(currentValue);
  
  if(currentValue > 10) currentValue = 0;
  Serial.println(currentValue);

  currentValue++;
  */
  Time t = rtc.time();
 /* 
  hour_tens = t.min / 10;
  hour_ones = t.min % 10;
  nixieWrite(A, B, C, D, hour_tens);
  nixieWriteSX(A, B, C, D,hour_ones);
*/
  writeNixieTime(t.hr, t.min);
  Serial.println(t.hr);
  Serial.println(t.min);
  printTime();
  delay(1000);
}

void writeNixieTime(uint8_t hour, uint8_t minute){
  int hour_tens;
  int hour_ones;

  int minute_tens;
  int minute_ones;

  hour_tens = hour / 10;
  hour_ones = hour % 10;

  minute_tens = minute / 10;
  minute_ones = minute % 10;

  Serial.println(minute_tens);
  Serial.println(minute_ones);

  io.digitalWrite(HOURLEFT_D, (hour_tens & 0x08) >> 3);
  io.digitalWrite(HOURLEFT_C, (hour_tens & 0x04) >> 2);
  io.digitalWrite(HOURLEFT_B, (hour_tens & 0x02) >> 1);
  io.digitalWrite(HOURLEFT_A, hour_tens & 0x01);

  io.digitalWrite(HOURRIGHT_D, (hour_ones & 0x08) >> 3);
  io.digitalWrite(HOURRIGHT_C, (hour_ones & 0x04) >> 2);
  io.digitalWrite(HOURRIGHT_B, (hour_ones & 0x02) >> 1);
  io.digitalWrite(HOURRIGHT_A, hour_ones & 0x01);

  io.digitalWrite(MINUTELEFT_D, (minute_tens & 0x08) >> 3);
  io.digitalWrite(MINUTELEFT_C, (minute_tens & 0x04) >> 2);
  io.digitalWrite(MINUTELEFT_B, (minute_tens & 0x02) >> 1);
  io.digitalWrite(MINUTELEFT_A, minute_tens & 0x01);

  io.digitalWrite(MINUTERIGHT_D, (minute_ones & 0x08) >> 3);
  io.digitalWrite(MINUTERIGHT_C, (minute_ones & 0x04) >> 2);
  io.digitalWrite(MINUTERIGHT_B, (minute_ones & 0x02) >> 1);
  io.digitalWrite(MINUTERIGHT_A, minute_ones & 0x01);
  
}

void nixieWrite_1(uint8_t value){
  io.digitalWrite(HOURLEFT_D, (value & 0x08) >> 3);
  io.digitalWrite(HOURLEFT_C, (value & 0x04) >> 2);
  io.digitalWrite(HOURLEFT_B, (value & 0x02) >> 1);
  io.digitalWrite(HOURLEFT_A, value & 0x01);
}

void nixieWrite_2(uint8_t value){
  io.digitalWrite(HOURRIGHT_D, (value & 0x08) >> 3);
  io.digitalWrite(HOURRIGHT_C, (value & 0x04) >> 2);
  io.digitalWrite(HOURRIGHT_B, (value & 0x02) >> 1);
  io.digitalWrite(HOURRIGHT_A, value & 0x01);
}

void nixieWrite_3(uint8_t value){
  io.digitalWrite(MINUTELEFT_D, (value & 0x08) >> 3);
  io.digitalWrite(MINUTELEFT_C, (value & 0x04) >> 2);
  io.digitalWrite(MINUTELEFT_B, (value & 0x02) >> 1);
  io.digitalWrite(MINUTELEFT_A, value & 0x01);
}

void nixieWrite_4(uint8_t value){
  io.digitalWrite(MINUTERIGHT_D, (value & 0x08) >> 3);
  io.digitalWrite(MINUTERIGHT_C, (value & 0x04) >> 2);
  io.digitalWrite(MINUTERIGHT_B, (value & 0x02) >> 1);
  io.digitalWrite(MINUTERIGHT_A, value & 0x01);
}
