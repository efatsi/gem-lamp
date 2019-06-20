#include <FastLED.h>
#include <SparkFunMMA8452Q.h>
#include <SparkFunMAX17043.h>

SYSTEM_MODE(MANUAL);

FASTLED_USING_NAMESPACE

#define NUM_LEDS 10
#define MIN_BRIGHTNESS 0
#define MAX_BRIGHTNESS 100

int brightness = 0;
int targetBrightness = 50;

double voltage = 0;
double soc = 0;

MMA8452Q accel;
CRGB leds[NUM_LEDS];

void setup()  {
  Serial.begin(9600);
  lipo.begin();
  accel.begin(SCALE_2G, ODR_1);
  FastLED.addLeds<APA102, A4, A5>(leds, NUM_LEDS);

  lipo.quickStart();

  Particle.variable("voltage", voltage);
  Particle.variable("soc", soc);
}

void loop()  {
  manageState();
  display();
}

void manageState() {
  if (accel.available()) {
    accel.read();

    if (accel.cz < 0) {
      // Serial.println("dim");
      targetBrightness -= 10;

      if (targetBrightness < MIN_BRIGHTNESS) {
        targetBrightness = MIN_BRIGHTNESS;
      }
    } else if (accel.cz > 0.8) {
      // Serial.println("bright");
      targetBrightness += 10;

      if (targetBrightness > MAX_BRIGHTNESS) {
        targetBrightness = MAX_BRIGHTNESS;
      }
    }

    voltage = lipo.getVoltage();
    soc = lipo.getSOC();
  }
}

void display() {
  if (targetBrightness != brightness) {
    FastLED.setBrightness(targetBrightness);
    for (size_t i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::White;
    }
    FastLED.show();

    brightness = targetBrightness;
  }
}
