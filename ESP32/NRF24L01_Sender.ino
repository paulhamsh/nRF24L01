// ESP 32 M5Core 2
// ===============
//
//  38   MISO
//  18   SCK      MOSI   23
//  3    CE       CSN    13
//       GND      VCC

#include <RF24.h>
#include <M5Core2.h>

RF24 radio(3, 13); // CE, CSN
const uint8_t address[5] = {0x31, 0x30, 0x30, 0x30, 0x30};

void setup() {
  M5.begin();
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(2); 
  M5.Lcd.println("NRF24L01 Sender");
  
  Serial.println("Starting configuration");
  SPI.begin();
  radio.begin();
  radio.setChannel(80);
  radio.setDataRate(RF24_250KBPS);
  radio.enableDynamicPayloads();
  radio.openWritingPipe(address);
  radio.stopListening(); 
  Serial.println(radio.isChipConnected() ? "nRF24L01 ok" : "nRF24L01 not responding");
  M5.Lcd.print(radio.isChipConnected() ? "nRF24L01 ok" : "nRF24L01 not responding"); 
  radio.printPrettyDetails();

  Serial.println("Running");
}

uint8_t dat[2] = {0x11, 0x22};

void loop() {
  Serial.print("Sending...");
  Serial.print(dat[0], HEX);
  Serial.print(" ");
  Serial.println(dat[1], HEX);

  radio.write(&dat, 2);

  dat[1]++;
  delay(1000);
}
