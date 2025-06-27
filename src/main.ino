#define BLYNK_TEMPLATE_ID "Add your Blynk template ID"
#define BLYNK_TEMPLATE_NAME "Add your Blynk template name"

#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

const char auth[] PROGMEM = "Add your Blynk auth token";
const char ssid[] PROGMEM = "Add your Wi-Fi network name";
const char pass[] PROGMEM = "Add your Wi-Fi password";

#define BLYNK_PRINT Serial
#define DHTPIN D2
#define DHTTYPE DHT11
#define LEDPIN D8
#define EN1 D3 

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

#define MOTOR_STOP 0
#define MOTOR_EXTEND 1
#define MOTOR_RETRACT 2

int motorState = MOTOR_STOP;
unsigned long motorStopTime = 0;
const unsigned long motorRunTime PROGMEM = 4000;
const float tempThreshold PROGMEM = 32.0;
const float humidityExtendThreshold PROGMEM = 95.0;
const float humidityRetractThreshold PROGMEM = 95.0;

bool extended = false;
bool retracted = false;

void setup() {
    pinMode(D1, INPUT); 
    pinMode(A0, INPUT);  
    pinMode(D5, INPUT);  
    pinMode(D6, OUTPUT); 
    pinMode(D7, OUTPUT); 
    pinMode(LEDPIN, OUTPUT);
    pinMode(EN1, OUTPUT); 

    dht.begin();
    Serial.begin(9600);

    char authBuffer[sizeof(auth)];
    strcpy_P(authBuffer, auth);
    char ssidBuffer[sizeof(ssid)];
    strcpy_P(ssidBuffer, ssid);
    char passBuffer[sizeof(pass)];
    strcpy_P(passBuffer, pass);

    Blynk.begin(authBuffer, ssidBuffer, passBuffer);
    timer.setInterval(1000L, sendSensorData);
    timer.setInterval(500L, controlMotor);
    timer.setInterval(2000L, checkErrors); 
}

void sendSensorData() {
    int rain = digitalRead(D1);
    int light = digitalRead(D5);
    float t = dht.readTemperature();
    float h = dht.readHumidity();

    Blynk.virtualWrite(V3, t);
    Blynk.virtualWrite(V4, h);
    Blynk.virtualWrite(V6, motorState == MOTOR_EXTEND ? F("Extended") : (motorState == MOTOR_RETRACT ? F("Retracted") : F("Stopped")));

    String weatherStatus;
    if (light == LOW) {
        weatherStatus = F("Sunny");
    } else if (h >= humidityExtendThreshold) {
        weatherStatus = F("Cloudy");
    } else if (rain == LOW) {
        weatherStatus = F("Rainy");
    } else {
        weatherStatus = F("Dark");
    }
    Blynk.virtualWrite(V7, weatherStatus);

    Serial.print(F("Rain: "));
    Serial.print(rain);
    Serial.print(F(" Light: "));
    Serial.print(light);
    Serial.print(F(" Temperature: "));
    Serial.print(t);
    Serial.print(F(" Humidity: "));
    Serial.println(h);  
}

BLYNK_WRITE(V8) {
    int manualControl = param.asInt();
    if (manualControl == 1 && motorState == MOTOR_STOP) {
        motorState = MOTOR_RETRACT;
        motorStopTime = millis() + motorRunTime;
        digitalWrite(D1, HIGH); 
        digitalWrite(D6, LOW);
        digitalWrite(D7, HIGH);
        retracted = true;
        extended = false;
    } else if (manualControl == 0 && motorState == MOTOR_STOP) {
        motorState = MOTOR_EXTEND;
        motorStopTime = millis() + motorRunTime;
        digitalWrite(D1, HIGH); 
        digitalWrite(D6, HIGH);
        digitalWrite(D7, LOW);
        extended = true;
        retracted = false;
    }
}

void controlMotor() {
  int rain = digitalRead(D1);
  int light = digitalRead(D5);
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  int ir = analogRead(A0);

  Serial.print(F("Rain: "));
  Serial.print(rain);
  Serial.print(F(" Temperature: "));
  Serial.print(t);
  Serial.print(F(" Humidity: "));
  Serial.println(h);

  if (ir < 512) {
    if (!extended && h < humidityExtendThreshold && light == LOW && rain == HIGH && t < tempThreshold) {
      if (motorState == MOTOR_STOP) {
        motorState = MOTOR_EXTEND;
        motorStopTime = millis() + motorRunTime;
        Serial.println("Extending clothesline");
        extended = true;
        retracted = false;
        digitalWrite(EN1, HIGH); 
      }
    } else if (!retracted && (h >= humidityRetractThreshold || light == HIGH || rain == LOW || t >= tempThreshold)) {
      if (motorState == MOTOR_STOP) {
        motorState = MOTOR_RETRACT;
        motorStopTime = millis() + motorRunTime;
        Serial.println("Retracting clothesline");
        retracted = true;
        extended = false;
        digitalWrite(EN1, HIGH); 
      }
    }

    if (millis() <= motorStopTime) {
      if (motorState == MOTOR_EXTEND) {
        digitalWrite(D6, LOW);
        digitalWrite(D7, HIGH);
      } else if (motorState == MOTOR_RETRACT) {
        digitalWrite(D6, HIGH);
        digitalWrite(D7, LOW);
      }
    } else {
      digitalWrite(EN1, LOW); 
      digitalWrite(D6, LOW);
      digitalWrite(D7, LOW);
      if (motorState != MOTOR_STOP) {
        motorState = MOTOR_STOP;
        Serial.println("Motor stopped");
      }
    }
  } else {
    digitalWrite(EN1, LOW); 
    digitalWrite(D6, LOW);
    digitalWrite(D7, LOW);
    motorState = MOTOR_STOP;
    Serial.println("IR sensor not detecting any object. Motor stopped.");
  }
}

void checkErrors() {
    if (digitalRead(D6) == LOW && digitalRead(D7) == LOW && motorState != MOTOR_STOP) {
        Serial.println("Error: Motor not responding. Blinking LED.");
        blinkLED();
    }
}

void blinkLED() {
    digitalWrite(LEDPIN, HIGH);
    delay(250);
    digitalWrite(LEDPIN, LOW);
    delay(250);
    digitalWrite(LEDPIN, HIGH);
    delay(250);
    digitalWrite(LEDPIN, LOW);
}

void loop() {
    Blynk.run();
    timer.run();
}
