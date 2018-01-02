#include "Adafruit_FONA.h"

#define FONA_RX 9
#define FONA_TX 8
#define FONA_RST 4
#define FONA_RI 7

// declare the pattern which will be changed based on SMS state.
char pattern = "p1";

// large reply buffer
char replybuffer[255];

// We default to using software serial. If you want to use hardware serial
// (because softserial isnt supported) comment out the following three lines 
// and uncomment the HardwareSerial line
#include <SoftwareSerial.h>
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;

// Hardware serial is also possible!
//  HardwareSerial *fonaSerial = &Serial1;

// Use this for FONA 800 and 808s
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout = 0);
void flushSerial();

uint8_t type;

void setup() {
  // put your setup code here, to run once:
  while (!Serial);

  Serial.begin(115200);
  Serial.println(F("FONA basic test"));
  Serial.println(F("Initializing....(May take 3 seconds)"));

  fonaSerial->begin(4800);
  if (! fona.begin(*fonaSerial)) {
    Serial.println(F("Couldn't find FONA"));
    while (1);
  }
  type = fona.type();
  fona.setGPRSNetworkSettings(F("wireless.twilio.com"));

  pinMode(13, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  // flush input
//  flushSerial();
//  while (! Serial.available() ) {
//    if (fona.available()) {
//      Serial.write(fona.read());
//    }
//  }
//  flushSerial();
  checkMessages();
//  playPattern(pattern);
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  delay(500);
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  delay(500);
  
}


void checkMessages() {
    
    Serial.print(F("Read #"));
    uint8_t smsn = 1;
    Serial.print(F("\n\rReading SMS #")); Serial.println(smsn);
  
    // Retrieve SMS sender address/phone number.
    if (! fona.getSMSSender(smsn, replybuffer, 250)) {
      Serial.println("Failed!");

    }
    Serial.print(F("FROM: ")); Serial.println(replybuffer);
  
    // Retrieve SMS value.
    uint16_t smslen;
    if (! fona.readSMS(smsn, replybuffer, 250, &smslen)) { // pass in buffer and max len!
      Serial.println("Failed!");
  
    }
    Serial.print(F("***** SMS #")); Serial.print(smsn);
    Serial.print(" ("); Serial.print(smslen); Serial.println(F(") bytes *****"));
    Serial.println(replybuffer);
    Serial.println(F("*****"));

  

//    replybuffer.trim();

    if((replybuffer == "off") || (replybuffer == "on")) {
      pattern = replybuffer;
    } else if ((replybuffer == "p1") || (replybuffer == "p2")) {
      pattern = replybuffer;
    } else if (replybuffer == "p3") {
      pattern = replybuffer;
    } else if (replybuffer == "menu") {
      fona.sendSMS(smsn, "on, off, p1, p2, p3, menu");
    } else {
      fona.sendSMS(smsn, "Sorry, unknown command");
    }
}


void playPattern(char pattern) {
  switch (pattern) {
    case 'p1':
      flashSlow();
      break; 
    case 'p2':
      flashFast();
      break;
    default: 
      break;
  }
}


void flushSerial() {
  while (Serial.available())
    Serial.read();
}

char readBlocking() {
  while (!Serial.available());
  return Serial.read();
}

uint16_t readnumber() {
  uint16_t x = 0;
  char c;
  while (! isdigit(c = readBlocking())) {
    //Serial.print(c);
  }
  Serial.print(c);
  x = c - '0';
  while (isdigit(c = readBlocking())) {
    Serial.print(c);
    x *= 10;
    x += c - '0';
  }
  return x;
}

uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout) {
  uint16_t buffidx = 0;
  boolean timeoutvalid = true;
  if (timeout == 0) timeoutvalid = false;

  while (true) {
    if (buffidx > maxbuff) {
      //Serial.println(F("SPACE"));
      break;
    }

    while (Serial.available()) {
      char c =  Serial.read();

      //Serial.print(c, HEX); Serial.print("#"); Serial.println(c);

      if (c == '\r') continue;
      if (c == 0xA) {
        if (buffidx == 0)   // the first 0x0A is ignored
          continue;

        timeout = 0;         // the second 0x0A is the end of the line
        timeoutvalid = true;
        break;
      }
      buff[buffidx] = c;
      buffidx++;
    }

    if (timeoutvalid && timeout == 0) {
      Serial.println(F("TIMEOUT"));
      break;
    }
    delay(1);
  }
  buff[buffidx] = 0;  // null term
  return buffidx;
}


void flashFast() {
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  delay(500);
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  delay(500);
}

void flashSlow() {
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(500);
}


