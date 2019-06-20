#include <FastLED.h>
#include <SparkFunMMA8452Q.h>
#include <SparkFunMAX17043.h>

SYSTEM_MODE(MANUAL);

FASTLED_USING_NAMESPACE

#define NUM_LEDS 10
#define MIN_BRIGHTNESS 2
#define MID_BRIGHTNESS 15
#define MAX_BRIGHTNESS 40
#define COLOR_COUNT 8

int brightness = MIN_BRIGHTNESS;
int targetBrightness = 9;

double voltage = 0;
double soc = 0;

MMA8452Q accel;
CRGB leds[NUM_LEDS];
int colorIndex = 0;
int colors[COLOR_COUNT] = {
  CRGB::White,
  CRGB::OrangeRed,
  CRGB::PaleVioletRed,
  CRGB::DarkRed,
  CRGB::Teal,
  CRGB::CadetBlue,
  CRGB::Yellow,
  CRGB::LightGoldenrodYellow
};

void setup()  {
  Serial.begin(9600);
  lipo.begin();
  accel.begin(SCALE_2G, ODR_1);
  FastLED.addLeds<APA102, A4, A5>(leds, NUM_LEDS);

  lipo.quickStart();

  Particle.variable("voltage", voltage);
  Particle.variable("soc", soc);

  // disable onboard LED
  ::RGB.control(true);
  ::RGB.color(0, 0, 0);
}

void loop()  {
  manageState();
  display();
}

void manageState() {
  // ## Face Down Z < -0.82
  // ## Face Up Z > 0.93
  // ## Left X > 0.9
  // ## Right X < -0.9

  if (accel.available()) {
    accel.read();

    if (accel.cz < -0.82) {
      // down
      targetBrightness = MIN_BRIGHTNESS;
    } else if (accel.cz > 0.9) {
      // up
      targetBrightness = MAX_BRIGHTNESS;
    } else if (accel.cx > 0.9) {
      // left
      colorIndex = 0;
      targetBrightness = MID_BRIGHTNESS;
      if (brightness == targetBrightness) {
        brightness -= 5;
      }
    } else if (accel.cx < -0.9) {
      // right
      int newColor = colorIndex;
      while (newColor == colorIndex) {
        newColor = random(0, COLOR_COUNT);
      }

      colorIndex = newColor;
      brightness = max(MIN_BRIGHTNESS - 1, brightness - 5);
    }

    voltage = lipo.getVoltage();
    soc = lipo.getSOC();
  }
}

void display() {
  if (brightness < targetBrightness) {
    int wait = 1000 / (targetBrightness - brightness);
    if (targetBrightness - brightness < 10) {
      wait = wait / 2;
    }

    for (int i = brightness; i <= targetBrightness; i++) {
      writeToLights(colors[colorIndex], i);
      delay(wait);
    }
  } else if (brightness > targetBrightness) {
    int wait = 1000 / (brightness - targetBrightness);
    if (brightness - targetBrightness < 10) {
      wait = wait / 2;
    }

    for (int i = brightness; i >= targetBrightness; i--) {
      writeToLights(colors[colorIndex], i);
      delay(wait);
    }
  }

  brightness = targetBrightness;

  // Give a little time to debounce
  delay(500);
}

void writeToLights(int color, int brightness) {
  FastLED.setBrightness(brightness);

  for (size_t i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;
  }
  FastLED.show();
}
