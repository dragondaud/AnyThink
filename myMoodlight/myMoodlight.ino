#include <FastLED.h>

#define LED_PIN 6       // D6 (PA07) for WS2812 LEDs
#define LED_COUNT 60    // Total number of LEDs
#define BRIGHTNESS 100  // Adjust as needed (0-255)

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


void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) { delay(250); }
  Serial.println("myMoodlight Serial Open");

  // Initialize FastLED
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LED_COUNT);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();

  // Set up touch sensor pins as inputs
  pinMode(TOUCH_PIN_0, INPUT);
  pinMode(TOUCH_PIN_1, INPUT);
  pinMode(TOUCH_PIN_2, INPUT);
  pinMode(TOUCH_PIN_3, INPUT);
  pinMode(TOUCH_PIN_4, INPUT);

  // Small delay for stability
  delay(50);

  showDefaultPattern();
}

void loop() {
  // Check touch sensors
  if (digitalRead(TOUCH_PIN_0) == HIGH) {
    // All LEDs green
    fill_solid(leds, LED_COUNT, COLOR_YELLOW);
    Serial.println("Touch0");
  } else if (digitalRead(TOUCH_PIN_1) == HIGH) {
    // All LEDs blue
    fill_solid(leds, LED_COUNT, COLOR_VIOLET);
    Serial.println("Touch1");
  } else if (digitalRead(TOUCH_PIN_2) == HIGH) {
    // All LEDs red
    fill_solid(leds, LED_COUNT, COLOR_RED);
    Serial.println("Touch2");
  } else if (digitalRead(TOUCH_PIN_3) == HIGH) {
    // All LEDs yellow
    fill_solid(leds, LED_COUNT, COLOR_GREEN);
    Serial.println("Touch3");
  } else if (digitalRead(TOUCH_PIN_4) == HIGH) {
    // All LEDs violet
    fill_solid(leds, LED_COUNT, COLOR_BLUE);
    Serial.println("Touch4");
  }

  // Update the LEDs
  FastLED.show();

  // Small delay to prevent flickering
  delay(50);
}

void showDefaultPattern() {
  // First 10 LEDs green (0-9)
  fill_solid(leds, 14, COLOR_RED);

  // 11th to 20th blue (10-19)
  fill_solid(leds + 14, 5, COLOR_VIOLET);

  // 20th to 25th red (20-24)
  fill_solid(leds + 19, 14, COLOR_YELLOW);

  // 25th to 40th yellow (25-39)
  fill_solid(leds + 33, 12, COLOR_GREEN);

  // 40th to 60th violet (40-59)
  fill_solid(leds + 45, 15, COLOR_BLUE);

  Serial.println("Show Default Pattern");
}
