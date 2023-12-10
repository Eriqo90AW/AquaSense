<!-- PROJECT LOGO -->
<br />
<div align="center">
  <h1 align="center">Welcome to AquaSenseHub</h1>
</div>

The AquaSenseHub project utilizes various sensors to monitor aquarium conditions and ensure the well-being of fish. The DHT11 temperature sensor is employed to check the temperature, a pH sensor measures acidity levels, a water level sensor observes water height, and a Total Dissolved Solids (TDS) sensor assesses water clarity. The automatic feeding involves actuators that can be manually or automatically controlled, connected to an ESP32. Smart notifications are sent via the MQTT protocol over WiFi when parameters deviate from normal limits. The entire connectivity relies on WiFi to link the ESP32 with the Blynk application, transferring data through the MQTT protocol. With this integration, AquaSenseHub provides accurate monitoring and responsive notifications, making aquarium maintenance and management more convenient.

## Contributors

This is a final Internet of Things project made by Group A9:

- [Abdul Fikih Kurnia](https://www.github.com/abdfikih) - 2106731200
- [Bintang Marsyuma Rakhasunu](https://www.github.com/marsyuma) - 2106731415
- [Eriqo Arief Wicaksono](https://www.github.com/Eriqo90AW) - 2106638406
- [Leonardo Jeremy Pongpare Munda](https://www.github.com/Leonardojeremy) - 2106707914

## Installation

## Features
- **Real-time Sensor Monitoring:** Keep an eye on pH levels, water levels, temperature, and turbidity in real-time.
- **User-friendly Interface:** Use the OLED display and Blynk app for an easy-to-understand interface.
- **Remote Control:** Manage the aquarium system remotely through the Blynk app.
- **Automated Fish Feeding:** Feed your fish with a click by activating the servo mechanism through the app.

Explore these features to make your aquarium experience even better!

## Technologies

This Project is built using the following technologies:

- **ESP32 Microcontroller:** The brain that integrates sensors and processes data.
- **OLED Module 0.96 (Adafruit_SSD1306):** Displays real-time sensor readings in an easy-to-read format.
- **Blynk App:** Monitor and control the aquarium remotely using the Blynk mobile application.
- **Water Level Sensor Universal:** to measure water levels accurately, ensuring stability in the aquarium environment.
- **Dallas Temperature Sensor (DS18B20):** monitors and regulates water temperature with precision, contributing to a comfortable and conducive habitat for aquatic life.
- **Turbidity Sensor Module (AB147):** utilizes optical sensors to gauge water clarity, assisting in maintaining a clean and healthy aquatic ecosystem.
- **PH Meter Module (PH-4502C):** measures real-time pH levels, ensuring the correct acidity or alkalinity for the optimal well-being of aquarium inhabitants.

## Schematic

<img width="7634" alt="Diagram Schematic" src="https://github.com/Eriqo90AW/AquaSense/blob/main/diagram-schematic.png??raw=true">

<img width="7634" alt="Diagram Goals" src="https://github.com/Eriqo90AW/AquaSense/blob/main/diagram-goals.png??raw=true">

## Documentation

Link to report progress:

