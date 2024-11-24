// ESP 32 keystudio
// ================
//
//  19   MISO
//  18   SCK      MOSI   23
//  16   CE       CSN    17
//       GND      VCC

#include <RF24.h>

RF24 radio(16, 17); // CE, CSN
const uint8_t address[5] = {0x31, 0x30, 0x30, 0x30, 0x30};

void setup() {
  Serial.begin(115200); 
  Serial.println("Starting configuration");
  SPI.begin();
  radio.begin();
  radio.setChannel(80);
  radio.setDataRate(RF24_250KBPS);
  radio.enableDynamicPayloads();
  radio.openWritingPipe(address);
  radio.stopListening(); 
  Serial.println(radio.isChipConnected() ? "nRF24L01 ok" : "nRF24L01 not responding");
  radio.printDetails();
  radio.printPrettyDetails();

  Serial.println("Running");
}

uint8_t dat[2] = {0x11, 0x22};

void loop() {
  bool status;
  Serial.print("Sending...");
  Serial.print(dat[0], HEX);
  Serial.print(" ");
  Serial.print(dat[1], HEX);
  Serial.print(" : ");

  status = radio.write(&dat, 2);
  Serial.println(status ? "success" : "fail");



  dat[1]++;
  delay(1000);
}