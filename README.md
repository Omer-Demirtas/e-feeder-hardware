# Project Goal

An over-engineered (and let's be honest, an engineering porn) solution, developed so that our masters (our cats) don't wake us up at 5 AM screaming "EMERGENCY FOOOOD", allowing us to continue our precious sleep.

This is not just a simple motor spinner. This is an intelligent feeding station that knows its duty even if the Wi-Fi is down, fetches the time from the internet, accepts commands from both Bluetooth and Home Assistant, and is written in C++ based on true OOP principles.

# 🛠️ Tech Stack (What's in the Kitchen?)

    Main Brain: ESP32

    Development Environment: PlatformIO

    Language: C++ (on the Arduino Framework)

    Architecture: Object-Oriented Programming (OOP) & Event-Driven Design

    Libraries:

        PubSubClient (for MQTT)

        BluetoothSerial

        TimeLib / time.h (Time management)

        ArduinoJson (Highly recommended for StorageService)

        Servo or Stepper libraries