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

<img width="7634" alt="Diagram Schematic" src="https://github.com/Eriqo90AW/AquaSense/blob/main/assets/diagram-schematic.png??raw=true">

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

<img width="7634" alt="Diagram Goals" src="https://github.com/Eriqo90AW/AquaSense/blob/main/assets/diagram-goals.png??raw=true">
The infrastructure of AquaSenseHub is meticulously designed to seamlessly integrate diverse components, ensuring optimal functionality and user engagement. At its core is the ESP32 microcontroller, serving as the central hub that orchestrates data flow and task execution. The OLED Module 0.96, driven by the Adafruit_SSD1306 library, provides a clear visual interface for real-time data presentation. The Blynk App acts as a remote monitoring and control platform, connecting the system to the broader IoT network. Supporting the sensor array, the Water Level Sensor Universal maintains stability, the Dallas Temperature Sensor (DS18B20) regulates water temperature, the Turbidity Sensor Module (AB147) ensures water clarity, and the PH Meter Module (PH-4502C) monitors pH levels. This interconnected infrastructure transforms AquaSenseHub into a sophisticated and user-friendly aquarium monitoring system, seamlessly blending hardware and software components to deliver real-time insights and control capabilities for aquarium enthusiasts.

# IV. Software Implementation Details

The AquaSenseHub code is the epitome of precision in orchestrating an aquarium monitoring and control system, leveraging the ESP32 microcontroller. Its intricate architecture seamlessly integrates diverse hardware components, including sensors, an OLED display, Blynk for remote control, and a servo motor for automating fish feeding. The code's structure, governed by the FreeRTOS multitasking library, meticulously organizes tasks for concurrent execution, ensuring real-time management of aquarium operations.

At the core of this intelligent system is the pivotal role played by Blynk. The initiation of the Blynk connection establishes a robust communication channel with the server, facilitated by a specific authentication token and Wi-Fi credentials. The BLYNK_WRITE(V4) function, operating as a Blynk callback, actively responds to user input on Virtual Pin 4, triggering the activateServo() function. This enables users to exert manual control over the servo through the Blynk application, adding a layer of interactivity to the system.

The AquaSenseHub extends its capabilities with the integration of multiple sensors, coordinated by tasks such as taskWaterTemp, taskWaterLevel, taskTurbidity, and taskPhSensor. Operating independently as FreeRTOS tasks, they efficiently collect data from sensors like the Dallas Temperature sensor, water level sensor, turbidity sensor, and pH sensor, ensuring real-time insights into the aquarium's vital statistics. The taskOled function manages the OLED display, offering dynamic real-time information by negotiating access to shared resources through semaphores. Servo control, orchestrated by the ESP32 Servo library, is enhanced with a sophisticated timer mechanism. The servoTimer, created with FreeRTOS functions, triggers the servoTimerCallback function at consistent intervals, ensuring an automated and scheduled fish feeding. The AquaSenseHub code exemplifies a sophisticated and modular approach to aquarium monitoring and control, seamlessly integrating sensor data, Blynk for remote accessibility, OLED display for on-site insights, servo control for automated fish feeding, and a meticulously crafted timer mechanism. This comprehensive solution sets a new standard for aquarium management, offering advanced automation and redefining the experience for aquarium enthusiasts.

# V. Test Results and Performance Evaluation

During the smart aquarium system's testing phase, initial attention was directed at establishing connections with four vital sensors: pH, water level, temperature, and turbidity. These sensors were integrated with the ESP32 device, and their numerical readings were monitored through the Serial Monitor. This ensured the accuracy of data received by the ESP32, aligning it with real environmental conditions.

The subsequent testing phase focused on evaluating WiFi connectivity and Blynk integration. Rigorous examination encompassed the establishment and stability of the WiFi connection, emphasizing indicators like connection status and signal strength. Concurrently, Blynk integration underwent scrutiny to assess seamless interaction with the Blynk platform for remote monitoring and control. Success indicators, including Blynk app interfaces and relevant logs, were captured through screenshots and visuals, affirming the system's capability to reliably transmit sensor data and provide a user-friendly experience. These positive outcomes underscore a robust foundation, enhancing system reliability and user accessibility as it progresses in development and refinement.

## Serial Monitor

<img height="300" alt="Serial Monitor" src="https://github.com/Eriqo90AW/AquaSense/blob/main/assets/serial-1.png??raw=true">

## Display

<img height="200" alt="Display Monitor" src="https://github.com/Eriqo90AW/AquaSense/blob/main/assets/serial-2.png??raw=true">

## Blynk

<img height="500" alt="Display Blynk" src="https://github.com/Eriqo90AW/AquaSense/blob/main/assets/blynk-app.png??raw=true">

## Circuit

<img height="500" alt="Diagram Circuit" src="https://github.com/Eriqo90AW/AquaSense/blob/main/assets/final-device.png??raw=true">

# VI. Conclusion and Future Work

In conclusion, the development of AquaSenseHub marks a significant milestone in creating an advanced and modular aquarium monitoring and control system. The integration of ESP32, diverse sensors, Blynk for remote control, and a servo motor for automated feeding showcases a sophisticated approach to aquarium management. The code's organization into FreeRTOS tasks ensures efficient multitasking and parallel execution, contributing to real-time monitoring of critical parameters.

Looking ahead, future work could explore enhancements and additional features for AquaSenseHub. Integration with more sensors, such as ammonia or dissolved oxygen sensors, could provide a more comprehensive understanding of the aquarium environment. Improved data visualization and analysis tools could be implemented to offer users deeper insights into trends and fluctuations. Additionally, exploring energy-efficient mechanisms and expanding compatibility with other home automation platforms could enhance AquaSenseHub's overall sustainability and usability.

Continuous testing and refinement will be essential to ensure the system's robustness and reliability in diverse aquarium setups. User feedback will play a crucial role in refining the user interface and experience, tailoring AquaSenseHub to meet the evolving needs of aquarium enthusiasts. Ultimately, AquaSenseHub stands as a versatile and extensible solution, with future iterations poised to redefine the standards of smart aquarium technology.

## Documentation

- **Main Device:**

  <img alt="Main Device" src="https://github.com/Eriqo90AW/AquaSense/blob/main/assets/final-device.jpg" height="500">

- **Aquarium and its sensors:**

  <img alt="Aquarium and its sensors" src="https://github.com/Eriqo90AW/AquaSense/blob/main/assets/final-aquarium.jpg??raw=true" height="500">

- **Final Assembly:**

  <img alt="Final Assembly" src="https://github.com/Eriqo90AW/AquaSense/blob/main/assets/final-assembly.jpg??raw=true" height="500">

- **Mobile User Interface:**

  <img alt="Mobile User Interface" src="https://github.com/Eriqo90AW/AquaSense/blob/main/assets/final-ui.jpg??raw=true" height="500">
