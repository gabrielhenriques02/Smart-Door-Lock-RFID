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

> **Use sempre o terminal NO.** Assim a trava só recebe 12V enquanto o Arduino comanda a abertura e fica desenergizada (trancada) se o Arduino desligar ou reiniciar. Se o módulo for do tipo Nível Baixo / Low-Level Trigger (aciona com `LOW` no pino IN), a correção é inverter a lógica no código, não trocar NO por NC. Ligada em NC, a trava ficaria energizada o tempo todo, aqueceria e abriria a porta em caso de falha.

---

## 3. Diodo de Proteção (Flyback)

A solenoide é uma bobina: ao ser desligada, gera um pico de tensão reversa que desgasta o contato do relé e pode reiniciar ou travar o Arduino. Para absorver esse pico, ligue um diodo **1N4007** em paralelo com a trava, o mais próximo possível dos terminais dela:

| Terminal do Diodo | Conexão | Observação |
| :--- | :--- | :--- |
| **Catodo** (lado da faixa) | **Fio Vermelho (+)** da Solenoide | Polaridade invertida em relação à fonte |
| **Anodo** | **Fio Preto (-)** da Solenoide | |

> **Atenção:** Se o diodo for ligado ao contrário, ele entra em curto com a fonte de 12V quando o relé fechar.

---

## 4. Alimentação do Conjunto
- Conecte o plug P4 da fonte de 12V ao conector Jack DC do Arduino Uno para alimentar a placa com segurança (o regulador interno rebaixa para 5V/3.3V).
- Derive a linha de 12V em paralelo para alimentar a etapa de potência da trava.

## 5. Lista Jumpers

| Tipo do Jumper | Quantidade | Ligação |
| :--- | :--- | :--- |
| **Adaptação fonte 12V** | 2 | M/M |
| **Relé - protoboard** | 3 | M/F |
| **solenoide - relé** | 2 | M/M |
