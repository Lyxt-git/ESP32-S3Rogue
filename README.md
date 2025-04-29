# ESP32-S3Rogue (Without NAT)
Build with ESP32-S3-N16R8 (Xiaozhi Robot Edition) Microcontroller

![ESP32-S3-N16R8-WROOM-1](https://github.com/user-attachments/assets/b01adf0c-3797-4eb9-b8ac-b3c46d89f6f9)
# Description
Firmware for ESP32-S3 that creates a rogue Wi-Fi AP with MAC spoofing, DNS hijacking, and web control. Designed for headless use in security research, CTFs, and IoT testing. Built with ESP-IDF and supports modular extensions.

Microcontroller : ESP32-S3-N16R8 /  DevKitC

ESP32-S3Rogue: Rogue Access Point + DNS Hijacker + MAC Spoofer
ESP32-S3Rogue is a proof-of-concept firmware for the ESP32-S3 that turns the device into a powerful rogue access point. 

It supports:
-Rogue Wi-Fi AP creation with custom SSID
-MAC address spoofing from UART input
-DNS hijacking to redirect all DNS queries to a fake IP
-Integrated web server for control or phishing (optional)
-LED indicator for live status
-Designed for headless operation — no buttons or screens needed

# Details Feature TL;DR
![Capture](https://github.com/user-attachments/assets/eb05fa59-909b-4ca9-9ed8-b5e4545abbcf)

# How-to:
- Flahing with VS Code PlatformIO
- App were built with ESP-IDF Framework
- Keep the CMakeLists.txt unedited (Baud should be 115200)
- Once flashed, it require you to enter MAC Address to be spoofed:
  Ignore to maintain ESP32 Default MAC by pressing Enter.
- Monitoring using UART Serial Monitor
- Optional : You can use another project like Deauther to support your WiFi Attack.

Important Note for depricate UART libraries in Debugging
-  esp_vfs_dev_uart_use_driver

# Use Cases (for education and research only)
-Ethical hacking labs
-Wi-Fi penetration testing
-CTF and red team exercises
-IoT/embedded security demonstrations

Future Development:
- Implementing NAT Routing to prove the Evil Twin Concept.

# Disclaimer:
This project is for educational and authorized security research only. Do not deploy it on networks without permission.

