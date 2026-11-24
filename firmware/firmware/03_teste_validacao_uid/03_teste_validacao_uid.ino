// Etapa Intermediária: Leitura de UID e Teste de Validação no Monitor Serial

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN  10

// ID do cartão cadastrado como autorizado (com os zeros formatados)
String UID_AUTORIZADO = "D5 D6 03 07";

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  while (!Serial);
  SPI.begin();
  mfrc522.PCD_Init();
  delay(4);
  mfrc522.PCD_DumpVersionToSerial();

  Serial.println("Aproxime o cartao do leitor...");
}

void loop() {
  // Aguarda a aproximação de uma tag/cartão
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Faz a leitura dos dados do cartão
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  String idLido = "";

  // Lê os bytes do UID e formata a String garantindo o zero à esquerda
  for (uint8_t i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) {
      idLido += "0";
    }
    idLido += String(mfrc522.uid.uidByte[i], HEX);

    if (i < mfrc522.uid.size - 1) {
      idLido += " ";
    }
  }

  idLido.toUpperCase(); // Converte os caracteres para maiúsculas

  Serial.print("ID Lido: ");
  Serial.println(idLido);

  // Verificação se o cartão lido corresponde ao cadastrado
  if (idLido == UID_AUTORIZADO) {
    Serial.println("Status: ACESSO CONCEDIDO!");
  } else {
    Serial.println("Status: ACESSO NEGADO!");
  }
  Serial.println("------------------------------------");

  mfrc522.PICC_HaltA(); // Interrompe a leitura da tag atual
  delay(1000);          // Aguarda 1 segundo para evitar múltiplas leituras seguidas
}
