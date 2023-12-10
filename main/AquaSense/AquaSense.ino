#define BLYNK_TEMPLATE_ID "TMPL6Nh6NAxPw"
#define BLYNK_TEMPLATE_NAME "AquaSense"
#define BLYNK_AUTH_TOKEN "PlJiWgE-LnpQNUdWpvuFl5LjVdlaBenw"

#define BLYNK_PRINT Serial

#include <Arduino.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <Adafruit_SSD1306.h>
#include <BlynkSimpleEsp32.h>

TaskHandle_t TaskHandleTemp;
TaskHandle_t TaskHandleLevel;
TaskHandle_t TaskHandleTurbid;
TaskHandle_t TaskHandlePh;
TaskHandle_t TaskHandleOled;
TaskHandle_t TaskHandleServo;

TimerHandle_t servoTimer;

QueueHandle_t temperatureQueue;
QueueHandle_t waterLevelQueue;
QueueHandle_t turbidityQueue;
QueueHandle_t phQueue;

SemaphoreHandle_t temperatureSemaphore;
SemaphoreHandle_t waterLevelSemaphore;
SemaphoreHandle_t turbiditySemaphore;
SemaphoreHandle_t phSemaphore;
SemaphoreHandle_t arbitrator;

char auth[] = "PlJiWgE-LnpQNUdWpvuFl5LjVdlaBenw";

const char *ssid = "abdul";
const char *password = "12345678";

const int SensorDataPin = 18;
const int WaterLevelPin = 34;
const int TurbiditySensorPin = 35;
const int PhSensorPin = 32;
const int LEDPin = 5;

const int delayTime = 500;

float tempWater = 0.0;
int levelWater = 0;
float levelTurbi = 0.0;
float levelPh = 0.0;

struct SensorData {
  float temperature;
  int waterLevel;
  float turbidity;
  float pH;
};

OneWire oneWire(SensorDataPin);
DallasTemperature sensors(&oneWire);
Servo myservo;

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

BLYNK_WRITE(V4) {
  int servoControl = param.asInt();
  if (servoControl == 1) {
    activateServo();
  }
}

void activateServo() {
  // Kontrol servo di sini sesuai dengan kebutuhan
  myservo.write(90);  // Contoh: Menggerakkan servo ke posisi 90 derajat
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  myservo.write(0);   // Menggerakkan servo kembali ke posisi awal
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  // Tambahkan logika kontrol servo sesuai dengan aplikasi Anda
}


void servoTimerCallback(TimerHandle_t xTimer) {
  activateServo();
}

void taskServo(void *pvParameters) {
  servoTimer = xTimerCreate(
      "ServoTimer",
      pdMS_TO_TICKS(5000),  
      pdTRUE,
      (void *)0,
      servoTimerCallback);

  if (servoTimer != NULL) {
    xTimerStart(servoTimer, 0);
    Serial.println("Timer created successfully");
  } else {
    Serial.println("Failed to create timer");
  }

  while (true) {
    // Tugas servo lainnya di sini

    vTaskDelay(500);
  }
}

bool requestResourceAccess(SemaphoreHandle_t resourceSemaphore, QueueHandle_t *queue, SensorData *data) {
  if (xSemaphoreTake(arbitrator, portMAX_DELAY) == pdPASS) {
    if (xSemaphoreTake(resourceSemaphore, portMAX_DELAY) == pdPASS) {
      if (xQueueReceive(*queue, data, 0) == pdPASS) {
        xSemaphoreGive(resourceSemaphore);
        xSemaphoreGive(arbitrator);
        return true; // Akses diberikan
      }
      xSemaphoreGive(resourceSemaphore);
    }
    else {
    }
    xSemaphoreGive(arbitrator);
  }
  else {
    Serial.println("Failed to acquire arbitrator.");
  }
  return false; // Gagal mendapatkan akses
}


void releaseResourceAccess(SemaphoreHandle_t resourceSemaphore) {
  xSemaphoreGive(resourceSemaphore);
}

