#include <Arduino.h>

int puls = 2; // Pin del pulsador
int LED = 10; // Pin del LED

unsigned long timeWorking = 0;   // Segundos que ha estado trabajando
unsigned long timeOffOn = 0;     // Segundos que ha estado parado
unsigned long prevSecond = 0;    // Para contar segundos
unsigned long startPress = 0;    // Cuando comenzó a pulsar el botón
bool isWorking = false;          // Estado actual
bool ledOn = false;              // LED encendido permanentemente

void setup() {
  Serial.begin(9600);
  pinMode(puls, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
}

void loop() {
  int pulsValue = digitalRead(puls);
  unsigned long currentMillis = millis();

  if (pulsValue == LOW) { // Pulsador presionado
    if (!isWorking) {
      // Acaba de iniciar el trabajo
      Serial.print("Temps entre parada-arranc: ");
      Serial.print(timeOffOn);
      Serial.println(" segons");
      timeOffOn = 0;
      prevSecond = currentMillis;
      startPress = currentMillis; // Marca inicio del pulsado
    }

    // Contar segundos trabajando
    if (currentMillis - prevSecond >= 1000) {
      prevSecond = currentMillis;
      timeWorking++;
    }

    isWorking = true;

  } else { // Pulsador suelto
    if (isWorking) {
      // Se acaba de soltar
      Serial.print("Temps treballat: ");
      Serial.print(timeWorking);
      Serial.println(" segons");

      // Encender LED si pulsado ≤ 15 s
      if ((currentMillis - startPress) <= 15000) {
        digitalWrite(LED, HIGH);
        ledOn = true;
      }

      timeWorking = 0;
      prevSecond = currentMillis;
    }

    // Contar segundos en parada
    if (currentMillis - prevSecond >= 1000) {
      prevSecond = currentMillis;
      timeOffOn++;
    }

    isWorking = false;
    startPress = 0; // Reiniciamos contador de pulsado
  }
}