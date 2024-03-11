#define BLYNK_TEMPLATE_ID   "user3"
#define BLYNK_TEMPLATE_NAME "user3@server.wyns.it"


#include <DHT.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define DHTPIN 23         // Pin voor de DHT11-sensor
#define DHTTYPE DHT11     // Type DHT-sensor
#define LED_PIN 21        // Pin voor de LED van de verwarmingsketel
#define POT_PIN 32        // Pin voor de potentiometer

DHT dht(DHTPIN, DHTTYPE);

char auth[] = "sio4lZS9vP_Ainu_PNk0lahIhhXWJX09"; // Vervang dit door jouw auth token
char ssid[] = "embed";  // Vervang dit door jouw WiFi SSID
char pass[] = "weareincontrol";  // Vervang dit door jouw WiFi wachtwoord

int desiredTemp = 0;   // Initieel ingestelde gewenste temperatuur
int currentTemp = 0;   // Variabele voor de gemeten temperatuur
int potValue = 0;      // Variabele voor de waarde van de potentiometer

BlynkTimer timer;

void setup() {
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);
    dht.begin();
    Blynk.begin(auth, ssid, pass, "server.wyns.it", 8081);
    timer.setInterval(1000L, updateBlynk); // Elke seconde de Blynk-app bijwerken
}

void loop() {
    Blynk.run();
    timer.run();
}

void updateBlynk() {
    // Lees de temperatuur van de sensor
    float temperature = dht.readTemperature();

        if (!isnan(temperature)) {
        currentTemp = temperature;
    }

    // Lees de waarde van de potentiometer
    potValue = analogRead(POT_PIN);

    // Converteer de potentiometerwaarde naar een gewenste temperatuur tussen 0 en 30 graden Celsius
    desiredTemp = map(potValue, 0, 4095, 0, 30);

    // Stuur de gemeten en gewenste temperatuur naar de Blynk-app
    Blynk.virtualWrite(V0, currentTemp);   // Virtuele pin V0 voor actuele temperatuur
    Blynk.virtualWrite(V1, desiredTemp);   // Virtuele pin V1 voor ingestelde temperatuur

    // Vergelijk de gemeten temperatuur met de gewenste temperatuur
    if (currentTemp < desiredTemp) {
        // Schakel de LED in
        digitalWrite(LED_PIN, HIGH);
        Blynk.virtualWrite(V2, 255); // Virtuele pin V2 voor status van de verwarmingsketel
    } else {
        // Schakel de LED uit
        digitalWrite(LED_PIN, LOW);
        Blynk.virtualWrite(V2, 0); // Virtuele pin V2 voor status van de verwarmingsketel
    }
}