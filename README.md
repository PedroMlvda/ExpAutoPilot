# ExpAutoPilot
Experimental Aircraft AutoPilot based on 





<img width="1489" height="530" alt="Screenshot 2025-10-17 155910" src="https://github.com/user-attachments/assets/928289d9-8016-4eee-9ee8-bc06b124014f" />

Experimental Aircraft AutoPilot
Overview
This project is the first iteration of an autopilot system designed for experimental aircraft applications. It runs on an Arduino UNO R4 WiFi and integrates:

GPS-based altitude and heading monitoring
Servo-controlled pitch adjustments
A compact OLED display interface for real-time feedback

The autopilot is intended to control RC servos connected to 3D-printed trim tabs, which are available in the 3D Print section of this repository.

Current Status
This is an early-stage implementation, and not all features are fully operational yet:

Altitude Lock is fully functional:

Pressing the LockAlt button captures the current altitude and engages PID pitch control.
The OLED immediately displays the locked altitude with a padlock icon.


Heading Lock is partially implemented:

The display shows heading lock status, but the actual heading-hold control logic is not yet active.
Future versions will include a PID for roll control based on locked heading.

<img width="896" height="553" alt="Screenshot 2025-10-17 155957" src="https://github.com/user-attachments/assets/3d267740-64ab-4fd9-b5bd-a4d0b0a2e489" />
<img width="1489" height="530" alt="Screenshot 2025-10-17 155910" src="https://github.com/user-attachments/assets/928289d9-8016-4eee-9ee8-bc06b124014f" />

Key Features

Startup Screen: Displays GPS and control checks, followed by a progress bar before operational mode.
Main UI:

Line 1: Current altitude (ft) and heading (°)
Line 2: Altitude lock (left) and heading lock (right)
Line 3: Servo positions (Pitch and Roll placeholder)
Line 4: Status message (default: Autopilot : Ready.)


Altitude Lock: PID pitch control at 100 Hz
GPS Altitude Filtering: EMA smoothing + quantization for stability


Hardware Requirements

Arduino UNO R4 WiFi
OLED SSD1309 (I²C)
GPS module (NMEA, TinyGPS++)
Servo motor for pitch control
Push buttons for LockAlt, LockHdg, Alt+, Alt-, Trim+, Trim-
Common ground and stable 5V supply (external recommended for servo)


Wiring Summary

OLED (I²C): SDA → A4, SCL → A5, VCC → 5V, GND → GND
GPS: TX → D0 (RX1), RX → D1 (TX1), VCC → 5V, GND → GND
Servo Pitch: Signal → D10, VCC → 5V, GND → GND
Buttons: D2/D8/D4/D3/D6/D7 → button → GND (INPUT_PULLUP)


Next Steps

Implement heading-hold PID for roll control
Add servo speed limiting for smoother transitions
Introduce debug logging and telemetry options



