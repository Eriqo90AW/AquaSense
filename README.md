<!-- PROJECT LOGO -->
<br />
<div align="center">
  <h1 align="center">Welcome to AquaSenseHub</h1>
</div>

# I. Introduction to the problem and the solution

The AquaSenseHub project utilizes various sensors to monitor aquarium conditions and ensure the well-being of fish. The DHT11 temperature sensor is employed to check the temperature, a pH sensor measures acidity levels, a water level sensor observes water height, and a Total Dissolved Solids (TDS) sensor assesses water clarity. The automatic feeding involves actuators that can be manually or automatically controlled, connected to an ESP32. Smart notifications are sent via over WiFi when parameters deviate from normal limits. The entire connectivity relies on WiFi to link the ESP32 with the Blynk application. With this integration, AquaSenseHub provides accurate monitoring and responsive notifications, making aquarium maintenance and management more convenient.

## Contributors

This is a final Internet of Things project made by Group A9:

- [Abdul Fikih Kurnia](https://www.github.com/abdfikih) - 2106731200
- [Bintang Marsyuma Rakhasunu](https://www.github.com/marsyuma) - 2106731415
- [Eriqo Arief Wicaksono](https://www.github.com/Eriqo90AW) - 2106638406
- [Leonardo Jeremy Pongpare Munda](https://www.github.com/Leonardojeremy) - 2106707914

## Features
- **Real-time Sensor Monitoring:** Keep an eye on pH levels, water levels, temperature, and turbidity in real-time.
- **User-friendly Interface:** Use the OLED display and Blynk app for an easy-to-understand interface.
- **Remote Control:** Manage the aquarium system remotely through the Blynk app.
- **Automated Fish Feeding:** Feed your fish with a click by activating the servo mechanism through the app.

Explore these features to make your aquarium experience even better!

# II. Hardware design and implementation details

## Schematic

<img width="7634" alt="Diagram Schematic" src="https://github.com/Eriqo90AW/AquaSense/blob/main/diagram-schematic.png??raw=true">


This Project is built using the following technologies:

- **ESP32 Microcontroller:** The brain that integrates sensors and processes data.
- **OLED Module 0.96 (Adafruit_SSD1306):** Displays real-time sensor readings in an easy-to-read format.
- **Blynk App:** Monitor and control the aquarium remotely using the Blynk mobile application.
- **Water Level Sensor Universal:** to measure water levels accurately, ensuring stability in the aquarium environment.
- **Dallas Temperature Sensor (DS18B20):** monitors and regulates water temperature with precision, contributing to a comfortable and conducive habitat for aquatic life.
- **Turbidity Sensor Module (AB147):** utilizes optical sensors to gauge water clarity, assisting in maintaining a clean and healthy aquatic ecosystem.
- **PH Meter Module (PH-4502C):** measures real-time pH levels, ensuring the correct acidity or alkalinity for the optimal well-being of aquarium inhabitants.


The ESP32 Microcontroller serves as the central processing unit, orchestrating the harmonious integration of diverse sensors within AquaSenseHub. This brain of the system efficiently collects and processes data from each sensor, ensuring a comprehensive understanding of the aquarium's conditions. The OLED Module 0.96, powered by the Adafruit_SSD1306 library, acts as the visual interface, displaying real-time sensor readings in a user-friendly format. This display provides aquarists with immediate insights into critical parameters, enhancing their ability to assess and manage the aquarium environment effortlessly.

Among the array of sensors, the Water Level Sensor Universal plays a pivotal role in maintaining aquarium stability. Accurately measuring water levels, this sensor provides indispensable information for preventing overflows or insufficient water levels. The Dallas Temperature Sensor (DS18B20) meticulously monitors and regulates water temperature, creating a comfortable habitat for aquatic life. The Turbidity Sensor Module (AB147), equipped with optical sensors, contributes to the system's functionality by assessing water clarity. This sensor aids in maintaining a pristine aquatic ecosystem by detecting suspended particles and impurities. The PH Meter Module (PH-4502C) is equally crucial, continuously measuring real-time pH levels to ensure the aquarium's optimal acidity or alkalinity, fostering the well-being of its inhabitants. Each sensor, with its specific function, collectively transforms AquaSenseHub into a sophisticated and user-centric smart aquarium system, empowering enthusiasts with both real-time data and actionable insights for effective aquarium management.



# III. Network infrastructure

<img width="7634" alt="Diagram Goals" src="https://github.com/Eriqo90AW/AquaSense/blob/main/diagram-goals.png??raw=true">
The infrastructure of AquaSenseHub is meticulously designed to seamlessly integrate diverse components, ensuring optimal functionality and user engagement. At its core is the ESP32 microcontroller, serving as the central hub that orchestrates data flow and task execution. The OLED Module 0.96, driven by the Adafruit_SSD1306 library, provides a clear visual interface for real-time data presentation. The Blynk App acts as a remote monitoring and control platform, connecting the system to the broader IoT network. Supporting the sensor array, the Water Level Sensor Universal maintains stability, the Dallas Temperature Sensor (DS18B20) regulates water temperature, the Turbidity Sensor Module (AB147) ensures water clarity, and the PH Meter Module (PH-4502C) monitors pH levels. This interconnected infrastructure transforms AquaSenseHub into a sophisticated and user-friendly aquarium monitoring system, seamlessly blending hardware and software components to deliver real-time insights and control capabilities for aquarium enthusiasts.

## Documentation


- **Main Device:**
  
  <img alt="Main Device" src="https://github.com/Eriqo90AW/AquaSense/blob/main/final-device.jpg" height="500">


- **Aquarium and its sensors:**
  
  <img alt="Aquarium and its sensors" src="https://github.com/Eriqo90AW/AquaSense/blob/main/final-aquarium.jpg??raw=true" height="500">


- **Final Assembly:**
  
  <img alt="Final Assembly" src="https://github.com/Eriqo90AW/AquaSense/blob/main/final-assembly.jpg??raw=true" height="500">

- **Mobile User Interface:**
  
  <img alt="Mobile User Interface" src="https://github.com/Eriqo90AW/AquaSense/blob/main/final-ui.jpg??raw=true" height="500">


