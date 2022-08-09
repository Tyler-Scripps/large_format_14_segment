#include <DNSServer.h>
#include <WiFi.h>

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

// me leds are wired up as so
//
//      11
//  9_______12
//  |\  |10/|
// 8| \ | / |14
//  |  \|/  |
//  7——— ———13
//  |  /|\  |
// 1| / | \ |6
//  |/  |4 \| 
//  2———————5
//      3

// | 1 | 2 | 3 | 4 | 5 |  6  |  7  |  8  |  9  |  10 |  11 |  12 |  13 | 14 |  
//  0-1|2-3|4-5|6-7|8-9|10-11|12-12|13-14|15-16|17-18|19-20|21-22|23-23|24-25|


//convert order as wired to labels down below by making the position in the array correleta to the letter labels
// i.e. 0=a, 1=b, 2=c, etc. and the numers in the arrays the start and stop pixels in the led strip

//                                  a       b       c      d     e      f      g      h       i       j       k      l      m     n
const uint8_t segments[14][2] = {{20,21},{24,25},{11,12},{5,6},{0,1},{14,15},{2,2},{13,13},{16,17},{18,19},{22,23},{9,10},{7,8},{3,4}};


// the alphanumeric_segs array is using a clockwise labeling scheme

//      a
//  i_______k
//  |\  |j /|
// f| \ | / |b
//  |  \|/  |
//  g———————h
//  |  /|\  |
// e| / | \ |c
//  |/  |m \| 
//  n———————l
//      d

static const uint16_t alphanumeric_segs[96]{
//  nmlkjihgfedcba  
  0b00000000000000, // ' ' (space)
	0b00001000001000, // '!'
	0b00001000000010, // '"'
 	0b01001101001110, // '#'  BAD
	0b01001101101101, // '$'  BAD
	0b10010000100100, // '%'
	0b00110011011001, // '&'  BAD?
	0b00001000000000, // '''
	0b00000000111001, // '('
	0b00000000001111, // ')'
	0b11111111000000, // '*'
	0b01001011000000, // '+'
	0b10000000000000, // ','
	0b00000001000000, // '-'  BAD
	0b00000000000000, // '.'  needs to be adjusted for display without seperate period
	0b10010000000000, // '/'
	0b00000000111111, // '0'
	0b00010000000110, // '1'
	0b00000011011011, // '2'
	0b00000010001111, // '3'
	0b00000011100110, // '4'
	0b00000011101101, // '5'
	0b00000011111101, // '6'
	0b01010000000001, // '7'
	0b00000011111111, // '8'
	0b00000011100111, // '9'
	0b00000000000000, // ':'  needs to be adjusted for display without seperate period
	0b10001000000000, // ';'
	0b00110000000000, // '<'
	0b00000011001000, // '='
	0b10000100000000, // '>'
  0b01000010000011, // '?'
	0b00001100111011, // '@'  BAD
	0b00000011110111, // 'A'
	0b01001010001111, // 'B'
	0b00000000111001, // 'C'
	0b01001000001111, // 'D'
	0b00000011111001, // 'E'
	0b00000011110001, // 'F'
	0b00000010111101, // 'G'
	0b00000011110110, // 'H'
	0b01001000001001, // 'I'
	0b00000000011110, // 'J'
	0b00110001110000, // 'K'
	0b00000000111000, // 'L'
	0b00010100110110, // 'M'
	0b00100100110110, // 'N'
	0b00000000111111, // 'O'
	0b00000011110011, // 'P'
	0b00100000111111, // 'Q'
	0b00100011110011, // 'R'
	0b00000110001101, // 'S'
	0b01001000000001, // 'T'
	0b00000000111110, // 'U'
	0b10010000110000, // 'V'
	0b10100000110110, // 'W'
	0b10110100000000, // 'X'
	0b01010100000000, // 'Y'
	0b10010000001001, // 'Z'
	0b00000000111001, // '['
	0b00100010000000, // '\'
	0b00000000001111, // ']'
  0b10100000000000, // '^'
	0b00000000001000, // '_'
	0b00000010000000, // '`'
	0b00000011011111, // 'a'
	0b00100001111000, // 'b'
	0b00000011011000, // 'c'
	0b10000010001110, // 'd'
	0b00000001111001, // 'e'
	0b00000001110001, // 'f'
	0b00000110001111, // 'g'
	0b00000011110100, // 'h'
	0b01000000000000, // 'i'
	0b00000000001110, // 'j'
	0b01111000000000, // 'k'
	0b01001000000000, // 'l'
	0b01000011010100, // 'm'
	0b00100001010000, // 'n'
	0b00000011011100, // 'o'
	0b00010001110001, // 'p'
	0b00100011100011, // 'q'
	0b00000001010000, // 'r'
	0b00000110001101, // 's'
	0b00000001111000, // 't'
	0b00000000011100, // 'u'
	0b10000000010000, // 'v'
	0b10100000010100, // 'w'
	0b10110100000000, // 'x'
	0b00001010001110, // 'y'
	0b10010000001001, // 'z'
	0b10000011001001, // '{'
	0b01001000000000, // '|'
	0b00110100001001, // '}'
	0b00000101010010, // '~'
	0b11111111111111, // Unknown character (DEL or RUBOUT)
};

