#include <FastLED.h>

#define LED_PIN 6         // D6 (PA07) for WS2812 LEDs
#define LED_COUNT 50      // Total number of LEDs
#define BRIGHTNESS 100    // Adjust as needed (0-255)
#define LED_TYPE WS2812B  // Type of LED pixels
#define COLOR_ORDER GRB   // Color order

// Input pins
#define TOUCH_PIN_0 0  // D0 (PA02)
#define TOUCH_PIN_1 1  // D1 (PA04)
#define TOUCH_PIN_2 2  // D2 (PA10)
#define TOUCH_PIN_3 3  // D3 (PA11)
#define TOUCH_PIN_4 4  // D4 (PA08)

// Define the LED array
CRGB leds[LED_COUNT];

// Colors using RGB values directly
// Much more saturated colors with a hint of pastel
const CRGB COLOR_GREEN = CRGB(0, 204, 0);      // RGB: (0, 204, 0) - Very saturated green
const CRGB COLOR_BLUE = CRGB(0, 102, 204);     // RGB: (0, 102, 204) - Much more saturated blue
const CRGB COLOR_RED = CRGB(204, 0, 0);        // RGB: (204, 0, 0) - More saturated red
const CRGB COLOR_YELLOW = CRGB(255, 255, 51);  // RGB: (255, 255, 51) - Much more saturated yellow
const CRGB COLOR_VIOLET = CRGB(128, 0, 255);   // RGB: (128, 0, 255) - More saturated violet

CRGB Color, lastColor;

void setup() {
  //Initialize serial and wait for port to open
  Serial.begin(9600);
  while (!Serial) { delay(250); }
  Serial.println("myMoodlight Serial Open");

  // Initialize FastLED
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, LED_COUNT);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();

  // Set up touch sensor pins as inputs
  pinMode(TOUCH_PIN_0, INPUT);
  pinMode(TOUCH_PIN_1, INPUT);
  pinMode(TOUCH_PIN_2, INPUT);
  pinMode(TOUCH_PIN_3, INPUT);
  pinMode(TOUCH_PIN_4, INPUT);

  // configure interrupts
  attachInterrupt(digitalPinToInterrupt(TOUCH_PIN_0), handle_0, RISING);
  attachInterrupt(digitalPinToInterrupt(TOUCH_PIN_1), handle_1, RISING);
  attachInterrupt(digitalPinToInterrupt(TOUCH_PIN_2), handle_2, RISING);
  attachInterrupt(digitalPinToInterrupt(TOUCH_PIN_3), handle_3, RISING);
  attachInterrupt(digitalPinToInterrupt(TOUCH_PIN_4), handle_4, RISING);

  // Small delay for stability
  delay(100);

  showDefaultPattern();
}

void handle_0() {
  Color = COLOR_YELLOW;
}

void handle_1() {
  Color = COLOR_VIOLET;
}

void handle_2() {
  Color = COLOR_RED;
}

void handle_3() {
  Color = COLOR_GREEN;
}

void handle_4() {
  Color = COLOR_BLUE;
}

void loop() {
  if (Color != lastColor) {
    lastColor = Color;
    uint8_t r = Color.r;
    uint8_t g = Color.g;
    uint8_t b = Color.b;
    char hexString[7];
    sprintf(hexString, "%02X%02X%02X", r, g, b);
    Serial.println(hexString);
    fill_solid(leds, LED_COUNT, Color);
    FastLED.show();
  }

  delay(50);
}

void showDefaultPattern() {
  int splitCount = LED_COUNT / 5;
  fill_solid(leds, splitCount, COLOR_RED);
  fill_solid(leds + (splitCount * 1), splitCount, COLOR_VIOLET);
  fill_solid(leds + (splitCount * 2), splitCount, COLOR_YELLOW);
  fill_solid(leds + (splitCount * 3), splitCount, COLOR_GREEN);
  fill_solid(leds + (splitCount * 4), splitCount, COLOR_BLUE);
  FastLED.show();
  Serial.println("Show Default Pattern");
}
