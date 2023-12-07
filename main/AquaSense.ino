// #define BLYNK_PRINT Serial // For Blynk. Uncomment it to view connection log, comment it to disable print and save space
// #define _DEBUG_            // For Thinger.io. Uncomment it to view connection log, comment it to disable print and save space
// #define _DISABLE_TLS_      // For Thinger.io. For some reasons, this allows the connection to Thinger.io to work

//Libraries
#include <WiFi.h>
#include <WiFiClient.h>
// #include <BlynkSimpleEsp32.h>
#include <DallasTemperature.h>
// #include <ThingerESP32.h>
#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//#include <Fonts/FreeMono9pt7b.h>

//Definitions
#define tdsEcSen 36                     // TDS EC sensor
#define tempWire 4                        // Temperature sensor
#define turbiSen 39                     // Turbidity sensor
#define pHSen 34                        // pH sensor
#define relay 5                           // Relay pin
#define vRef 3.3                          // ESP32 reference voltage
#define adcRes 4096.0                     // ESP32 ADC resolution
// #define USERNAME "nope"                // Thinger.io username
// #define DEVICE_ID "nope"                 // Thinger.io Device ID
// #define DEVICE_CREDENTIAL "nope"  // Thinger.io Device Credential
#define OLED_RESET -1
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels           

Adafruit_ADS1115 ads;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
OneWire oneWire(tempWire);
DallasTemperature tempSen(&oneWire);
// BlynkTimer timer;
// ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

char auth[] = "nope"; // auth token
char ssid[] = "nope";                  // WiFi SSID.
char pass[] = "nope";                       // WiFi password. Set password to "" for open networks.
float voltsEC, voltsTurbi, voltspH, voltsDO;
float temperature = 25;
float ec = 0, tds = 0, ecCalibration = 1.1500, pH = 0, ntu = 0, turbiOffset = 0.11;
int freq = 2000, channel = 0, resolution = 8;
int tempTrig = 0, tempEmail = 0, alarmPlay = 0;
int pHTrig = 0, pHEmail = 0;
// int dataLog = 0;
int16_t adc0, adc1, adc2, adc3;

float round_to_dp( float in_value, int decimal_place ) {
  float multiplier = powf( 10.0f, decimal_place );
  in_value = roundf( in_value * multiplier ) / multiplier;
  return in_value;
}

// BLYNK_WRITE(V5) {
  // dataLog = param.asInt();
// }

void readADC() {

  adc0 = ads.readADC_SingleEnded(0);
  adc1 = ads.readADC_SingleEnded(1);
  adc2 = ads.readADC_SingleEnded(2);
  adc3 = ads.readADC_SingleEnded(3);

  voltsEC = adc0 * 0.000125;
  voltsTurbi = adc1 * 0.000125 + turbiOffset; // turbiOffset is the offset for turbidity voltage, unless you have the patience to keep tuning the pot...
  voltspH = adc2 * 0.125;
  voltsDO = adc3 * 0.000125;
}

void getTemp() {
  tempSen.requestTemperatures();
  temperature = tempSen.getTempCByIndex(0);
  if (temperature > 40) {
    tempTrig = 1;
    if (tempEmail == 0) {
      tempEmail = 1;
      // Blynk.email("Temperature Alert", String("Recorded Temperature: ") + temperature + String("°C"));
    }
    digitalWrite(relay, LOW);
  }
  else {
    tempTrig = 0;
    tempEmail = 0;
    digitalWrite(relay, HIGH);
  }
  Serial.print("Temp: ");
  Serial.println(temperature);
  // Blynk.virtualWrite(V0, temperature);
}

void getTdsEc() {                // read the analog value more stable by the median filtering algorithm, and convert to voltage value
  float tempCoefficient = 1.0 + 0.02 * (temperature - 25.0);          // temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
  float voltageComp = (voltsEC / tempCoefficient) * ecCalibration;                     // temperature and calibration compensation
  tds = (133.42 * pow(voltageComp, 3) - 255.86 * pow(voltageComp, 2) + 857.39 * voltageComp) * 0.5; // convert voltage value to tds value
  ec = 2 * tds;
  //Serial.println(voltsEC);
  // Sensor Values to Blynk application
  // Blynk.virtualWrite(V1, ec);
}


