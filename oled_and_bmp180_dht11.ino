#include <Wire.h>
#include <Adafruit_BMP085.h>
#include "U8glib.h"
#include <Adafruit_Sensor.h>
#include "DHT.h"

Adafruit_BMP085 bmp;
#define DHTTYPE    DHT11     // DHT 11
#define DHTPIN 2 
DHT dht(DHTPIN, DHTTYPE);
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);  // I2C / TWI
 
float pressure = 0.0;
float tempC = 0.0;
float humidity = 0.0;
float altitude = 0.0;
void BmpSensorRead(float* pressure, float* tempC, float* altitude,float* humidity);
void DisplayPresTemp(float* pressure, float* tempC, float* altitude,float* humidity);

void setup(void)
{
  Serial.begin(9600);
  dht.begin();
  // assign default color value
  if (u8g.getMode() == U8G_MODE_R3G3B2)
  {
    u8g.setColorIndex(255);     // white
  }
  else if (u8g.getMode() == U8G_MODE_GRAY2BIT)
  {
    u8g.setColorIndex(3);         // max intensity
  }
  else if (u8g.getMode() == U8G_MODE_BW)
  {
    u8g.setColorIndex(1);         // pixel on
  }
  else if (u8g.getMode() == U8G_MODE_HICOLOR)
  {
    u8g.setHiColorByRGB(255, 255, 255);
  }

  for (int a = 0; a < 30; a++)
  {
    u8g.firstPage();

    do
    {
      u8g.setFont(u8g_font_osb18);
      u8g.setFontRefHeightExtendedText();
      u8g.setDefaultForegroundColor();
      u8g.setFontPosTop();
      u8g.drawStr(4, a, "Weather Station");
    }
    while (u8g.nextPage());
  }

  delay(3000);

  if (!bmp.begin())
  {
    u8g.firstPage();

    do
    {
      u8g.setFont(u8g_font_fub11);
      u8g.setFontRefHeightExtendedText();
      u8g.setDefaultForegroundColor();
      u8g.setFontPosTop();
      u8g.drawStr(4, 0, "BMP085 Sensor");
      u8g.drawStr(4, 20, " ERROR!");
    }
    while (u8g.nextPage());

    Serial.println("BMP085 sensor, ERROR!");

    while (1) {}
  }
}
void loop(void)
{
  BmpSensorRead(&pressure, &tempC, &altitude,&humidity);
  DisplayPresTemp(&pressure, &tempC, &altitude,&humidity);
  delay(1000);
}
void DisplayPresTemp(float* pressure, float* tempC, float* altitude,float* humidity)
{
  u8g.firstPage();

  do
  {
    u8g.setFont(u8g_font_fub11);
    u8g.setFontRefHeightExtendedText();
    u8g.setDefaultForegroundColor();
    u8g.setFontPosTop();
    u8g.drawStr(2, 2, "Press.");
    u8g.setPrintPos(65, 2);
    u8g.print(*pressure);
    u8g.drawStr(2, 22, "Temp C");
    u8g.setPrintPos(77, 22);
    u8g.print(*tempC);
    u8g.drawStr(4, 42, "humidity");
    u8g.setPrintPos(75, 42);
    u8g.print(*humidity);
  }
  while (u8g.nextPage());
}

void BmpSensorRead(float* pressure, float* tempC, float* altitude,float* humidity)
{
  *tempC = bmp.readTemperature();
  Serial.print("Temperature = ");
  Serial.print(*tempC);
  Serial.println(" *C");

  *pressure = bmp.readPressure() / 100.0;
  Serial.print("Pressure = ");
  Serial.print(*pressure / 100.0);
  Serial.println(" hPa");

  // Calculate altitude assuming 'standard' barometric
  // pressure of 1013.25 millibar = 101325 Pascal
  *altitude = bmp.readAltitude();
  Serial.print("Altitude = ");
  Serial.print(*altitude);
  Serial.println(" meters");

  *humidity = dht.readHumidity();
  Serial.print("humidity = ");
  Serial.print(*humidity);
  Serial.println(" RH");
  
}
