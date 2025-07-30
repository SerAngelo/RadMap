#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Dimensioni display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Pin
const int geigerPin = 2;     // Pin collegato al contatore Geiger
const int SD_csPin = 10;     // Chip Select SD

// Display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Variabili conteggio
volatile int count = 0; // Conteggio impulsi
unsigned long previousMillis = 0;
const long interval = 60000; // 1 minuto

void setup() {
  pinMode(geigerPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(geigerPin), countPulse, RISING);

  Serial.begin(9600);

  // Inizializzazione SD
  if (!SD.begin(SD_csPin)) {
    Serial.println("Inizializzazione della scheda SD fallita!");
    while (true); // Blocca se fallisce
  }

  // Inizializzazione display OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 non trovato"));
    while (true); // Blocca se fallisce
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println(F("RadMAP Inizializzato"));
  display.display();
  delay(2000);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Mostra il conteggio
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 10);
    display.print("Conteggi: ");
    display.println(count);
    display.display();

    // Salva su SD
    File dataFile = SD.open("conteggi.txt", FILE_WRITE);
    if (dataFile) {
      dataFile.println(count);
      dataFile.close();
    } else {
      Serial.println("Errore nell'aprire il file per scrivere");
    }

    count = 0; // Reset conteggio
  }
}

void countPulse() {
  count++;
}