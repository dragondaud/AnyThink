// myMoodlight
// by David Denney <dragondaud@gmail.com>
// based on project https://www.instructables.com/Mood-Light-That-Matches-Any-Color-You-Tap-on-the-A/
// made for Arduino https://docs.arduino.cc/language-reference/

#include <FastLED.h>  // https://fastled.io/

#define LED_PIN 6         // D6 (PA07) for WS2812 LEDs
#define LED_COUNT 50      // Total number of LEDs on strand
#define BRIGHTNESS 64     // Adjust as needed (0-255)
#define LED_TYPE WS2812B  // Type of LED pixels
#define COLOR_ORDER GRB   // Color order

// Touch sensor input pins
#define TOUCH_PIN_0 0  // D0 (PA02)
#define TOUCH_PIN_1 1  // D1 (PA04)
#define TOUCH_PIN_2 2  // D2 (PA10)
#define TOUCH_PIN_3 3  // D3 (PA11)
#define TOUCH_PIN_4 4  // D4 (PA08)

// Define the LED array
CRGB leds[LED_COUNT];

// Define array of colors, for each button, 1 - 5, and 0 for default pattern
// from predefined color names or triad as CRGB(200, 200, 200)
// color names https://github.com/FastLED/FastLED/blob/master/src/crgb.h#L557
// original pastel colors
// CRGB ledColors[] = { CRGB(0, 0, 0), CRGB(255, 255, 51), CRGB(128, 0, 255), CRGB(204, 0, 0), CRGB(0, 204, 0), CRGB(0, 102, 204) };
CRGB ledColors[] = { CRGB(0, 0, 0), CRGB::Yellow, CRGB::DarkViolet, CRGB::DarkRed, CRGB::DarkGreen, CRGB::DarkBlue };

// Selected color and previous color, global vars initialize to zero
int Color, lastColor;

// track double taps
const unsigned long DOUBLE_TAP_TIMEOUT = 1500;  // milliseconds
unsigned long firstTapTime;
bool tap = false;

void setup() {
  // Initialize serial and wait to open, for debugging
  // https://docs.arduino.cc/language-reference/en/functions/communication/serial/
  Serial.begin(9600);
  delay(2000);
  Serial.println("myMoodlight Ready");

  // Initialize FastLED library
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, LED_COUNT);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();

  // Set up touch sensor pins as inputs
  // https://docs.arduino.cc/language-reference/en/functions/digital-io/pinMode/
  pinMode(TOUCH_PIN_0, INPUT);
  pinMode(TOUCH_PIN_1, INPUT);
  pinMode(TOUCH_PIN_2, INPUT);
  pinMode(TOUCH_PIN_3, INPUT);
  pinMode(TOUCH_PIN_4, INPUT);

  // configure interrupts
  // https://docs.arduino.cc/language-reference/en/functions/external-interrupts/attachInterrupt/
  attachInterrupt(digitalPinToInterrupt(TOUCH_PIN_0), handle_1, RISING);
  attachInterrupt(digitalPinToInterrupt(TOUCH_PIN_1), handle_2, RISING);
  attachInterrupt(digitalPinToInterrupt(TOUCH_PIN_2), handle_3, RISING);
  attachInterrupt(digitalPinToInterrupt(TOUCH_PIN_3), handle_4, RISING);
  attachInterrupt(digitalPinToInterrupt(TOUCH_PIN_4), handle_5, RISING);

  // Small delay for stability
  // https://docs.arduino.cc/language-reference/en/functions/time/delay/
  delay(50);

  // Display a default pattern
  showDefaultPattern();
}

// Interrupt handlers for each touch pin and unhandled tap tracking
// For more information on interrupts: https://gammon.com.au/interrupts
void handle_1() {
  Color = 1;
  tap = true;
}

void handle_2() {
  Color = 2;
  tap = true;
}

void handle_3() {
  Color = 3;
  tap = true;
}

void handle_4() {
  Color = 4;
  tap = true;
}

void handle_5() {
  Color = 5;
  tap = true;
}

void loop() {
  // If selected Color has changed, update led display, remember selection
  if (Color != lastColor) {
    lastColor = Color;

    // output touch color to serial, for debugging
    uint8_t r = ledColors[Color].r;
    uint8_t g = ledColors[Color].g;
    uint8_t b = ledColors[Color].b;
    Serial.printf("Touch#%d: (%d, %d, %d)\n", Color, r, g, b);

    fill_solid(leds, LED_COUNT, ledColors[Color]);  // fill array with Color
    FastLED.show();                                 // display array on leds
    firstTapTime = millis();                        // track tap time
    delay(50);                                      // prevent bounce
    tap = false;                                    // reset tap tracking

  } else if (Color == lastColor and tap == true) {  // new tap same color
    // second tap within timeout == double tap
    if (millis() - firstTapTime < DOUBLE_TAP_TIMEOUT) {
      Serial.println("Double tap");
      showDefaultPattern();
    }
    tap = false;
  }

  if (Serial.available()) {  // check Serial input buffer
    Serial.readString();     // read string to eol or timeout
    showDefaultPattern();    // reset to default pattern on any serial input
  }
}

void showDefaultPattern() {
  int splitCount = LED_COUNT / 5;  // split available leds into 5 sections
  Serial.println("Show default pattern");
  fill_solid(leds, splitCount, ledColors[1]);
  fill_solid(leds + (splitCount * 1), splitCount, ledColors[2]);
  fill_solid(leds + (splitCount * 2), splitCount, ledColors[3]);
  fill_solid(leds + (splitCount * 3), splitCount, ledColors[4]);
  fill_solid(leds + (splitCount * 4), splitCount, ledColors[5]);
  FastLED.show();
  delay(50);
  Color = lastColor = 0;
}
