#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <SD.h>

#define LED_PIN 36                // Pin připojený k datovému pinu panelu
#define LED_COUNT 1089            // 33x33 pixelů = 1089 LED diod
#define JAS 30                    // Jas displeje
#define PIN_CS 4                  // Pin pro SD kartu
#define BACKGROUND_COLOR 0x000000 // Černá barva pozadí

Adafruit_NeoPixel panel = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);

void showImage(String fileName) {
  // Otevření souboru pro čtení
  File bmpFile = SD.open(fileName);

  // Pokud se obrázek otevře provede se následující
  if (bmpFile) {


    // Nastavení jasu
    panel.setBrightness(JAS);
    // Inicializace NeoPixel
    panel.begin();

    // Čtěte hlavičku BMP souboru
    byte bmpHeader[54];
    bmpFile.read(bmpHeader, 54); // Čtěte prvních 54 bytů (hlavička BMP)

    // Zkontrolujte, zda je soubor platný BMP
    if (bmpHeader[0] != 'B' || bmpHeader[1] != 'M') {
      Serial.println("Nepodporovaný formát souboru.");
      bmpFile.close();
      return;
    }

    // Získání šířky a výšky
    uint32_t width = *(uint32_t *)&bmpHeader[18];
    uint32_t height = *(uint32_t *)&bmpHeader[22];
    uint32_t bits = *(uint32_t *)&bmpHeader[28];
    //Serial.print("RGB bits: ");
    //Serial.println(bits);

    //Serial.print("Šířka: ");
    //Serial.println(width);
    //Serial.print("Výška: ");
    //Serial.println(height);

    // Přejděte na začátek pixelových dat
    uint32_t pixelDataOffset = *(uint32_t *)&bmpHeader[10];
    bmpFile.seek(pixelDataOffset); // Změna pozice na začátek pixelových dat

    for (int y = 0; y < height && y < 33; y++) {
      // Vypočítej padding pro aktuální řádek (každý řádek musí být násobkem 4 bajtů)
      int padding = (4 - (width * 3) % 4) % 4; // 3 bajty na pixel (RGB)

      for (int x = 0; x < width && x < 33; x++) {
        // Přejdi na pozici pixelu v BMP souboru
        bmpFile.seek(pixelDataOffset + (y * (width * 3 + padding)) + (x * 3));

        // Přečti RGB hodnoty
        byte blue = bmpFile.read();
        byte green = bmpFile.read();
        byte red = bmpFile.read();

        // Výpočet indexu pixelu na LED panelu
        int pixelIndex = (32 - x) * 33 + (32 - y); // nebo jiný výpočet v závislosti na orientaci panelu

        // Nastav barvu pixelu
        panel.setPixelColor(pixelIndex, panel.Color(red, green, blue));
      }
    }
    panel.show();
  }
  else {
    Serial.println("Nepodařilo se otevřít soubor obrazek.jpg");
  }
}

// Funkce pro tisk BMP souborů
void printBmpFiles() {
    File root = SD.open("/"); // Otevři kořenový adresář
    File file = root.openNextFile(); // Otevři první soubor

    while (file) {
        // Zkontroluj, zda je to normální soubor (ne adresář)
        if (!file.isDirectory()) {
          String fileName = file.name(); // Získej název souboru v originálním formátu
          Serial.println(fileName); // Vypiš název souboru

          if (fileName.indexOf('~') == -1 && fileName.endsWith(".BMP")) {
          }
        }
        
        file.close(); // Zavři aktuální soubor
        file = root.openNextFile(); // Otevři další soubor
    }

    root.close();
}

void setup() {
  Serial.begin(9600);
  if (!SD.begin(PIN_CS)) {
    Serial.println("SD karta nebyla načtena :(");
    while (true);
  }
  Serial.println("SD karta byla úspěšně inicializována.");

  printBmpFiles(); // Volání funkce pro vypsání souborů

}

void loop() {


  

  for (int i = 1; i <= 19; i++) {
      String imageName = "image" + String(i) + ".bmp";
      showImage(imageName);

      
  }

}