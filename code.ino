#include <Adafruit_NeoPixel.h>
#include <DHT.h>

#define PIXELS_PER_SEGMENT  3
#define DIGITS              2
#define PIN                 5
#define DHTPIN              4
#define DHTTYPE             DHT22   // Sensor DHT 22  (AM2302)

DHT dht(DHTPIN, DHTTYPE);

byte segments[10] = {
  0b1111110,
  0b0011000,
  0b0110111,
  0b0111101,
  0b1011001,
  0b1101101,
  0b1101111,
  0b0111000,
  0b1111111,
  0b1111001
};

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXELS_PER_SEGMENT * 7 * DIGITS, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  strip.begin();
  Serial.begin(9600); 
  dht.begin(); 
}

void writeNum(int num, int type) {
  for (int i = 0; i <= DIGITS - 1; i++) {
    writeDigit(i, num % 10, type);
    num /= 10;
  }
}

void writeDigit(int index, int value, int type) {
  byte seg = segments[value];
  for (int i = 6; i >= 0; i--) {
    int offset = index * (PIXELS_PER_SEGMENT * 7) + i * PIXELS_PER_SEGMENT;

    uint32_t color = 0;
    if (type == 1){
      color = seg & 0x01 != 0 ? strip.Color(255, 0, 0) : strip.Color(0, 0, 0);
    }else{
      color = seg & 0x01 != 0 ? strip.Color(0, 0, 255) : strip.Color(0, 0, 0);  
    }
    
    for (int x = offset; x < offset + PIXELS_PER_SEGMENT; x++) {
      strip.setPixelColor(x, color);
    }
    seg = seg >> 1;
  }
}

void clearDisplay() {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
}

void loop()
{
    clearDisplay();
    
    // Read Humidity
    float h = dht.readHumidity();
    // Read  temperature Celsius
    float t = dht.readTemperature();
  
    Serial.print("Humidity: "); 
    Serial.print(h);
    Serial.println(" %");
  
    Serial.print("Temperature: "); 
    Serial.print(t);
    Serial.println(" C  ");

    writeNum(t,1);// temp - 1=color red
    strip.show();
    delay(3000);
    
    
    writeNum(h,0);// humidity - 0=color blue
    strip.show();
    delay(3000);

}