void taskOled(void *pvParameters) {
  SensorData data;

  while (true) {
    display.clearDisplay();

    // Display title
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("AQUASENSE");

    // Read data from queues
    if (requestResourceAccess(temperatureSemaphore, &temperatureQueue, &data)) {
      tempWater = data.temperature;
      Serial.println(tempWater);
      releaseResourceAccess(temperatureSemaphore);
    }

    if (requestResourceAccess(waterLevelSemaphore, &waterLevelQueue, &data)) {
      levelWater = data.waterLevel;
      releaseResourceAccess(waterLevelSemaphore);
    }

    if (requestResourceAccess(turbiditySemaphore, &turbidityQueue, &data)) {
      levelTurbi = data.turbidity;
      releaseResourceAccess(turbiditySemaphore);
    }

    if (requestResourceAccess(phSemaphore, &phQueue, &data)) {
      levelPh = data.pH;
      releaseResourceAccess(phSemaphore);
    }

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
  QueueHandle_t *queue = (QueueHandle_t *)pvParameters;

  pinMode(5, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(23, OUTPUT);

  while (true) {
    sensors.requestTemperatures();
    delay(100);  // Tambahkan delay di sini

    float temperature_Celsius = sensors.getTempCByIndex(0);

    if (temperature_Celsius > 32.0) {
      digitalWrite(LEDPin, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(19, LOW);
      digitalWrite(23, LOW);
    } else if (temperature_Celsius > 30.0 && temperature_Celsius <= 32.0) {
      digitalWrite(5, LOW);
      digitalWrite(19, HIGH);
      digitalWrite(23, LOW);
    } else {
      digitalWrite(5, LOW);
      digitalWrite(19, LOW);
      digitalWrite(23, HIGH);
    }

    SensorData data;
    data.temperature = temperature_Celsius;

    Serial.println("ini " + String(data.temperature));

    if (xQueueSend(*queue, &data, portMAX_DELAY) != pdPASS) {
      Serial.println("Queue is full! Data lost.");
    }

    Blynk.virtualWrite(V0, String(temperature_Celsius)); // Tambahkan Blynk.virtualWrite untuk suhu

    vTaskDelay(100);  
  }
}


void taskWaterLevel(void *pvParameters) {
  QueueHandle_t *queue = (QueueHandle_t *)pvParameters;

  while (true) {
    int waterLevel = analogRead(WaterLevelPin);
    levelWater = map(waterLevel, 1500, 4095, 0, 100);
    if (levelWater < 0) {
      levelWater = 0;
    }

    SensorData data;
    data.waterLevel = levelWater;

    if (requestResourceAccess(waterLevelSemaphore, queue, &data)) {
      if (xQueueSend(*queue, &data, portMAX_DELAY) != pdPASS) {
        Serial.println("Queue is full! Data lost.");
      }
      releaseResourceAccess(waterLevelSemaphore);
    }

    Blynk.virtualWrite(V1, String(levelWater)); // Tambahkan Blynk.virtualWrite untuk level air

    vTaskDelay(100);
  }
}

void taskTurbidity(void *pvParameters) {
  QueueHandle_t *queue = (QueueHandle_t *)pvParameters;

  while (true) {
    int turbidityValue = analogRead(TurbiditySensorPin);
    levelTurbi = turbidityValue;

    SensorData data;
    data.turbidity = levelTurbi;

    if (requestResourceAccess(turbiditySemaphore, queue, &data)) {
      if (xQueueSend(*queue, &data, portMAX_DELAY) != pdPASS) {
        Serial.println("Queue is full! Data lost.");
      }
      releaseResourceAccess(turbiditySemaphore);
    }

    Blynk.virtualWrite(V2, String(levelTurbi)); // Tambahkan Blynk.virtualWrite untuk turbidity

    vTaskDelay(100);
  }
}

void taskPhSensor(void *pvParameters) {
  QueueHandle_t *queue = (QueueHandle_t *)pvParameters;

  while (true) {
    int phValue = analogRead(PhSensorPin);
    levelPh = convertToPhLevel(phValue);

    SensorData data;
    data.pH = levelPh;

    if (requestResourceAccess(phSemaphore, queue, &data)) {
      if (xQueueSend(*queue, &data, portMAX_DELAY) != pdPASS) {
        Serial.println("Queue is full! Data lost.");
      }
      releaseResourceAccess(phSemaphore);
    }

    Blynk.virtualWrite(V3, String(levelPh)); // Tambahkan Blynk.virtualWrite untuk pH

    vTaskDelay(100);
  }
}

float convertToPhLevel(int analogValue) {
  return (analogValue * (14.0 - 0.0) / 4095.0 + 0.0) - 3;
}

void setup() {
  Serial.begin(115200);

  connectToWiFi();

  Blynk.begin(auth, ssid, password);

  sensors.begin();
  pinMode(LEDPin, OUTPUT);

  myservo.attach(13);  // Attach servo ke pin 13

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    return;
  }

  display.display();
  delay(1000);

  // Initialize queues
  temperatureQueue = xQueueCreate(10, sizeof(SensorData));
  waterLevelQueue = xQueueCreate(10, sizeof(SensorData));
  turbidityQueue = xQueueCreate(10, sizeof(SensorData));
  phQueue = xQueueCreate(10, sizeof(SensorData));

  // Initialize semaphores
  temperatureSemaphore = xSemaphoreCreateMutex();
  waterLevelSemaphore = xSemaphoreCreateMutex();
  turbiditySemaphore = xSemaphoreCreateMutex();
  phSemaphore = xSemaphoreCreateMutex();
  arbitrator = xSemaphoreCreateMutex();

  vTaskDelay(100);


  xTaskCreatePinnedToCore(
      taskWaterLevel,
      "taskWaterLevel",
      10000,
      (void *)&waterLevelQueue,
      1,
      &TaskHandleLevel,
      1);

    xTaskCreatePinnedToCore(
      taskWaterTemp,
      "taskWaterTemp",
      10000,
      (void *)&temperatureQueue,
      2,
      &TaskHandleTemp,
      1);


  xTaskCreatePinnedToCore(
      taskTurbidity,
      "taskTurbidity",
      10000,
      (void *)&turbidityQueue,
      2,
      &TaskHandleTurbid,
      0);

  xTaskCreatePinnedToCore(
      taskPhSensor,
      "taskPhSensor",
      10000,
      (void *)&phQueue,
      1,
      &TaskHandlePh,
      0);

  xTaskCreatePinnedToCore(
      taskOled,
      "taskOled",
      10000,
      NULL,
      0,
      &TaskHandleOled,
      0);
  
  xTaskCreatePinnedToCore(
      taskServo,
      "taskServo",
      10000,
      NULL,
      3,
      &TaskHandleServo,
      0);

  
  vTaskDelete(NULL);
}

void loop() {
  Blynk.run();
}
