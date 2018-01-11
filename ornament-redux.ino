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

  // testing stuff here

  char thing1[8] = "hello";
  char thing2[8] = "hello";

  if(thing1 == thing2) {
    Serial.println(F("Things are teh same"));
  } else {
    Serial.println(F("Things are not the same"));
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  checkMessages(&pattern);
  delay(300);
  Serial.println(pattern);
  playPattern(pattern);
}


void checkMessages(char *pattern) {
    // get number of SMSs
    int8_t smsnum = fona.getNumSMS();
    uint16_t smslen;
    
    if (smsnum == 0) {
      Serial.println("No messages");
      
    } else {
      
      Serial.println("Some messages");
      
      for(int i = 1; i <= smsnum; i++) {
        // read the message(s)
        Serial.print(F("Reading message # ")); Serial.println(i);
         if (!fona.readSMS(i, replybuffer, 250, &smslen)) {  // pass in buffer and max len!
            Serial.println(F("Failed!"));
            break;
          } else {
            // print out the contents of the message
            Serial.println(replybuffer);
          }

          // This is what I was working on on 1/9. Trying to flip a switch on & off
          
//          if(replybuffer == off) {
//            Serial.println(F("Did this print statement ever get accessed?"));
//            pattern = 0;
//          } else if (replybuffer == on) {
//            Serial.println(F("This print statement is never getting accessed."));
//            pattern = 1;
//          } 
          // This is what was in the presentation.
//          string.trim(replybuffer);
//          switch(replybuffer) {
//            case 'p1': case 'on': case 'off':
//              pattern = replybuffer;
//              break;
//            default:
//              break;
//          }

          // print sender info
          if (! fona.getSMSSender(i, replybuffer, 250)) {
            Serial.println("Failed!");
            break;
          }
          Serial.print(F("FROM: ")); Serial.println(replybuffer);
          
          if (fona.deleteSMS(i)) {
            Serial.println(F("Deleted "));
          } else {
            Serial.println(F("Couldn't delete"));
          }
      }


      // Retrieve SMS sender address/phone number.
      Serial.print(F("FROM: ")); Serial.println(replybuffer);
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

void playPattern (int pattern) {
  Serial.print(F("Pattern number ")); Serial.println(pattern);
  switch(pattern) {
    case 1: 
      flashSlow();
      break;
    case 0:
      flashFast();
      break;
  }
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
  delay(300);
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  delay(300);
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  delay(300);
}

void flashSlow() {
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  delay(5000);
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  delay(5000);
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  delay(500);
}



