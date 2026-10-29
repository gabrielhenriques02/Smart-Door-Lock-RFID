//RFID tag scan code
//Youtube Channel : Mahade DIY
// Ask me if you face any problem to your project

/*
Esse código é para descobrir o ID de cada cartão. O ID de um cartão nada mais é
do que um conjunto de 4bytes que são lidos sequencialmente pelo leitor e convertidos
para um valor Hexadecimal.

Se tiverem dúvidas sobre o que é um Byte e o que é um valor Hexadecimal, segue um link
que explica o que eles são: https://www.treinaweb.com.br/blog/bits-bytes-e-unidades-de-medida
*/

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN  10
byte readCard[4];
byte a = 0;

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  while (!Serial);
  SPI.begin();
  mfrc522.PCD_Init();
  delay(4);
  mfrc522.PCD_DumpVersionToSerial();
 
 Serial.print("Scan your card");
}

void loop() {
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return 0;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return 0;
  }
  a = 0;
  Serial.println(F("Scanned PICC's UID:"));
  for ( uint8_t i = 0; i < 4; i++) {  //
    readCard[i] = mfrc522.uid.uidByte[i];
    Serial.print(readCard[i], HEX);
    Serial.print(" ");
  
    delay(500);
    a += 3;
  }
  Serial.println("");
  mfrc522.PICC_HaltA();
  return 1;
}

