#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN    6
int cds = A0;
int LED = 9;

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 60

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int SensorPin = A0; //analog pin 0int SensorPin = A0; //analog pin 0

// setup() function -- runs once at startup --------------------------------

void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.
  Serial.begin(9600);
  pinMode(cds, INPUT);
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
}


// loop() function -- runs repeatedly as long as board is on ---------------

void loop() {
  // Fill along the length of the strip in various colors...

  int SensorReading = analogRead(SensorPin);

  int mfsr_r18 = map(SensorReading, 0, 1024, 0, 255);
  //  Serial.println(mfsr_r18);

  cds = analogRead(A1); // A0로 받은 아날로그 값을 cds변수로 넘겨줍니다.
  Serial.println(cds); // Serial 모니터에 cds 값을 찍어주고 한줄 내려줍니다.
  if (mfsr_r18 > 0)
  {
    colorWipe(strip.Color(255,   255,   255), 0, 10); // Red
  }
  else {
    colorWipe(strip.Color(0,   0,   0), 0, 10); // Red
  }

  if (cds < 50)
  {
    colorWipe(strip.Color(255,   0,   255), 10, 20); // Red
  }
  else
  {
    colorWipe(strip.Color(0,   0,   0), 10, 20); // Red
  }

}
void colorWipe(uint32_t color, int start, int finish) {
  for (int i = start; i < finish; i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    //turnOnLED(color, start);
    strip.show();                          //  Update strip to match
    delay(15);                           //  Pause for a moment
  }
}

void turnOnLED(uint32_t color, int start) {
  strip.setPixelColor(start, color);
  strip.setPixelColor(start++, color);
  strip.setPixelColor(start++, color);
  strip.setPixelColor(start++, color);
  strip.setPixelColor(start++, color);
  strip.setPixelColor(start++, color);
  strip.setPixelColor(start++, color);
  strip.setPixelColor(start++, color);
  strip.setPixelColor(start++, color);
  strip.setPixelColor(start++, color);
}
