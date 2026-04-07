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

// Cooldown to avoid repeated emails (ms)
const unsigned long EMAIL_COOLDOWN = 30000;
unsigned long lastSmokEmail = 0;
unsigned long lastMotionEmail = 0;

SMTPSession smtp;

void sendEmail(String subject, String body) {
  ESP_Mail_Session session;
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = SENDER_EMAIL;
  session.login.password = SENDER_PASSWORD;
  session.login.user_domain = "";

  SMTP_Message message;
  message.sender.name = "ESP32 Alert System";
  message.sender.email = SENDER_EMAIL;
  message.subject = subject;
  message.addRecipient("Recipient", RECIPIENT_EMAIL);
  message.text.content = body.c_str();

  if (!smtp.connect(&session)) {
    Serial.println("SMTP connection failed!");
    return;
  }

  if (!MailClient.sendMail(&smtp, &message)) {
    Serial.println("Email send failed: " + smtp.errorReason());
  } else {
    Serial.println("Email sent successfully!");
  }

  smtp.closeSession();
}

void setup() {
  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT);
  pinMode(LIGHT_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);

  // Connect to WiFi
  Serial.print("Connecting to WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected! IP: " + WiFi.localIP().toString());
}

void loop() {
  int motionDetected = digitalRead(PIR_PIN);
  int smokeLevel = analogRead(SMOKE_PIN);
  unsigned long now = millis();

  // Motion detection - control light
  if (motionDetected == HIGH) {
    digitalWrite(LIGHT_PIN, HIGH);
    Serial.println("Motion detected - Light ON");

    // Send email alert with cooldown
    if (now - lastMotionEmail > EMAIL_COOLDOWN) {
      sendEmail(
        "⚠️ Motion Alert!",
        "Motion has been detected by your ESP32 sensor system. The light has been turned ON."
      );
      lastMotionEmail = now;
    }
  } else {
    digitalWrite(LIGHT_PIN, LOW);
  }

  // Smoke detection - trigger alarm
  if (smokeLevel > SMOKE_THRESHOLD) {
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(RED_LED_PIN, HIGH);
    Serial.print("Smoke detected! Level: ");
    Serial.println(smokeLevel);

    // Send email alert with cooldown
    if (now - lastSmokEmail > EMAIL_COOLDOWN) {
      sendEmail(
        "🚨 Smoke Alert!",
        "Smoke has been detected! Smoke level: " + String(smokeLevel) + ". Please check immediately!"
      );
      lastSmokEmail = now;
    }
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(RED_LED_PIN, LOW);
  }

  delay(200);
}


