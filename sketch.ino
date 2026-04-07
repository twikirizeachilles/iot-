#include <WiFi.h>
#include <ESP_Mail_Client.h>

// WiFi credentials
#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PASSWORD "your_wifi_password"

// Gmail SMTP settings
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465
#define SENDER_EMAIL "your_email@gmail.com"
#define SENDER_PASSWORD "your_app_password"   // Gmail App Password
#define RECIPIENT_EMAIL "recipient@gmail.com"

// Pin definitions
const int PIR_PIN = 13;
const int SMOKE_PIN = 34;
const int LIGHT_PIN = 26;
const int BUZZER_PIN = 25;
const int RED_LED_PIN = 27;

// Smoke threshold
const int SMOKE_THRESHOLD = 400;
