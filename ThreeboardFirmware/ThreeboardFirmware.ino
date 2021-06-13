// Forked from the great Zack Freedman, rewrote the display driver and strip out bluefruit.
bool serialDebug = 1; // enable debug info on serial port

int OLEDDisplayTimeout = 10; // oled menu display timeout (seconds)
bool displayDimState = false;

#include <Bounce2.h>
#include <Keyboard.h>
#define OLED_ADDR 0x3C // OLED i2c address
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define I2C_SDA SDA
#define I2C_SCL SCL

// oled SSD1306 display connected to I2C (SDA, SCL pins)
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int leftButtonPin = 12;
int midButtonPin = 11;
int rightButtonPin = 10;

Bounce leftButton = Bounce();
Bounce midButton = Bounce();
Bounce rightButton = Bounce();

bool hasKeyPressed;

uint8_t keycode7[6] = { 0x5F };
uint8_t keycode8[6] = { 0x60 };
uint8_t keycode9[6] = { 0x61 };


void setup()
{
  Serial.begin(115200);
  pinMode(leftButtonPin, INPUT_PULLDOWN);
  pinMode(midButtonPin, INPUT_PULLDOWN);
  pinMode(rightButtonPin, INPUT_PULLDOWN);
  leftButton.attach(leftButtonPin);
  leftButton.interval(5); // interval in ms

  midButton.attach(midButtonPin);
  midButton.interval(5); // interval in ms

  rightButton.attach(rightButtonPin);
  rightButton.interval(5); // interval in ms


  // initialise the oled display
  Wire.begin(I2C_SDA, I2C_SCL); // if you get an error it may be the board you are using does not allow defining the pins in which case try:  Wire.begin();
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR))
  {
    Serial.println(("\nError initialising the oled display"));
  }
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.drawRect(0, 0, 128, 32, WHITE);
    display.setTextSize(2);
    display.setCursor(3, 3);
    display.print("Press");
    display.setCursor(30, 15);
    display.print("a key...");
    display.display();
    delay(2000);
    //  display.ssd1306_command(SSD1306_SETCONTRAST);
    //  display.ssd1306_command(10); // Where c is a value from 0 to 255 (sets contrast e.g. brightness)
    display.ssd1306_command(SSD1306_DISPLAYOFF); // To switch display off
}


void loop()
{
  leftButton.update();
  midButton.update();
  rightButton.update();

  bool leftButtonPressed = leftButton.fell();
  bool midButtonPressed = midButton.fell();
  bool rightButtonPressed = rightButton.fell();

  int i, x, y;
  char szTemp[32];
  unsigned long ms;

  if ( hasKeyPressed )
  {
    hasKeyPressed = false;
    //    blehid.keyRelease();
    Keyboard.releaseAll();
    // Delay a bit after a report
    delay(5);
  }


  if (leftButtonPressed) {
    hasKeyPressed = true;
    Serial.println("leftButtonPressed");
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('m');
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('t');
    display.ssd1306_command(SSD1306_DISPLAYON);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.print("Minimize");
    display.setCursor(35, 15);
    display.print("Or Mute");
    display.display();
    delay(2000);
    display.ssd1306_command(SSD1306_DISPLAYOFF);
  }

  if (midButtonPressed) {
    hasKeyPressed = true;
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('t');
    display.ssd1306_command(SSD1306_DISPLAYON);
    display.clearDisplay();
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.print("Middle");
    display.setCursor(35, 15);
    display.print("Button");
    display.display();
    delay(2000);
    display.ssd1306_command(SSD1306_DISPLAYOFF);
  }

  if (rightButtonPressed) {
    Serial.println("rightButtonPressed");
    hasKeyPressed = true;
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('q');
    display.ssd1306_command(SSD1306_DISPLAYON);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.print("Locked!");
    display.display();
    delay(2000);
    display.ssd1306_command(SSD1306_DISPLAYOFF);
  }
}

