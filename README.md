# ESP32-S3Rogue : Rogue AP + DNS Hijacker + MAC Spoofing + Basic Web Server (NAT Placeholder only)
Build with ESP32-S3-N16R8 Microcontroller, read changes !

![image](https://github.com/user-attachments/assets/29bcabf5-b8cc-4c9d-bf4b-20da729424d7)
# Description
Firmware for ESP32-S3 that creates a rogue Wi-Fi AP with MAC spoofing, DNS hijacking, and web control. Designed for headless use in security research and emonstration, and IoT testing. Built with ESP-IDF and supports modular extensions.

# Microcontroller : 
- ESP32-S3-N16R8 /  DevKitC (Non IPEX) -  (Xiaozhi Robot Version)

# Framework use : 
- ESP-IDF v5.3.3
  
# TL;DR
ESP32-S3Rogue: Evil Twin

# Function :
- Rogue Wi-Fi AP creation with custom SSID
- MAC address spoofing via UART input
- DNS hijacking to redirect DNS queries to a fake IP
- Integrated web server for control or phishing
- LED indicator for live status
- NAT Routing for traffic forwarding between AP and STA
- Client connection tracking (AP client status monitoring)

# Details Feature
![image](https://github.com/user-attachments/assets/2c21e87d-ed8c-4de8-a9d8-f8c91d0e95bf)

# How-to:
# Combine Deauther Device for completing the Evil Twin Ops.
- Flahing with VS Code PlatformIO
- App were built with ESP-IDF Framework
- Keep the CMakeLists.txt unedited (Baud should be 115200)
- Once flashed, it require you to enter MAC Address to be spoofed:
  Ignore to maintain ESP32 Default MAC by pressing Enter.
- Monitoring using UART Serial Monitor
- Optional : You can use another project like Deauther to support your WiFi Attack.

# Changelog :  
Libraries & Function :
-  esp_vfs_dev_uart_use_driver (Deprecated Function)
-  esp_netif_nat.h (Deprecated in ESP-IDF v5.3.3)

# Use Cases (for education and research only)
- Ethical hacking labs
- Wi-Fi penetration testing
- IoT/embedded security demonstrations with Microcontroller

Future Development:
- Implementing NAT Routing to prove the Evil Twin Concept.

# Disclaimer:
This project is for educational and authorized security research only. Do not deploy it on networks without permission.

Lyxt
