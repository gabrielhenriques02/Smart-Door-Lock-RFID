# Ligação Arduino Uno ↔ Display LCD 16x2 (I2C)

Guia de fiação entre o Arduino Uno e o módulo Display LCD 16x2 com adaptador I2C (PCF8574).

---

## 1. Tabela de Pinagem

| Pino no Módulo I2C | Função / Sinal | Conexão no Arduino Uno | Observações |
| :--- | :--- | :--- | :--- |
| **GND** | Terra (0V) | **GND** | Barramento GND |
| **VCC** | Alimentação (+5V) | **5V** | Barramento 5V |
| **SDA** | Dados I2C | **Pino Analógico A4** | Barramento I2C (SDA) |
| **SCL** | Clock I2C | **Pino Analógico A5** | Barramento I2C (SCL) |

---

## 2. Dicas e Ajustes
- **Ajuste de Contraste:** Se o display ligar mas não exibir caracteres legíveis, ajuste o trimpot azul localizado na parte traseira do módulo I2C com uma chave de fenda pequena.
- **Endereço I2C:** A maioria desses módulos utiliza o endereço `0x27` ou `0x3F`. Use a biblioteca `LiquidCrystal_I2C` informando o endereço correto no código.

## 3. Lista Jumpers

| Tipo do Jumper | Quantidade |
| :--- | :--- |
| **M/M** | 7 |
| **M/F** | 0 |
