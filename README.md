# 🔐 Smart-Door-Lock-RFID

Sistema embarcado de controle de acesso desenvolvido com **Arduino Uno** e tecnologia **RFID (Identificação por Radiofrequência)**, projetado para atuar como a **fechadura eletrônica de um cofre**. O objetivo do projeto é substituir chaves mecânicas convencionais por uma autenticação digital segura, liberando a trava eletromecânica apenas mediante a aproximação de um cartão ou tag previamente autorizado[cite: 1].

---

## 🎯 Propósito e Visão Geral

Cofres e compartimentos de segurança exigem mecanismos de travamento confiáveis e de acionamento controlado. Neste projeto, o Arduino Uno atua como a unidade central de processamento que valida a identidade de cartões por proximidade (sem contato físico) e comanda um circuito de potência isolado para destrancar uma fechadura solenoide de 12V[cite: 1].

O sistema pode operar tanto com feedback visual em display LCD I2C quanto de forma autônoma e discreta (sem display, operando com temporização automática de travamento)[cite: 1].

---

## 🛠️ Preparações e Montagem

### 1. Componentes Necessários

* **1x Microcontrolador Arduino Uno**
* **1x Módulo Leitor RFID-RC522 (13,56 MHz)** + Cartões/Tags compatíveis
* **1x Módulo Relé 5V (1 Canal)** — isolamento entre o sinal lógico de 5V e a carga de 12V
* **1x Fechadura Trava Solenoide 12VDC**
* **1x Diodo Retificador 1N4007** — proteção contra surto de tensão reversa (*flyback*) da bobina da fechadura
* **1x Fonte de Alimentação 12VDC** + Conectores Jack P4 (macho e fêmea)
* **1x Display LCD 16x2 com Módulo Adaptador I2C** *(opcional, utilizado na versão com interface visual)*
* **Protoboard e Cabos Jumper** (macho-macho e macho-fêmea)[cite: 1]

### 2. Esquema de Ligação (Pinout)

#### Módulo RFID-RC522 (Comunicação SPI)

> **Atenção:** O módulo RC522 deve ser alimentado exclusivamente em **3.3V**[cite: 1].

| Pino RFID-RC522 | Pino Arduino Uno | Função |
| :--- | :--- | :--- |
| **3.3V (VCC)** | `3.3V` | Alimentação |
| **RST** | `D9` | Reset do módulo |
| **GND** | `GND` | Terra comum |
| **MISO** | `D12` | Master In Slave Out (SPI) |
| **MOSI** | `D11` | Master Out Slave In (SPI) |
| **SCK** | `D13` | Serial Clock (SPI) |
| **SDA (SS)** | `D10` | Slave Select (SPI)[cite: 1] |

#### Módulo Relé 5V, Circuito de Potência (12V) e Diodo de Proteção

* **Controle (Lado Arduino):**
  * `VCC` → Pino `5V` do Arduino
  * `GND` → Pino `GND` do Arduino
  * `IN` (Sinal) → Pino Digital `D7` do Arduino[cite: 1]
* **Potência (Lado Fechadura 12V):**
  * **Positivo (+12V) da Fonte** → Borne **COM** (*Common* / Comum) do relé
  * **Positivo (+12V) da Fechadura Solenoide** → Borne **NO** (*Normally Open* / Normal Aberto) do relé
  * **Negativo (-12V / GND) da Fonte** → Conectado **diretamente** ao fio negativo da fechadura solenoide e ao GND/Jack de alimentação do Arduino
  * **Borne NC (*Normally Closed*):** Permanece desconectado[cite: 1]
* **Diodo de Roda Livre / *Flyback* (`1N4007` em paralelo com a fechadura):**
  * **Cátodo (lado com a faixa cinza)** → Conectado ao **fio positivo (+12V)** da fechadura solenoide (saída do borne `NO`).
  * **Ânodo (lado sem faixa)** → Conectado ao **fio negativo (GND)** da fechadura solenoide.
  * *Função:* Permanece bloqueado enquanto a trava está acionada e conduz apenas no instante do desligamento, dissipando em ciclo fechado o pico de tensão reversa gerado pela bobina.

#### Display LCD 16x2 I2C *(Opcional)*

* `VCC` → `5V` | `GND` → `GND` | `SDA` → `A4` | `SCL` → `A5`[cite: 1]

### 3. Preparação do Software (Arduino IDE)

