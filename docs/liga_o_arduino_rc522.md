# Ligação Arduino Uno ↔ Leitor RFID RC522

Guia de fiação e pinagem entre o microcontrolador Arduino Uno e o módulo leitor RFID-RC522 (comunicação SPI).

---

## 1. Tabela de Pinagem

| Pino no RC522 | Função / Sinal | Conexão no Arduino Uno | Observações |
| :--- | :--- | :--- | :--- |
| **3.3V** | Alimentação | **3.3V** | **Atenção:** Nunca conecte ao 5V para evitar queimar o módulo |
| **RST** | Reset | **Pino Digital 9** | Controle de reset via software |
| **GND** | Terra (0V) | **GND** | Referência comum |
| **IRQ** | Interrupção | *Não conectado* | Deixe desconectado |
| **MISO** | SPI Master In / Slave Out | **Pino Digital 12** | Barramento SPI |
| **MOSI** | SPI Master Out / Slave In | **Pino Digital 11** | Barramento SPI |
| **SCK** | SPI Clock | **Pino Digital 13** | Barramento SPI |
| **SDA (SS)**| SPI Slave Select | **Pino Digital 10** | Habilitação do chip SPI |

---

## 2. Recomendações de Montagem
- Utilize jumpers fêmea-macho para ligar o RC522 diretamente à protoboard ou ao Arduino.
- Certifique-se de que a barra de pinos do RC522 esteja bem soldada para evitar falhas intermitentes de comunicação via SPI.
- Mantenha os fios o mais curtos possível para minimizar ruídos no sinal.