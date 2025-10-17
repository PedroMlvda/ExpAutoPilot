#pragma once
#include <Arduino.h>

// Pins
constexpr uint8_t PIN_LOCK_ALT   = 2;   // Interrupteur Lock Altitude (INPUT_PULLUP, actif LOW)
constexpr uint8_t PIN_LOCK_HDG   = 8;   // Interrupteur Lock Heading (INPUT_PULLUP, actif LOW)
constexpr uint8_t PIN_ALT_PLUS   = 4;   // Poussoir + Altitude (INPUT_PULLUP, actif LOW)
constexpr uint8_t PIN_ALT_MINUS  = 3;   // Poussoir - Altitude (INPUT_PULLUP, actif LOW)
constexpr uint8_t PIN_HDG_PLUS   = 6;   // Poussoir Trim + Pitch (INPUT_PULLUP, actif LOW)
constexpr uint8_t PIN_HDG_MINUS  = 7;   // Poussoir Trim - Pitch (INPUT_PULLUP, actif LOW)

// Servos
constexpr uint8_t PIN_SERVO_PITCH = 10;
constexpr uint8_t PIN_SERVO_ROLL  = 11; // réservé

// I2C
constexpr uint32_t I2C_FREQ_HZ = 400000; // 400 kHz

// UI / Control rates
constexpr uint8_t  UI_FPS  = 10;   // 10 FPS (toutes les 100 ms)
constexpr uint16_t CTRL_HZ = 100;  // 100 Hz (toutes les 10 ms)

// Debounce
constexpr uint16_t DEBOUNCE_MS = 15;

// PID Pitch
constexpr float KpPitch = 1.2f;
constexpr float KiPitch = 0.05f;
constexpr float KdPitch = 0.8f;

// Servo Pitch bounds
constexpr int SERVO_PITCH_NTR = 90;
constexpr int SERVO_PITCH_MIN = 20;
constexpr int SERVO_PITCH_MAX = 140;

// Startup
constexpr uint16_t STARTUP_PROGRESS_MS = 5000; // 5 s

// --- FILTRE ALTITUDE ---
// Filtre EMA sur l'altitude GPS (en pieds) avant quantification
// currentAlt = round( (EMA(alt_ft)) / QUANT_FT ) * QUANT_FT
constexpr float ALT_EMA_ALPHA = 0.20f;   // 0.1..0.3 typique (0.20 recommandé)
constexpr int   ALT_QUANT_FT  = 50;      // quantification à 50 ft (changer à 10 si besoin)