// equation: https://forum.arduino.cc/t/getting-ntu-from-turbidity-sensor-on-3-3v/658067/14
void getTurbidity() {
  if (voltsTurbi < 1.6913) {
    ntu = 3000;
  }
  else if (voltsTurbi < 2.7720) {
    ntu = -2572.2 * pow(voltsTurbi, 2) + 8700.5 * voltsTurbi - 4352.9;
  }
  else {
    ntu = 0;
  }
  Serial.println(voltsTurbi, 4);
  // Blynk.virtualWrite(V2, ntu);
}


//got problem here
void getpH() {
  pH = 0.000528 * (float)adc2 - 0.25; //c = 0.4628
  // Blynk.virtualWrite(V3, pH);
  if (pH >= 20 || pH <= 3) {
    pHTrig = 1;
    if (pHEmail == 0) {
      pHEmail = 1;
      // Blynk.email("pH Alert", String("Recorded pH: ") + pH);
    }
  }
  else {
    pHTrig = 0;
    pHEmail = 0;
  }
}

void getDO() {

}

void displayUpdate() {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.clearDisplay();
  display.println("T  : " + String(temperature) + (char)247 + "C");
  display.setCursor(0, 10);
  display.println("pH : " + String(pH));
  display.setCursor(0, 20);
  display.println("EC : " + String(ec) + "uS/cm");
  display.setCursor(0, 30);
  display.println("Tur: " + String(voltsTurbi) + "V");

  //display.println("DO  : " + String(temperature) + (char)247 + "C");
  display.display();
}

void allNotif() {
  String notifMessage = "";
  if (tempTrig == 1 || pHTrig == 1) {
    if (tempTrig == 1) {
      notifMessage += " Temperature (" + String(temperature) + "°C) ";
    }
    if (pHTrig == 1) {
      notifMessage += " pH (" + String(pH) + ") ";
    }
    Serial.println(notifMessage);
    // Blynk.notify(String("Abnormal Readings On:") + notifMessage);
  }
  else {

  }
}

void alarmSound() {
  if (tempTrig == 1 || pHTrig == 1) {
    if (alarmPlay == 0) {
      alarmPlay = 1;
      ledcWriteTone(channel, 500);
      ledcWrite(channel, 5);
      delay(10);
    }
    else {
      alarmPlay = 0;
      ledcWriteTone(channel, 500);
      ledcWrite(channel, 0);
      delay(10);
    }
  }
  else {
    ledcWriteTone(channel, 500);
    ledcWrite(channel, 0);
    delay(10);
  }
}

// void thingerSend() {
//   thing["senVal"] >> [] (pson & out) {
//     out["Temperature"] = temperature;
//     out["pH"] = pH;
//     out["EC"] = ec;
//     out["TDS"] = tds;
//     out["Turbidity"] = voltsTurbi;
//   };

//   if (dataLog == 1) {
//     thing.write_bucket("sensorValues", "senVal");
//   }
//   else {

//   }
// }


void setup()
{
  Serial.begin(115200);                   // begin serial monitor, 9600 is recommended for ESP8266 shield setup
  // // Blynk.begin(auth, ssid, pass);  // start Blynk
  // thing.add_wifi(ssid, pass);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(15, channel);
  display.display();
  //display.setFont(&FreeMono9pt7b);
  ads.setGain(GAIN_ONE);
  ads.begin();
  for (int i = 0; i < 3; i++) {
    ledcWrite(channel, 10);
    ledcWriteTone(channel, 1046.50);
    delay(10);
    ledcWrite(channel, 0);
    delay(100);
  }
  display.clearDisplay();
  // timer.setInterval(5000L, readADC);
  // timer.setInterval(5100L, getTemp);
  // timer.setInterval(5200L, getTdsEc);
  // timer.setInterval(5300L, getTurbidity);
  // timer.setInterval(5400L, getpH);
  // timer.setInterval(5500L, getDO);
  // timer.setInterval(5600L, displayUpdate);
  // timer.setInterval(120000L, allNotif);
  // timer.setInterval(2000L, alarmSound);
  // timer.setInterval(60000L, thingerSend);
}

void loop()
{
  getTemp();
  delay(100);
  // Blynk.run();
  // timer.run();
  // thing.handle();

}