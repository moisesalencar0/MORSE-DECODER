Tradutor de Código Morse para Texto e Texto para Código Morse
Sobre o projeto
O que este projeto implementa?

Este projeto implementa um tradutor bidirecional entre código Morse e texto utilizando a BeagleBone Black.

Objetivo

O objetivo da atividade foi aplicar, de forma prática, os conceitos fundamentais de programação para microcontroladores abordados durante a disciplina, desenvolvendo um sistema embarcado completo envolvendo drivers, interrupções, temporizadores e periféricos da placa.

Disciplina

Técnicas de Programação para Sistemas Embarcados

Funcionalidades

O sistema possui dois modos de operação.

Modo 1 — Código Morse → Texto

A interface é realizada pela UART (via Minicom), onde é exibido um menu de seleção.

A entrada do código Morse é feita utilizando dois botões da BeagleBone Black:

Botão Down → ponto (.)
Botão Up → traço (-)

Os dois botões pressionados simultaneamente (combo) possuem funções especiais:

1 pressão: confirma a letra atual;
2 pressões consecutivas: adiciona um espaço entre palavras (/);
3 pressões consecutivas: finaliza a mensagem e inicia a decodificação.

O sistema ignora sequências que não pertencem à tabela internacional de código Morse.

O buffer de entrada suporta até 128 caracteres.

Modo 2 — Texto → Código Morse

O usuário envia uma mensagem pela UART.

O sistema converte automaticamente cada caractere em código Morse, representando-o através de:

piscadas em um LED;
sinais sonoros emitidos por um buzzer.
Hardware utilizado
BeagleBone Black
Processador Sitara AM335x (Texas Instruments)
Periféricos utilizados
GPIO
DMTimer7
Controlador de interrupções (INTC)
UART
Watchdog Timer
Organização do projeto

Com objetivo didático, o projeto foi organizado em camadas.

Cada periférico possui sua própria implementação de driver, enquanto os módulos responsáveis pela lógica da aplicação permanecem separados, reduzindo o acoplamento entre hardware e aplicação.

Compilação

O projeto deve ser compilado utilizando uma ferramenta de cross-compilation para ARM.

Após a compilação, o binário é enviado para a BeagleBone Black utilizando TFTP.

O repositório contém um documento em PDF explicando detalhadamente todo o processo de configuração do ambiente de desenvolvimento.

Execução

Após gravar o binário na placa:

Inicie a execução do programa.
Conecte-se à UART utilizando o Minicom.
O menu principal será exibido.
Escolha um dos modos de operação:
Código Morse → Texto;
Texto → Código Morse.
Entradas
Botões físicos da BeagleBone Black;
Mensagens enviadas pela UART.
Saídas
Texto exibido na UART;
Piscadas do LED;
Sinais sonoros do buzzer.
Funcionamento interno

A aplicação é coordenada pela main.c, responsável pelo fluxo principal do programa.

A inicialização dos periféricos é centralizada em board.c, através da função Board_Init(), que configura e inicializa todos os drivers necessários.

Os módulos da aplicação utilizam esses drivers para acessar o hardware, mantendo uma separação entre lógica de aplicação e controle dos periféricos.

Limitações
A implementação utiliza apenas a tabela internacional básica de código Morse, não incluindo símbolos especiais como @, $, %, entre outros.
O buffer de entrada possui capacidade máxima de 128 caracteres.
A entrada em código Morse foi adaptada para utilizar dois botões (ponto e traço), devido às limitações físicas e de precisão dos botões disponíveis.
Embora o projeto utilize debounce por software e tolerâncias temporais, a implementação do método tradicional de um único botão exigiria maior precisão na detecção da duração dos pressionamentos.
Autores
Moisés Alencar — UFC — GitHub: @moisesalencar0
Rafael de Castro — UFC — GitHub: @rafaelccastro007