// myMoodlight
// by David Denney <dragondaud@gmail.com>
// based on project https://www.instructables.com/Mood-Light-That-Matches-Any-Color-You-Tap-on-the-A/

#include <FastLED.h>  // https://fastled.io/

#define LED_PIN 6         // D6 (PA07) for WS2812 LEDs
#define LED_COUNT 50      // Total number of LEDs on strand
#define BRIGHTNESS 100    // Adjust as needed (0-255)
#define LED_TYPE WS2812B  // Type of LED pixels
#define COLOR_ORDER GRB   // Color order

// Input pins
#define TOUCH_PIN_0 0  // D0 (PA02)
#define TOUCH_PIN_1 1  // D1 (PA04)
#define TOUCH_PIN_2 2  // D2 (PA10)
#define TOUCH_PIN_3 3  // D3 (PA11)
#define TOUCH_PIN_4 4  // D4 (PA08)

// Colors using RGB values directly, instead of predefined colors
const CRGB COLOR_GREEN = CRGB(0, 204, 0);      // RGB: (0, 204, 0) - Very saturated green
const CRGB COLOR_BLUE = CRGB(0, 102, 204);     // RGB: (0, 102, 204) - Much more saturated blue
const CRGB COLOR_RED = CRGB(204, 0, 0);        // RGB: (204, 0, 0) - More saturated red
const CRGB COLOR_YELLOW = CRGB(255, 255, 51);  // RGB: (255, 255, 51) - Much more saturated yellow
const CRGB COLOR_VIOLET = CRGB(128, 0, 255);   // RGB: (128, 0, 255) - More saturated violet

// Define the LED array
CRGB leds[LED_COUNT];

// Define colors for each button, 0 - 4, in order from predefined colors or
// color names https://github.com/FastLED/FastLED/blob/master/src/crgb.h#L557
CRGB ledColors[] = { COLOR_YELLOW, COLOR_VIOLET, COLOR_RED, COLOR_GREEN, COLOR_BLUE };

// Selected color and previous color
int Color, lastColor;

void setup() {
  // Initialize serial and wait for port to open
  // https://docs.arduino.cc/language-reference/en/functions/communication/serial/
  Serial.begin(9600);
  delay(250);  // wait for serial port to initialize
  Serial.println("myMoodlight Serial Open");

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
  attachInterrupt(digitalPinToInterrupt(TOUCH_PIN_0), handle_0, RISING);
  attachInterrupt(digitalPinToInterrupt(TOUCH_PIN_1), handle_1, RISING);
  attachInterrupt(digitalPinToInterrupt(TOUCH_PIN_2), handle_2, RISING);
  attachInterrupt(digitalPinToInterrupt(TOUCH_PIN_3), handle_3, RISING);
  attachInterrupt(digitalPinToInterrupt(TOUCH_PIN_4), handle_4, RISING);

  // Small delay for stability
  // https://docs.arduino.cc/language-reference/en/functions/time/delay/
  delay(50);

  // Display a default pattern
  showDefaultPattern();
}


// Interrupt handlers for each touch pin
void handle_0() {
  Color = 0;
}

void handle_1() {
  Color = 1;
}

void handle_2() {
  Color = 2;
}

void handle_3() {
  Color = 3;
}

void handle_4() {
  Color = 4;
}

void loop() {
  // If selected Color has changed, update led display, remember selection
  if (Color != lastColor) {
    lastColor = Color;
    uint8_t r = ledColors[Color].r;
    uint8_t g = ledColors[Color].g;
    uint8_t b = ledColors[Color].b;
    Serial.printf("Touch#%d: (%d, %d, %d)\n", Color, r, g, b);
    fill_solid(leds, LED_COUNT, ledColors[Color]);  // fill array with Color
    FastLED.show();                                 // display array on leds
    delay(50);
  }
  if (Serial.available()) {
    Serial.readString();
    showDefaultPattern();  // reset to default pattern on serial input
    delay(50);
  }
}

void showDefaultPattern() {
  int splitCount = LED_COUNT / 5;  // split available leds into 5 sections
  Serial.println("Show Default Pattern");
  fill_solid(leds, splitCount, COLOR_RED);
  fill_solid(leds + (splitCount * 1), splitCount, COLOR_VIOLET);
  fill_solid(leds + (splitCount * 2), splitCount, COLOR_YELLOW);
  fill_solid(leds + (splitCount * 3), splitCount, COLOR_GREEN);
  fill_solid(leds + (splitCount * 4), splitCount, COLOR_BLUE);
  FastLED.show();
}
