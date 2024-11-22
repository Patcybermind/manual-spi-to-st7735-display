#include <SPI.h>

#define TFT_CS   10  // Chip select pin
#define TFT_DC    9  // Data/Command pin
#define TFT_RST   8  // Reset pin (optional, can tie to 3.3V if unused)

void writeCommand(uint8_t cmd) {
  digitalWrite(TFT_DC, LOW);  // Command mode
  SPI.transfer(cmd);
}

void writeData(uint8_t data) {
  digitalWrite(TFT_DC, HIGH); // Data mode
  SPI.transfer(data);
}
bool blackTime = false;
void setup() {
  
  Serial.begin(9600);

  pinMode(TFT_CS, OUTPUT);
  pinMode(TFT_DC, OUTPUT);
  pinMode(TFT_RST, OUTPUT);

  SPI.begin();
  SPI.beginTransaction(SPISettings(16000000, MSBFIRST, SPI_MODE0));

  SPI.setBitOrder(MSBFIRST); // Most Significant Bit First
  SPI.setDataMode(SPI_MODE0); // Mode 0 (CPOL=0, CPHA=0)

  // Reset the display (optional)
  digitalWrite(TFT_RST, LOW);
  delay(10);
  digitalWrite(TFT_RST, HIGH);
  delay(5);

  digitalWrite(TFT_CS, LOW); // Select the display

  // Initialization sequence (refer to ST7735 datasheet)
  writeCommand(0x01); // SWRESET
  delay(120);
  writeCommand(0x11); // SLPOUT
  delay(120);
  writeCommand(0x3A); // COLMOD
  writeData(0x06);     // 18-bit color
  writeCommand(0x36); // MADCTL
  writeData(0x00);     // Normal orientation
  writeCommand(0x2A); // CASET
  writeData(0x00);
  writeData(0x00);
  writeData(0x00);
  writeData(0x7F);
  writeCommand(0x2B); // RASET
  writeData(0x00);
  writeData(0x00);
  writeData(0x00);
  writeData(0x9F);
  writeCommand(0x29); // DISPON
  delay(120);
  
  digitalWrite(TFT_CS, HIGH); // Deselect

}


void loop() {
  // setup a timer to time how long it takes to send a frame
  

  
  digitalWrite(TFT_CS, LOW); // Select display
  writeCommand(0x2A); // CASET
  writeData(0x00);
  writeData(0x00);
  writeData(0x00);
  writeData(0x7F);
  writeCommand(0x2B); // RASET
  writeData(0x00);
  writeData(0x00);
  writeData(0x00);
  writeData(0x9F); // in binary it is 10011111 and its decimal its : 159
  writeCommand(0x28); // DISPOFF / freeze display
  writeCommand(0x2C); // RAMWR

  
  digitalWrite(TFT_DC, HIGH); // Data mode
  uint32_t start = millis();
  if (blackTime == true) {
  //uint8_t r, g, b;
    
    for (int i = 0; i < 20480; i++) {
        SPI.transfer(0x00);
        SPI.transfer(0x00);      
        SPI.transfer(0x00);      
    }
  
  } else {
  //uint8_t r, g, b;
    
    for (int i = 0; i < 20480; i++) {
        SPI.transfer(0xFC);  
        SPI.transfer(0xFC);    
        SPI.transfer(0xFC);   
  }
  } 

  writeCommand(0x29); // DISPON / unfreeze display
  digitalWrite(TFT_CS, HIGH); // Deselect display

  // You can add a delay here to control the gradient update rate.
  // Or remove it to constantly cycle through the loop.
  // calculate the time it took to send the frame
  uint32_t elapsed = millis() - start;
  Serial.print("Frame time: ");
  Serial.print(elapsed);
  Serial.println(" ms");

  if (blackTime == true) {
    blackTime = false;
  } else {
    blackTime = true;
  }

  delay(1000);
}