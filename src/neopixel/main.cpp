#include <Adafruit_NeoPixel.h>

#define DIN_PIN    (23)            // NeoPixel　の出力ピン番号はGP23
#define LED_COUNT  (1)             // LEDの連結数
#define WAIT_MS    (1000)          // 次の点灯までのウエイト
#define BRIGHTNESS (128)           // 輝度
Adafruit_NeoPixel pixels(LED_COUNT, DIN_PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  pixels.begin();             //NeoPixel制御開始
}

void loop()
{
  Serial.println("点灯開始");
  pixels.clear();
  
  //pixels.Color(Red, Green, Blue)で、パレット情報を作成する。
  //赤点灯
  pixels.setPixelColor(0, pixels.Color(BRIGHTNESS, 0, 0));
  pixels.show();
  delay(WAIT_MS);

  //緑点灯
  pixels.setPixelColor(0, pixels.Color(0, BRIGHTNESS, 0));
  pixels.show();
  delay(WAIT_MS);

  //赤　＋　緑　で　黄点灯
  pixels.setPixelColor(0, pixels.Color(BRIGHTNESS, BRIGHTNESS, 0));
  pixels.show();
  delay(WAIT_MS);

  //青点灯
  pixels.setPixelColor(0, pixels.Color(0, 0, BRIGHTNESS));
  pixels.show();
  delay(WAIT_MS);

  //赤　＋　青　で　紫点灯
  pixels.setPixelColor(0, pixels.Color(BRIGHTNESS, 0, BRIGHTNESS));
  pixels.show();
  delay(WAIT_MS);

  //緑　＋　青　で　水点灯
  pixels.setPixelColor(0, pixels.Color(0, BRIGHTNESS, BRIGHTNESS));
  pixels.show();
  delay(WAIT_MS);

  //赤　＋　緑　＋　青　で　白点灯
  pixels.setPixelColor(0, pixels.Color(BRIGHTNESS, BRIGHTNESS, BRIGHTNESS));
  pixels.show();
  delay(WAIT_MS);
}