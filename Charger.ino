#include <MAX17043.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

MAX17043 batteryMonitor;

float batteryPercentage;
float voltage;

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ 
  B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void center(String text, int y) {
  //Centers and displays text on OLED screen at position y (only for text size 3)
  //Characters are 15px X 21px
  int len = (text.length() * 15) + ((text.length() - 1) * 3);
  int x = (128 - len) / 2;
  display.setCursor(x, y);
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.println(text);
}

void setup() {
  Serial.begin(9600);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.clearDisplay();


  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  
  display.println("Charging Module Mk1.0");

  display.display();
  delay(300);
 

  //H
  display.drawLine(2, 20, 2, 35, WHITE);
  display.drawLine(2, 28, 22, 28, WHITE);
  display.drawLine(22, 20, 22, 35, WHITE);
  //.
  display.drawLine(26, 33, 26, 35, WHITE);
  display.drawLine(27, 33, 27, 35, WHITE);
  display.drawLine(28, 33, 28, 35, WHITE);
  //L
  display.drawLine(32, 20, 32, 35, WHITE);
  display.drawLine(32, 35, 53, 35, WHITE);
  //O
  display.drawRect(57, 20, 21, 16, WHITE);
  //V
  display.drawLine(81, 20, 91, 35, WHITE);
  display.drawLine(91, 35, 101, 20, WHITE);
  //E
  display.drawLine(105, 20, 105, 35, WHITE);
  display.drawLine(105, 35, 126, 35, WHITE);
  display.drawLine(105, 20, 126, 20, WHITE);
  display.drawLine(105, 28, 117, 28, WHITE);


  display.display();
  delay(300);
  



  //E
  display.drawLine(1, 44, 1, 52, WHITE);
  display.drawLine(1, 44, 11, 44, WHITE);
  display.drawLine(1, 52, 11, 52, WHITE);
  display.drawLine(1, 48, 7, 48, WHITE);
  display.display();
  delay(1);
  //N
  display.drawLine(14, 44, 14, 52, WHITE);
  display.drawLine(25, 44, 25, 52, WHITE);
  display.drawLine(14, 44, 25, 52, WHITE);
  display.display();
  delay(1);
  //G
  display.drawLine(28, 44, 28, 52, WHITE);
  display.drawLine(28, 44, 39, 44, WHITE);
  display.drawLine(28, 52, 39, 52, WHITE);
  display.drawLine(39, 52, 39, 48, WHITE);
  display.drawLine(39, 48, 34, 48, WHITE);
  display.display();
  delay(1);
  //I
  display.drawLine(42, 44, 42, 52, WHITE);
  display.display();
  delay(1);
  //N
  display.drawLine(45, 44, 45, 52, WHITE);
  display.drawLine(56, 44, 56, 52, WHITE);
  display.drawLine(45, 44, 56, 52, WHITE);
  display.display();
  delay(1);
  //E
  display.drawLine(59, 44, 59, 52, WHITE);
  display.drawLine(59, 44, 69, 44, WHITE);
  display.drawLine(59, 52, 69, 52, WHITE);
  display.drawLine(59, 48, 65, 48, WHITE);
  display.display();
  delay(1);
  //E
  display.drawLine(72, 44, 72, 52, WHITE);
  display.drawLine(72, 44, 82, 44, WHITE);
  display.drawLine(72, 52, 82, 52, WHITE);
  display.drawLine(72, 48, 78, 48, WHITE);
  display.display();
  delay(1);
  //R
  display.drawRoundRect(85, 44, 12, 5, 2, WHITE);
  display.drawLine(85, 44, 85, 52, WHITE);
  display.drawLine(93, 49, 96, 52, WHITE);
  display.display();
  delay(1);
  //I
  display.drawLine(99, 44, 99, 52, WHITE);
  display.display();
  delay(1);
  //N
  display.drawLine(102, 44, 102, 52, WHITE);
  display.drawLine(113, 44, 113, 52, WHITE);
  display.drawLine(102, 44, 113, 52, WHITE);
  display.display();
  delay(1);
  //G
  display.drawLine(116, 44, 116, 52, WHITE);
  display.drawLine(116, 44, 127, 44, WHITE);
  display.drawLine(116, 52, 127, 52, WHITE);
  display.drawLine(127, 52, 127, 48, WHITE);
  display.drawLine(127, 48, 122, 48, WHITE);
  display.display();
  delay(1);

  display.display();
  delay(5000);

  display.clearDisplay();



  
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println("In collaboration with");
  display.drawBitmap(0, 16, logo16_glcd_bmp, 16, 16, WHITE);
  display.setCursor(20, 16);
  display.setTextSize(2);
  display.println("Adafruit");
  display.println("Industries");
  display.display();
  delay(3000);

  // Clear the buffer.
  display.clearDisplay();
  
  Wire.begin();
  batteryMonitor.reset();
  batteryMonitor.quickStart();
  
}

void loop() {
  batteryPercentage = batteryMonitor.getSoC();
  voltage = batteryMonitor.getVCell();
  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  display.clearDisplay();
  
  display.println("Battery Percentage:");
  
  center(String(int(batteryPercentage)) + "%", 17);

  
  //Charging bar
  
  //Solid
  //display.drawRect(0, 49, 128, 15, WHITE);
  //display.fillRect(0, 49, float((batteryPercentage / 100) * display.width()), 15, WHITE);

  //Boardered
  display.drawRoundRect(0, 49, 128, 15, 2, WHITE);
  display.fillRect(3, 52, float((batteryPercentage / 100) * (display.width() - 6)), 9, WHITE);

  Serial.println(batteryPercentage);
  Serial.println(voltage);
  
  display.display();
  delay(500);
}
