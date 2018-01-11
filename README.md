# Holiday Hardware Hack Ornament 
by Kat Reinhart

This is my version of the project presented by [Margaret Staples](https://www.twilio.com/blog/2017/07/introducing-twilio-developer-evangelist-margaret-staples.html) at the Women Who Code Seattle December 2017 Holiday Hardware Hack.  

This is my first foray into the world of IoT development and the C++ learning/re-learning curve has been steep but fun. 

### Hardware
* Adafruit Feather FONA 32u4 + SIM card 
* Neopixel 16px Ring (not implemented yet)
* Single red or blue LED, resistor, jumper cables, etc. 
* Photo to come soon! 

### Work in Progress
This is very much a work in progress. I will update this README as I make progress on it! 

### Goal 
The goal is to have an IoT device which can receive SMS messages and play different patterns on the Neopixel ring based on the message content. 

### Challenges 
I am having trouble with detecting equality between a char[255] and char[] types. 