//wifi stuff
const char* hostname = "segClock";
const char* ssid = "ESPUI";
const char* password = "espui";

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
DNSServer dnsServer;

//fastled stuff
#define NUM_LEDS 26 //probably don't want this with mutliple digits
#define DATA_PIN 19
CRGB digit1[NUM_LEDS];

//display related variables
uint8_t globalRed = 50;
uint8_t globalGreen = 0;
uint8_t globalBlue = 0;

uint8_t tempChar = ' ';

// espui variables
int timeLabelId;
int timeId;


void setSegment(uint8_t segmentNum, CRGB *leds, uint8_t red, uint8_t green, uint8_t blue) {
  // Serial.print("setting segment: ");
  // Serial.print(segmentNum);
  // Serial.print(" to: ");
  // Serial.println(red);
  for(int i = segments[segmentNum][0]; i <= segments[segmentNum][1]; i++) {
    leds[i].setRGB(red, green, blue);
  }
}

void setSegments(uint16_t segmentsBits, CRGB *leds, uint8_t red, uint8_t green, uint8_t blue) {
  // Serial.println(segmentsBits, BIN);
  for (uint8_t i = 0; i < 14; i++) {
    if (bitRead(segmentsBits, i)) {
      setSegment(i, leds, red, green, blue);
    } else {
      setSegment(i, leds, 0, 0, 0);
    }
  }
}

void setDigit(uint8_t displayChar, CRGB *leds) {
  if (displayChar >= ' ' && displayChar <= '~') {
    setSegments(alphanumeric_segs[displayChar - ' '], leds, globalRed, globalGreen, globalBlue);
    // Serial.print("setting digit to: "); //debugging
    // Serial.print(displayChar);    //debugging
    // Serial.print(", index: ");
    // Serial.println(displayChar - ' ');
  }
}

// ESPUI callbacks
void timeCallback(Control *sender, int type) {
  if(type == TM_VALUE) { 
    Serial.println(sender->value);
  }
}

void timeButtonCallback(Control* sender, int type)
{
    switch (type)
    {
    case B_DOWN:
        // Serial.println("Button DOWN");
        break;

    case B_UP:
        Serial.println("Updating Time");
        ESPUI.updateTime(timeId);
        break;
    }
}

void setup() {
  ESPUI.setVerbosity(Verbosity::VerboseJSON);
  Serial.begin(115200);
  
  WiFi.setHostname(hostname);
  WiFi.begin(ssid, password);
  Serial.print("\n\nTry to connect to existing network");

  uint8_t timeout = 10;
  // Wait for connection, 5s timeout
  do
  {
    delay(500);
    Serial.print(".");
    timeout--;
  } while (timeout && WiFi.status() != WL_CONNECTED);
  
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("\n\nCreating hotspot");
    WiFi.mode(WIFI_AP);
    delay(100);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    
    uint32_t chipid = 0;
    for (int i = 0; i < 17; i = i + 8)
    {
      chipid |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
    }
  
    char ap_ssid[25];
    snprintf(ap_ssid, 26, "ESPUI-%08X", chipid);
    WiFi.softAP(ap_ssid);

    timeout = 5;
    do
    {
      delay(500);
      Serial.print(".");
      timeout--;
    } while (timeout);
  }

  dnsServer.start(DNS_PORT, "*", apIP);

  Serial.println("\n\nWiFi parameters:");
  Serial.print("Mode: ");
  Serial.println(WiFi.getMode() == WIFI_AP ? "Station" : "Client");
  Serial.print("IP address: ");
  Serial.println(WiFi.getMode() == WIFI_AP ? WiFi.softAPIP() : WiFi.localIP());

  //espui elements
  timeLabelId = ESPUI.label("Last Time Update:", ControlColor::Turquoise, "not updated");
  timeId = ESPUI.addControl(Time, "", "", None, 0, timeCallback);
  ESPUI.button("Update Time", &timeButtonCallback, ControlColor::Peterriver, "Press");
  ESPUI.begin("ESPUI Control");

  //initialize leds
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(digit1, NUM_LEDS);
}

void loop() {
  dnsServer.processNextRequest();

  
  if(Serial.available() > 0) {
    Serial.println(Serial.peek());
    if(Serial.peek() >= ' ' && Serial.peek() <= '~') {
      // Serial.println("good character");
      tempChar = Serial.read();
      Serial.print("tempChar: ");
      Serial.println(char(tempChar));
    } else {
      Serial.read();
      // Serial.println("bad character");
    }
  }
  
  setDigit(tempChar, digit1);
  FastLED.show();
}