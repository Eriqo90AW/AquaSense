#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <Adafruit_SSD1306.h>

TaskHandle_t TaskHandleTemp;
TaskHandle_t TaskHandleLevel;
TaskHandle_t TaskHandleTurbid;
TaskHandle_t TaskHandlePh;
TaskHandle_t TaskHandleOled;

const char *ssid = "abdul";     // Ganti dengan nama WiFi Anda
const char *password = "12345678";  // Ganti dengan kata sandi WiFi Anda

const int SensorDataPin = 18;
const int WaterLevelPin = 34;      // Pin untuk sensor water level
const int TurbiditySensorPin = 35; // Pin untuk sensor turbidity
const int PhSensorPin = 32;        // Pin untuk sensor pH
const int LEDPin = 5;              // Pin untuk LED

const int delayTime = 500;

float tempWater = 0.0; 
int levelWater = 0;
float levelTurbi = 0.0;
float levelPh = 0.0;

OneWire oneWire(SensorDataPin);
DallasTemperature sensors(&oneWire);

// Inisialisasi objek OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void connectToWiFi() {
  Serial.println("Connecting to WiFi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }

  Serial.println("Connected to WiFi");
}

void taskOled(void *pvParameters) {
    while (true) {
      display.clearDisplay();

      // Display title
      display.setTextSize(2);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 0);
      display.print("AQUASENSE");

      // Display temperature
      display.setTextSize(1);
      display.setCursor(0, 25);
      display.print("Temperature : ");
      display.print(tempWater);
      display.println(" C");

      // Display water level
      display.setCursor(0, 35);
      display.print("Water Level : ");
      display.println(levelWater);

      // Display turbidity
      display.setCursor(0, 45);
      display.print("Turbidity   : ");
      display.println(levelTurbi);

      // Display pH
      display.setCursor(0, 55);
      display.print("pH Level    : ");
      display.println(levelPh);

      Serial.println("Temp:\t\t" + String(tempWater));
      Serial.println("Water Level:\t" + String(levelWater));
      Serial.println("Turbidity:\t" + String(levelTurbi));
      Serial.println("pH Level:\t" + String(levelPh));

      display.display();
      delay(delayTime);
    
    }
}

void taskWaterTemp(void *pvParameters) {
  while (true) {
    // Read temperature
    sensors.requestTemperatures();
    float temperature_Celsius = sensors.getTempCByIndex(0);
    float temperature_Fahrenheit = sensors.getTempFByIndex(0);

    // Check if temperature is above 30 degrees Celsius
    if (temperature_Celsius > 30.0) {
      digitalWrite(LEDPin, HIGH);  // Turn on the LED
    } else {
      digitalWrite(LEDPin, LOW);   // Turn off the LED
    }

    tempWater = temperature_Celsius;
    delay(delayTime);
  }
}

void taskWaterLevel(void *pvParameters) {
  while (true) {
    // Read water level
    int waterLevel = analogRead(WaterLevelPin);
    levelWater = waterLevel;
    delay(delayTime);
  }
}

void taskTurbidity(void *pvParameters) {
  while (true) {
    // Read turbidity
    int turbidityValue = analogRead(TurbiditySensorPin);
    levelTurbi = turbidityValue;
    delay(delayTime);
  }
}

void taskPhSensor(void *pvParameters) {
  while (true) {
    // Read pH value (contoh menggunakan analogRead)
    int phValue = analogRead(PhSensorPin);
    
    // Lakukan konversi nilai pH berdasarkan karakteristik sensor pH yang digunakan
    levelPh = convertToPhLevel(phValue);

    delay(delayTime);
  }
}

float convertToPhLevel(int analogValue) {
  // Implementasikan konversi nilai analog ke pH sesuai dengan karakteristik sensor pH yang digunakan
  // Anda perlu merujuk ke dokumentasi atau spesifikasi sensor pH untuk implementasi yang sesuai
  // Contoh sederhana: phLevel = analogValue * (pHMax - pHMin) / 4095.0 + pHMin;
  return (analogValue * (14.0 - 0.0) / 4095.0 + 0.0) - 3;
}

void setup() {
  Serial.begin(115200);
  
  // Koneksi ke WiFi
  connectToWiFi();

  sensors.begin();
  pinMode(LEDPin, OUTPUT);

  // Inisialisasi OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println("SSD1306 allocation failed");
    return;
  }

  display.display();  // Clear the display buffer.
  delay(1000);  // Pause for a second

  xTaskCreatePinnedToCore(
    taskWaterTemp,           // function to execute
    "taskWaterTemp",         // name of the task
    10000,                   // stack size (bytes)
    NULL,                    // parameter to pass
    1,                       // priority (0 is the lowest priority)
    &TaskHandleTemp,         // task handle
    1                        // pin task to core 1
  );

  xTaskCreatePinnedToCore(
    taskWaterLevel,          // function to execute
    "taskWaterLevel",        // name of the task
    10000,                   // stack size (bytes)
    NULL,                    // parameter to pass
    0,                       // priority (0 is the lowest priority)
    &TaskHandleLevel,        // task handle
    0                        // pin task to core 0
  );

  xTaskCreatePinnedToCore(
    taskTurbidity,           // function to execute
    "taskTurbidity",         // name of the task
    10000,                   // stack size (bytes)
    NULL,                    // parameter to pass
    2,                       // priority (0 is the lowest priority)
    &TaskHandleTurbid,       // task handle
    0                        // pin task to core 0
  );

  xTaskCreatePinnedToCore(
    taskPhSensor,            // function to execute
    "taskPhSensor",          // name of the task
    10000,                    // stack size (bytes)
    NULL,                     // parameter to pass
    3,                        // priority (0 is the lowest priority)
    &TaskHandlePh,            // task handle
    0                         // pin task to core 0
  );

  xTaskCreatePinnedToCore(
    taskOled,                 // function to execute
    "taskOled",               // name of the task
    10000,                    // stack size (bytes)
    NULL,                     // parameter to pass
    4,                        // priority (0 is the lowest priority)
    &TaskHandleOled,          // task handle
    0                         // pin task to core 0
  );

  vTaskDelete(NULL);
}

void loop() {
  // Empty loop, the main code has been moved to tasks
}