#include <Arduino.h>
#include <DHT.h>

#define DHTPIN 23        // Pin voor de DHT11-sensor
#define DHTTYPE DHT11    // Type DHT-sensor
#define LED_PIN 21       // Pin voor de LED van de verwarmingsketel
#define POT_PIN 32       // Pin voor de potentiometer

DHT dht(DHTPIN, DHTTYPE);

int desiredTemp = 0;   // Initieel ingestelde gewenste temperatuur
int currentTemp = 0;   // Variabele voor de gemeten temperatuur
int potValue = 0;      // Variabele voor de waarde van de potentiometer

void setup() {
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);
    dht.begin();
}

void loop() {
    // Lees de temperatuur van de sensor
    float temperature = dht.readTemperature();

    if (!isnan(temperature)) {
        currentTemp = temperature;
    }

    // Lees de waarde van de potentiometer
    potValue = analogRead(POT_PIN);

    // Converteer de potentiometerwaarde naar een gewenste temperatuur tussen 0 en 30 graden Celsius
    desiredTemp = map(potValue, 0, 4095, 0, 30);

    // Vergelijk de gemeten temperatuur met de gewenste temperatuur
    if (currentTemp < desiredTemp) {
        // Schakel de LED in
        digitalWrite(LED_PIN, HIGH);
    } else {
        // Schakel de LED uit
        digitalWrite(LED_PIN, LOW);
    }

    // Schrijf de gemeten en gewenste temperatuur naar de seriële monitor
    Serial.print("Gemeten temperatuur: ");
    Serial.println(currentTemp);
    Serial.print("Gewenste temperatuur: ");
    Serial.println(desiredTemp);

    delay(250);  // Wacht een seconde voordat de metingen opnieuw worden uitgevoerd
}
