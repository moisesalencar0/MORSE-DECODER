# Projeto Final - TPSE

## Visão Geral

Projeto final da disciplina TPSE utilizando a BeagleBone Black.

O sistema consiste em um monitor de temperatura ambiente voltado para o contexto de uma planta em ambiente controlado. O sistema mede continuamente a temperatura através de um sensor I2C e alerta quando a temperatura sai de uma faixa configurável.

A demonstração será realizada utilizando um isqueiro para elevar artificialmente a temperatura e disparar os alarmes.

---

# Objetivos

## Objetivo Principal

Desenvolver um sistema embarcado capaz de:

* Ler temperatura ambiente através de sensor I2C.
* Exibir a temperatura em um display I2C.
* Permitir configuração de temperatura mínima e máxima, com botões.
* Acionar alertas visuais e sonoros quando a temperatura sair da faixa definida.
* Operar de forma contínua na BeagleBone Black.

## Contexto da Aplicação

Controle de temperatura para uma planta que necessita permanecer dentro de uma faixa adequada.

Exemplo:

* Temperatura mínima: 20°C
* Temperatura máxima: 30°C

Comportamento:

* Temperatura < 20°C → Alarme.
* Temperatura > 30°C → Alarme.
* Temperatura dentro da faixa → Operação normal.

---

# Hardware

## Plataforma

* BeagleBone Black

## Periféricos

### Sensor de Temperatura

Interface:

* I2C

Responsabilidade:

* Medir temperatura ambiente.

### Display

Interface:

* I2C

Responsabilidade:

* Exibir temperatura.
* Exibir status do sistema.

### Buzzer

Responsabilidade:

* Alerta sonoro.

### LED

Responsabilidade:

* Alerta visual.

---

# Funcionalidades Obrigatórias

## Leitura de Temperatura

O sistema deve:

* Ler temperatura periodicamente.
* Atualizar valor exibido.

## Exibição

Mostrar:

TEMP: XX.X C

ou

TEMP: XX.X C

ou

TEMP: XX.X C

## Controle de Faixa

Configurar:

* Temperatura mínima.
* Temperatura máxima.

## Alarmes

Quando a temperatura estiver fora da faixa:

* Acionar buzzer.
* Acionar LED.

Quando retornar à faixa:

* Desligar buzzer.
* Desligar LED.

---

# Funcionalidades Extras Desejadas

## Configuração via UART      --- OBS: NÃO VAMOS USAR UM MENU DE UART: VAMOS USAR OS BOTÕES PRA SETAR, EM UM MODO EDIÇÃO, TIPO UM BOTÃO PRA AUMENTAR PRA DIMINUIR.

Menu simples:

1 - Alterar temperatura mínima

2 - Alterar temperatura máxima

3 - Mostrar configuração

4 - Mostrar temperatura atual

## Histerese

Objetivo:

Evitar que o sistema fique entrando e saindo do estado de alarme rapidamente.

Exemplo:

MAX = 30°C

Entrou em alarme em:

30.1°C

Só retorna ao estado normal abaixo de:

29.0°C

## Máquina de Estados

Estados:

NORMAL

ALTA_TEMPERATURA

BAIXA_TEMPERATURA

Benefícios:

* Código mais organizado.
* Mais fácil de debugar.

## Heartbeat

LED piscando lentamente para indicar:

"Sistema operacional."

---

# Organização do Código

Estrutura inicial sugerida:

project/

├── app/

│ ├── main.c

│ ├── menu.c

│ └── controller.c

│

├── drivers/

│ ├── gpio.c

│ ├── uart.c

│ ├── timer.c

│ ├── i2c.c

│ ├── buzzer.c

│ ├── temp_sensor.c

│ └── display.c

│

├── include/

│ ├── gpio.h

│ ├── uart.h

│ ├── timer.h

│ ├── i2c.h

│ ├── buzzer.h

│ ├── temp_sensor.h

│ └── display.h

│

├── docs/

│ ├── esquema.png

│ ├── contexto_projeto.md

│ └── fotos/

│

└── README.md

---

# Reaproveitamento

O projeto utilizará as funções desenvolvidas durante as práticas anteriores.

Bibliotecas já existentes:

* GPIO
* UART
* Timer
* Delay
* Menu
* Controle de LEDs

Novos módulos:

* Driver I2C
* Driver do sensor de temperatura
* Driver do display

---

# Planejamento

## Dia 1

Objetivo:

Configurar barramento I2C.

Entregáveis:

* Sensor detectado.
* Display detectado.

---

## Dia 2

Objetivo:

Leitura do sensor.

Entregáveis:

* Temperatura exibida na UART.

---

## Dia 3

Objetivo:

Display funcionando.

Entregáveis:

* Temperatura exibida no display.

---

## Dia 4

Objetivo:

Implementar lógica de controle.

Entregáveis:

* Alarmes funcionando.

---

## Dia 5

Objetivo:

