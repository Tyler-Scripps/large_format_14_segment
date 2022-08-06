#include <ESPUI.h>
#include <dataControlsJS.h>
#include <dataGraphJS.h>
#include <dataIndexHTML.h>
#include <dataNormalizeCSS.h>
#include <dataSliderJS.h>
#include <dataStyleCSS.h>
#include <dataTabbedcontentJS.h>
#include <dataZeptoJS.h>

#include <FastLED.h>

// int segments[14][2] = {{0,4},{5,9},{10,14},{15,19},{20,24},{25,29},(30,34),{35,40},{41,44},{45,49},{50,54},{55,59},{60,64},{65,69}};
int segments[14][2] = {0,4,5,9,10,14,15,19,20,24,25,29,30,34,35,40,41,44,45,49,50,54,55,59,60,64,65,69};

uint8_t globalRed = 0;
uint8_t globalGreen = 0;
uint8_t globalBlue = 0;


void setSegment(uint8_t segmentNum, CRGB *leds, uint8_t red, uint8_t green, uint8_t blue) {
  for(int i = segments[segmentNum][0]; i < segments[segmentNum][1]; i++) {
    leds[i].setRGB(red, green, blue);
  }
}

void setDigit(char tempChar, CRGB leds) {

}

void setup() {

}

void loop() {

}