    // Přejděte na začátek pixelových dat
      uint32_t pixelDataOffset = *(uint32_t*)&bmpHeader[10];
      bmpFile.seek(pixelDataOffset); // Změna pozice na začátek pixelových dat


      for (uint32_t y = 0; y < height && y < 33; y++) {
        for (uint32_t x = 0; x < width && x < 33; x++) {
          // Přejděte na původní pixel
          bmpFile.seek(pixelDataOffset + ((height - y) * width + x) * 3); // Přečtěte RGB
          
          byte blue = bmpFile.read();  // Modrá složka
          byte green = bmpFile.read(); // Zelená složka
          byte red = bmpFile.read();   // Červená složka
          
          // Otočení o 90 stupňů po směru hodinových ručiček a zrcadlení po ose X
          //int pixelIndex = x * 33 + ((32 - y) * -1); // Nový výpočet pro 90 stupňů
          int pixelIndex = x * 33 + y;
        
    
          panel.setPixelColor(pixelIndex, panel.Color(red, green, blue));
        }
      }

      panel.show();
