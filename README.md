# Single-Cell LiPo Battery Monitor

This project is a compact embedded battery-monitoring system developed using an **ESP32-C3** and **ESP-IDF**. It measures the voltage of a single-cell lithium-polymer (LiPo) battery, classifies the battery state, and provides immediate visual and serial feedback.

For development and testing, a bench power supply is used to simulate the battery. This allows the input voltage to be swept through nominal, undervoltage, and overvoltage conditions in a controlled manner.

## Features

- Measures battery voltage through an analog voltage-sensing circuit and the ESP32-S3 ADC.
- Classifies the measured voltage as **nominal**, **undervoltage**, or **overvoltage**.
- Illuminates a green LED during nominal operation.
- Illuminates a red LED when an under- or overvoltage condition is detected.
- Reports the measured battery voltage through the serial console.
- Separates battery-monitoring logic from the main application for a modular firmware structure.

## System Architecture

The simulated battery voltage is scaled by the sensing circuit before being sampled by the ESP32 ADC. The firmware converts the ADC measurement into an estimated battery voltage, compares it with the configured operating limits, and updates the LEDs and serial output accordingly.

1. **Bench power supply:** Simulates a single-cell LiPo battery.
2. **Voltage-sensing circuit:** Scales the input to a safe ADC voltage.
3. **ESP32-S3 ADC:** Samples the scaled analog voltage.
4. **Battery-monitoring firmware:** Calculates the battery voltage and determines its state.
5. **Status outputs:** Drives the red or green LED and reports the voltage over the serial console.

<!-- Add your architecture notes image to images/architecture-notes.png. -->
![Handwritten system architecture notes](architecture-notes.png)

## Other Embedded and Mechatronics Projects

### Two-Axis Star Tracker

This project uses an **STM32 microcontroller** to control two stepper motors in a two-axis configuration. The mechanism positions a laser pointer toward selected stars in the night sky. I designed the CAD enclosure and developed the electrical system and embedded firmware, giving me end-to-end experience across mechanical design, electronics, motor control, and firmware integration.

<!-- Add the star tracker image to images/star-tracker.jpg. -->
![Two-axis star tracker](star-tracker.heic)

### Automated Wall-Mounted Cat Feeder

This project is an automated, wall-mounted cat feeder built around a custom food-dispensing mechanism. An embedded controller drives a stepper motor to dispense a controlled portion of dry cat food. The mechanical design and dispensing mechanism are complete, and the custom PCB is currently in development.

<!-- Add the cat feeder image to images/cat-feeder.jpg. -->
![Automated wall-mounted cat feeder](cat-feeder.heic)

## Project Status

- **LiPo battery monitor:** Complete and bench-tested.
- **Two-axis star tracker:** Mechanical, electrical, and firmware development complete.
- **Automated cat feeder:** Mechanical design complete; PCB design in progress.
