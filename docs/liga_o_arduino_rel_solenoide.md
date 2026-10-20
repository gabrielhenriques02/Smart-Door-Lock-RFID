# Ligação Arduino Uno ↔ Módulo Relé 5V ↔ Trava Solenoide 12V

Guia de acionamento de carga externa de 12V (trava solenoide) através de um módulo relé de 5V controlado pelo Arduino.

---

## 1. Conexão: Arduino ↔ Módulo Relé (Lado Lógico)

| Pino no Relé | Função | Conexão no Arduino | Observações |
| :--- | :--- | :--- | :--- |
| **VCC** | Alimentação (+5V) | **5V** | Alimentação da bobina |
| **GND** | Terra (0V) | **GND** | Terra comum |
| **IN** | Sinal de Controle | **Pino Digital 7** | Saída digital configurada no código |

---

## 2. Conexão de Potência: Fonte 12V ↔ Relé ↔ Trava Solenoide

> **Atenção:** A trava solenoide requer alimentação externa de 12V e não deve ser alimentada diretamente pelos pinos de 5V do Arduino.

| Origem | Destino | Descrição |
| :--- | :--- | :--- |
| **Fonte 12V (+)** | **COM** (Comum do Relé) | O positivo da fonte de 12V entra no contato central do relé |
| **NO** (Normalmente Aberto) | **Fio Vermelho (+)** da Solenoide | Sai do relé apenas quando o Arduino comanda a abertura |
| **Fonte 12V (-)** | **Fio Preto (-)** da Solenoide | Conexão negativa direta entre a fonte e a solenoide |

*(Nota: Caso utilize um relé do tipo Nível Baixo / Low-Level Trigger, pode ser necessário ligar ao terminal **NC** ao invés de **NO**, dependendo da lógica do código).*

---

## 3. Alimentação do Conjunto
- Conecte o plug P4 da fonte de 12V ao conector Jack DC do Arduino Uno para alimentar a placa com segurança (o regulador interno rebaixa para 5V/3.3V).
- Derive a linha de 12V em paralelo para alimentar a etapa de potência da trava.