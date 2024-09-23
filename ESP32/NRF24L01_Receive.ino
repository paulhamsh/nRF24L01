// ESP 32 keystudio
// ================
//
//  19   MISO
//  18   SCK      MOSI   23
//  16   CE       CSN    17
//       GND      VCC

#include <RF24.h>

RF24 radio(16, 17);
const uint8_t address[5] = {0x31, 0x30, 0x30, 0x30, 0x30};

void setup() {
  Serial.begin(115200);
  Serial.println("Started");
  SPI.begin();
  radio.begin();
  radio.setChannel(80);
  radio.setDataRate(RF24_250KBPS);
  radio.enableDynamicPayloads();
  radio.setAutoAck(true);
  radio.openReadingPipe(1, address);
  radio.startListening();
  Serial.println(radio.isChipConnected() ? "nRF24L01 ok" : "nRF24L01 not responding");
  radio.printPrettyDetails();

  Serial.println("Running");
}

uint8_t data_pipe;
int sz;

void loop() {
  if(radio.available(&data_pipe)) {
    sz = radio.getDynamicPayloadSize();
    Serial.print("Received on pipe ");
    Serial.print(data_pipe);
    Serial.print(" size ");
    Serial.print(sz);
    Serial.print(" : ");

    uint8_t data[sz];

    radio.read(&data, sz);
    Serial.print(data[0],HEX);
    Serial.print(" ");
    Serial.println(data[1], HEX);
  }
}