1. Instale a biblioteca **`MFRC522`** através do Gerenciador de Bibliotecas da Arduino IDE.
2. Caso utilize o display LCD, instale a biblioteca **`LiquidCrystal_I2C`** (ou a biblioteca **`hd44780`** de Bill Perry, que detecta automaticamente o endereço I2C e o mapeamento de pinos do módulo).
3. Selecione a placa **Arduino Uno** (`Ferramentas > Placa > Arduino AVR Boards > Arduino Uno`) e a porta serial correspondente[cite: 1].

---

## ⚙️ Parte Lógica e Funcionamento

### 1. Protocolos e Leitura Sequencial do Cartão

O funcionamento lógico do cofre baseia-se em duas camadas de comunicação[cite: 1]:

* **Interface Sem Fio (ISO/IEC 14443 Tipo A — 13,56 MHz):** Ao aproximar o cartão (PICC) do leitor, o campo eletromagnético energiza o chip interno da tag, que transmite seu **UID (*Unique Identifier*)**.
* **Interface Serial SPI (Leitor ↔ Arduino):** O MFRC522 repassa os dados ao Arduino via barramento SPI. Cada cartão possui um UID composto por **4 bytes** (32 bits) lidos sequencialmente e armazenados no vetor `mfrc522.uid.uidByte[]`[cite: 1].

### 2. Fluxo de Execução em Duas Etapas

1. **Etapa 1 — Descoberta do UID (`Tag Scan`):** Carrega-se um código de leitura que captura os 4 bytes do cartão, converte-os para representação **hexadecimal** e os exibe no Monitor Serial (exemplo: `D5 D6 3 7` ou `83 B9 BE 18`).
   * *Nota sobre formatação:* A função padrão `Serial.print(byte, HEX)` omite zeros à esquerda em valores menores que `0x10` (transformando `03` em `3`). A string cadastrada no código principal deve seguir exatamente a mesma formatação gerada pelo laço de leitura[cite: 1].
2. **Etapa 2 — Controle de Acesso (`Door Lock`):** No programa definitivo, o laço `loop()` aguarda continuamente a presença de um novo cartão (`PICC_IsNewCardPresent()`) e lê seu serial (`PICC_ReadCardSerial()`). Os bytes são concatenados em uma `String` em letras maiúsculas e comparados com a variável `UID_AUTORIZADO`[cite: 1].

### 3. Lógica de Acionamento do Relé e Segurança

* **Relé *Low-Level Trigger* vs. *High-Level Trigger*:**
  * Módulos relé do tipo **High-Level Trigger** energizam a bobina com sinal `HIGH` (5V) e desligam com `LOW` (0V).
  * Módulos do tipo **Low-Level Trigger** operam com lógica invertida: mantêm-se desligados em repouso com sinal `HIGH` (5V) e fecham o contato entre `COM` e `NO` ao receberem `LOW` (0V)[cite: 1].
* **Abertura Temporizada do Cofre:** Ao validar um cartão autorizado, o Arduino aciona o pino `D7` para liberar a trava solenoide, aguarda **3 segundos** (`delay(3000)`) para a abertura da porta do cofre e retorna o pino ao estado de repouso, trancando o mecanismo automaticamente. *(Nota: O projeto original de referência utiliza lógica de alternância/toggle a cada leitura, enquanto a adaptação para cofre utiliza o fechamento automático temporizado)*[cite: 1].
* **Estabilidade e Proteção contra Ruído Eletromagnético:**
  * **Hardware (Diodo *Flyback*):** O diodo `1N4007` em paralelo com a solenoide absorve a força contraeletromotriz gerada pelo colapso do campo magnético da bobina ao trancar a porta, impedindo que picos de tensão retornem para o Arduino e apaguem o display LCD.
  * **Software:** O comando `PICC_HaltA()` coloca o cartão em espera até ser afastado fisicamente, enquanto a reinicialização periódica do leitor (`mfrc522.PCD_Init()`) garante a recuperação do barramento SPI.

---

## 📚 Referências

* **Vídeo de Referência do Projeto:** [How to Make RFID Door Lock System Using Arduino — Canal Mahade DIY (YouTube)](https://www.youtube.com/watch?v=sqAxRGf0Rsg)
* **Documentação da Biblioteca MFRC522 & Padrão ISO/IEC 14443-A:** Comunicação RFID de proximidade em 13,56 MHz e interface SPI.
* **Conceitos de Bits, Bytes e Sistema Hexadecimal:** [TreinaWeb — Bits, Bytes e Unidades de Medida](https://www.treinaweb.com.br/blog/bits-bytes-e-unidades-de-medida)[cite: 1]