#include <SPI.h>
#include <SD.h>

void setup() {
  Serial.begin(9600);
  Serial.println("Inizializzazione SD...");

  if (!SD.begin(10)) {
    Serial.println("Inizializzazione della scheda SD fallita!");
    return;
  }

  Serial.println("Scheda SD inizializzata correttamente.");
}

void loop() {}
