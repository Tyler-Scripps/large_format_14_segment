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

//                                  a       b       c      d     e      f       g       h       i       j       k      l     m     n
const uint8_t segments[14][2] = {{19,20},{24,25},{10,11},{4,5},{0,1},{13,14},{12,12},{23,23},{15,16},{17,18},{21,22},{8,9},{6,7},{2,3}};


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
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b00000000000000, //
  0b11111111111111 // fully lit character
};

//wifi stuff
const char* hostname = "segClock";
const char* ssid = "ESPUI";
const char* password = "espui";

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
DNSServer dnsServer;

uint8_t globalRed = 0;
uint8_t globalGreen = 0;
uint8_t globalBlue = 0;


void setSegment(uint8_t segmentNum, CRGB *leds, uint8_t red, uint8_t green, uint8_t blue) {
  for(int i = segments[segmentNum][0]; i < segments[segmentNum][1]; i++) {
    leds[i].setRGB(red, green, blue);
  }
}

void setSegments(uint16_t segmentsBits, CRGB *leds, uint8_t red, uint8_t green, uint8_t blue) {
  for (uint8_t i; i < 14; i++) {
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

  dnsServer.start(DNS_PORT, "*", apIP);

  Serial.println("\n\nWiFi parameters:");
  Serial.print("Mode: ");
  Serial.println(WiFi.getMode() == WIFI_AP ? "Station" : "Client");
  Serial.print("IP address: ");
  Serial.println(WiFi.getMode() == WIFI_AP ? WiFi.softAPIP() : WiFi.localIP());
}

void loop() {

}