#include <FastLED.h>
#include <SparkFunMMA8452Q.h>
#include <SparkFunMAX17043.h>

SYSTEM_MODE(MANUAL);

FASTLED_USING_NAMESPACE

#define NUM_LEDS 10
#define MIN_BRIGHTNESS 0
#define MAX_BRIGHTNESS 100

int brightness = 50;

double voltage = 0;
double soc = 0;

MMA8452Q accel;
CRGB leds[NUM_LEDS];

void setup()  {
  Serial.begin(9600);
  lipo.begin();
  FastLED.addLeds<APA102, A4, A5>(leds, NUM_LEDS);
  accel.begin(SCALE_2G, ODR_1);

  lipo.quickStart();

  Particle.variable("voltage", voltage);
  Particle.variable("soc", soc);
}

void loop()  {
  FastLED.setBrightness(brightness);
  for (size_t i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::White;
  }
  FastLED.show();

  if (accel.available()) {
    accel.read();

    if (accel.cx > 0.4) {
      // Serial.println("dim");
      brightness -= 10;
    } else if (accel.cx < -0.6) {
      // Serial.println("bright");
      brightness += 10;
    }

    if (brightness < MIN_BRIGHTNESS) {
      brightness = MIN_BRIGHTNESS;
    } else if (brightness > MAX_BRIGHTNESS) {
      brightness = MAX_BRIGHTNESS;
    }

    voltage = lipo.getVoltage();
    soc = lipo.getSOC();
  }
}