Configuração de limites.

Entregáveis:

* Menu UART funcional.

---

## Dia 6

Objetivo:

Montagem física.

Entregáveis:

* Estrutura organizada.
* Fiação protegida.

---

## Dia 7

Objetivo:

Testes finais.

Entregáveis:

* Demonstração completa.
* Documentação básica.

---

# Segurança Física

Objetivos:

* Evitar exposição direta dos fios.
* Facilitar transporte.
* Melhorar apresentação.

Possibilidades:

* Caixa de papelão rígido.
* Caixa impressa em 3D.
* Caixa acrílica.
* Base em MDF.

Importante:

Professor não deve ter acesso fácil aos fios energizados.

---

# Documentação

## README

Deve conter:

* Objetivo do projeto.
* Componentes utilizados.
* Ligações.
* Como compilar.
* Como executar.
* Fotos do projeto.

## Esquemático

Produzir imagem contendo:

* BeagleBone Black.
* Sensor.
* Display.
* LED.
* Buzzer.
* Ligações de alimentação.
* Ligações I2C.

---

# Controle de Versão

Mesmo de forma simples.

Fluxo sugerido:

git init

git add .

git commit -m "Projeto inicial"

git commit -m "I2C funcionando"

git commit -m "Sensor funcionando"

git commit -m "Display funcionando"

git commit -m "Sistema integrado"

---

# Critérios de Sucesso

## Mínimo

[ ] Sensor funcionando

[ ] Display funcionando

[ ] Leitura contínua

[ ] LED de alarme

[ ] Buzzer de alarme

[ ] Faixa mínima e máxima

[ ] Demonstração com isqueiro

[ ] INTERRUPÇÃO COM BOTÃO : setar maximo e mínimo atrávés de edit

[ ] Código modular

## Desejável

[ ] Caixa de proteção

[ ] Esquemático

[ ] README

[ ] GitHub


## Decisões de Arquitetura e Hardware (23/06/2026)

Estrutura definida:

```text
app/
board/
drivers/
peripherals/
inc/
└── starterware kit
```

Responsabilidades:

* `app/` → lógica principal da aplicação e máquina de estados.
* `board/` → inicialização da placa, pinmux e configuração global.
* `drivers/` → abstrações dos periféricos do AM335x (GPIO, I2C, UART, Timer).
* `peripherals/` → dispositivos do projeto (sensor, display, buzzer, LEDs).
* `inc/` → código de terceiros, lista de hardware

---

### Organização da Inicialização

Fluxo planejado:

```c
int main(void)
{
    BoardInit();

    while(1)
    {
        ApplicationRun();
    }
}
```

BoardInit:

```text
BoardInit()
├── BoardPinMuxSetup()  ---- ou algo do tipo
├── GPIOInit()
├── I2CInit()
├── UARTInit()
└── TimerInit()
```

---

O pinmux será responsabilidade do módulo `board`.
Cada driver será responsável por habilitar seu próprio clock.

---


Uso preferencial de:

```c
uint32_t
uint16_t
uint8_t
```

em vez de:

```c
unsigned int
unsigned short
unsigned char
```

-

| Função       | GPIO     | Pino BBB |
| ------------ | -------- | -------- |
| BBB_LED0 dbg | GPIO1_21 | 
| BBB_LED1  dbg| GPIO1_22 | 
| LED_red      | GPIO1_28 | P8_12    |
| BUZZER       | GPIO1_17 | P9_23    |
| BUTTON_UP    | GPIO2_06 | P8_45    |
| BUTTON_DOWN  | GPIO2_07 | P8_46    |

Obs.: confirmar pinmux e conflitos antes da montagem final.

---

### Interface do Usuário

Decisão:

Utilizar apenas 2 botões.

Motivos:

* Menor custo.
* Menos fios.
* Menor ocupação de GPIO.
* Interface suficientemente simples para o projeto.

---

### Máquina de Estados do Menu

Estados:

```c
VIEW_MODE
EDIT_MIN
EDIT_MAX
```

Fluxo:

```text
VIEW_MODE
    |
    | UP + DOWN
    v
EDIT_MIN
    |
    | UP + DOWN
    v
EDIT_MAX
    |
    | UP + DOWN
    v
VIEW_MODE
```

---

### Operação dos Botões

Modo normal:

```text
Mostra temperatura atual.
```

Editar MIN:

```text
UP   → aumenta MIN
DOWN → diminui MIN
```

Editar MAX:

```text
UP   → aumenta MAX
DOWN → diminui MAX
```

Troca de estado:

```text
UP + DOWN pressionados simultaneamente
```

---

### Estratégia de Detecção de Dois Botões

Não exigir simultaneidade perfeita.

Implementação prevista:

```c
if (BUTTON_UP && BUTTON_DOWN)
{
    ...
}
```

Com:

* debounce de ~20 ms;
* trava de evento (lock);
* novo evento apenas após soltar ambos os botões.

---
