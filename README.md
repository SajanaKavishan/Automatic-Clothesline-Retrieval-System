# Automatic-Clothesline-Retrieval-System

A smart, sensor-based clothesline system built using **NodeMCU (ESP8266)** and controlled via the **Blynk IoT** platform. It automatically extends or retracts the clothesline based on weather conditions like temperature, humidity, light, and rain detection. Manual control is also available through the Blynk app.

---

## 🚀 Features

- 🧠 **Automatic Motor Control** based on:
  - Rain detection
  - Light level (sunny/dark)
  - Temperature and humidity thresholds
  - IR object detection (clothes presence)
- 🖐️ **Manual Control** via Blynk Virtual Pin V8
- 📲 **Real-Time Feedback** to Blynk:
  - Temperature & Humidity (V3 & V4)
  - Clothesline Status: Extended/Retracted/Stopped (V6)
  - Weather Status: Sunny / Rainy / Cloudy / Dark (V7)
- 🛠️ **Motor Safety & Error Handling**
  - Motor stops automatically after 4 seconds(It depends on how long it takes for your clothesline to get under a roof)
  - Blinks LED on motor failure
- 🧪 Sensor Integration:
  - DHT11 (Temp & Humidity)
  - Rain Sensor (D1)
  - LDR (D5)
  - IR Sensor (A0)

---

## 📷 System Overview

- **Microcontroller**: NodeMCU ESP8266
- **Platform**: Blynk IoT (App + Cloud)
- **Programming Language**: Arduino (C++)
- **Notifications**: Blynk Virtual Pins
- **Motor Control Pins**: D6, D7, EN1 (D3)
- **Indicator LED**: D8

---

## 🧰 Wiring (Pin Configuration)

| Component          | ESP8266 Pin |
|--------------------|-------------|
| DHT11              | D2          |
| Rain Sensor        | D1 (digital)|
| LDR (Light Sensor) | D5 (digital)|
| IR Sensor          | A0 (analog) |
| Motor EN           | D3          |
| Motor IN1          | D6          |
| Motor IN2          | D7          |
| LED Indicator      | D8          |

---

## 📄 Blynk Setup

Set up these Virtual Pins in the Blynk mobile app:

| Virtual Pin | Purpose                        |
|-------------|--------------------------------|
| V3          | Temperature (°C)               |
| V4          | Humidity (%)                   |
| V6          | Clothesline Status             |
| V7          | Weather Status                 |
| V8          | Manual Control (Button: 0/1)   |

---

First Year First Semester Project | IoT Smart Systems | Mobile + Hardware Integration
