// Etapa 2: Controle de Acesso da Fechadura Eletrônica do Cofre

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN  10

const int RELAY_PIN = 7;

// ID retornado pelo leitor na Etapa 1
String UID_AUTORIZADO = "D5 D6 3 7";

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  // Configuração do relé (LOW para acionar módulos Low-Level Trigger)
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // Inicia desligado em repouso

  SPI.begin();
  mfrc522.PCD_Init();
  delay(4);
}

void loop() {
  // Aguarda a aproximação de um cartão
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Lê os dados do cartão
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  String idLido = "";

  for (uint8_t i = 0; i < mfrc522.uid.size; i++) {
    idLido += String(mfrc522.uid.uidByte[i], HEX);

    if (i < mfrc522.uid.size - 1) {
      idLido += " ";
    }
  }

  idLido.toUpperCase();

  // Verificação do cartão e acionamento
  if (idLido == UID_AUTORIZADO) {
    digitalWrite(RELAY_PIN, LOW);   // Liga o relé e destranca
    delay(3000);                    // Mantém aberto por 3 segundos
    digitalWrite(RELAY_PIN, HIGH);  // Desliga o relé e tranca
  }

  // Coloca o cartão atual em pausa até ser afastado
  mfrc522.PICC_HaltA();

  // Reinicializa a comunicação do leitor para destravar a SPI caso ocorra ruído do relé
  mfrc522.PCD_Init();

  delay(1000); // Pausa de segurança antes da próxima leitura
}
