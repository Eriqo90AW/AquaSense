#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <Adafruit_SSD1306.h>

const char *ssid = "Wokwi-GUEST";     // Ganti dengan nama WiFi Anda
const char *password = "";  // Ganti dengan https://wokwi.com/makers/urishkata sandi WiFi Anda

const int SensorDataPin = 2;
const int WaterLevelPin = 3;      // Pin untuk sensor water level
const int TurbiditySensorPin = 4; // Pin untuk sensor turbidity
const int LEDPin = 5;             // Pin untuk LED

OneWire oneWire(SensorDataPin);
DallasTemperature sensors(&oneWire);

// Inisialisasi objek OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);
  
  // Koneksi ke WiFi
  connectToWiFi();

  sensors.begin();
  pinMode(LEDPin, OUTPUT);

  // Inisialisasi OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.display();  // Clear the display buffer.
  delay(2000);  // Pause for 2 seconds

  // Assign the loop function to a specific core
  TaskHandle_t TaskCore1;
  xTaskCreatePinnedToCore(
    taskWaterTemp,           // function to execute
    "taskWaterTemp",         // name of the task
    10000,                   // stack size (bytes)
    NULL,                    // parameter to pass
    1,                       // priority (0 is the lowest priority)
    &TaskCore1,              // task handle
    1                        // pin task to core 1
  );

  // Assign a different task for water level to another core
  TaskHandle_t TaskCore0;
  xTaskCreatePinnedToCore(
    taskWaterLevel,          // function to execute
    "taskWaterLevel",        // name of the task
    10000,                   // stack size (bytes)
    NULL,                    // parameter to pass
    0,                       // priority (0 is the lowest priority)
    &TaskCore0,              // task handle
    0                        // pin task to core 0
  );

  // Assign a task for turbidity to another core
  TaskHandle_t TaskCore2;
  xTaskCreatePinnedToCore(
    taskTurbidity,           // function to execute
    "taskTurbidity",         // name of the task
    10000,                   // stack size (bytes)
    NULL,                    // parameter to pass
    2,                       // priority (0 is the lowest priority)
    &TaskCore2,              // task handle
    0                        // pin task to core 0
  );
}

void connectToWiFi() {
  Serial.println("Connecting to WiFi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }

  Serial.println("Connected to WiFi");
}

void displayData(float temperature_Celsius, int waterLevel, int turbidityValue) {
  display.clearDisplay();
  
  // Display temperature
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Temp: ");
  display.print(temperature_Celsius);
  display.println(" C");

  // Display water level
  display.setCursor(0, 10);
  display.print("Water Level: ");
  display.println(waterLevel);

  // Display turbidity
  display.setCursor(0, 20);
  display.print("Turbidity: ");
  display.println(turbidityValue);

  display.display();
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

    displayData(temperature_Celsius, 0, 0);  // Display temperature on OLED

    delay(10000);
  }
}

void taskWaterLevel(void *pvParameters) {
  while (true) {
    // Read water level
    int waterLevel = analogRead(WaterLevelPin);

    displayData(0, waterLevel, 0);  // Display water level on OLED

    delay(10000);
  }
}

void taskTurbidity(void *pvParameters) {
  while (true) {
    // Read turbidity
    int turbidityValue = analogRead(TurbiditySensorPin);

    displayData(0, 0, turbidityValue);  // Display turbidity on OLED

    delay(10000);
  }
}

void loop() {
  // Empty loop, the main code has been moved to taskWaterTemp, taskWaterLevel, and taskTurbidity
}
