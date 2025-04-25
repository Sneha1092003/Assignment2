## ASSIGNMENT 2

# LCD control module with ESP32 (16x2 display)

✅OBJECTIVE
* TO create a module driver in ESP-IDF to control LCD display (16x2) using GPIO AND API timers, without any prebuilt libraries.

🛠️ Requirements
* Use ESP-IDF framework (v4.x or v5.x is fine)
* LCD interface: 4-bit mode or 8-bit mode (your choice)
* Do not use any ready-made libraries (like hd44780, etc.)
* Use only GPIO, FreeRTOS, and timers as needed

⚙️HOW TO WIRE THE LCD

🔌RS → GPIO 21 

🔌EN → GPIO 22 

🔌R/W → GND

🔌D4 → GPIO 15

🔌D5 → GPIO 23

🔌D6 → GPIO 18

🔌D7 → GPIO 19

🔌VCC → 3V3

🔌GND → GND
