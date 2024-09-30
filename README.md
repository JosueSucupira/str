# PSistemas de Tempo Real - RP2040 com FreeRTOS

Este repositório contém os projetos desenvolvidos na disciplina de **Sistemas de Tempo Real**, utilizando a placa **Raspberry Pi Pico (RP2040)** e o **FreeRTOS**. O objetivo dos projetos foi implementar sistemas de controle em tempo real, explorando multitarefa, gerenciamento de recursos e sincronização.

## Descrição

Durante a disciplina, foram implementados diversos sistemas e algoritmos para o controle de tarefas em tempo real utilizando o sistema operacional **FreeRTOS**. O microcontrolador **RP2040** foi utilizado como base para a execução dos projetos, aproveitando suas duas CPUs e diversos periféricos.

### Ferramentas Utilizadas:
- **Plataforma:** Raspberry Pi Pico (RP2040)
- **Sistema Operacional:** FreeRTOS
- **IDE:** Visual Studio Code com PlatformIO
- **Linguagem:** C/C++

### Funcionalidades Implementadas:
- Criação e gerenciamento de múltiplas tarefas
- Uso de semáforos para sincronização entre tarefas
- Implementação de filas para troca de mensagens entre tarefas
- Controle de LEDs e leitura de sensores (ADC) com FreeRTOS

# Configuração do Ambiente FreeRTOS no RP2040 (Linux)
## 1. Instalar Ferramentas Necessárias

Execute os seguintes comandos no terminal para instalar as ferramentas essenciais:

```bash
sudo apt update
sudo apt install cmake gcc-arm-none-eabi build-essential
```
## 2. Baixar e Configurar o FreeRTOS e o Pico SDK
### Passo 1: Clone os Repositórios
Clone os repositórios do Pico SDK e do FreeRTOS Kernel com os submódulos:
```bash
git clone https://github.com/raspberrypi/pico-sdk --recurse-submodules
git clone -b smp https://github.com/FreeRTOS/FreeRTOS-Kernel --recurse-submodules
```
### Passo 2: Configurar Variáveis de Ambiente
Você pode adicionar estas linhas ao arquivo ~/.bashrc para que sejam carregadas automaticamente em novas sessões de terminal:
```bash
echo "export PICO_SDK_PATH=$PWD/pico-sdk" >> ~/.bashrc
echo "export FREERTOS_KERNEL_PATH=$PWD/FreeRTOS-Kernel" >> ~/.bashrc
source ~/.bashrc
```

## 3. Criar e Compilar o Projeto

### Passo 1: Estrutura do Projeto
Crie uma estrutura de diretórios para o seu projeto e adicione o arquivo `main.c` com o código FreeRTOS desejado.
Exemplo da estrutura de diretórios:

```
/meu_projeto 
├── CMakeLists.txt 
└── main.c
└── FreeRTOSConfig.h
```
### Passo 2: CMakeLists.txt

Crie um arquivo `CMakeLists.txt` para definir o projeto. Um exemplo básico seria:

```cmake
cmake_minimum_required(VERSION 3.13)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(meu_projeto)

pico_sdk_init()

add_executable(meu_projeto main.c)

target_link_libraries(meu_projeto pico_stdlib FreeRTOS-Kernel)

pico_add_extra_outputs(meu_projeto)
```

### Passo 3: Compilar o Projeto
No terminal, navegue até a pasta do projeto e execute os seguintes comandos para compilar:

```bash
mkdir build
cd build
cmake ..
make
```

## 4. Fazer Upload para a Pico
### Passo 1: Modo BOOTSEL
Coloque o Raspberry Pi Pico em BOOTSEL mode:

1. Desconecte o Pico do USB.
2. Pressione e segure o botão BOOTSEL.
3. Conecte o Pico ao PC mantendo o botão pressionado até o drive ser montado no seu sistema.
### Passo 2: Copiar o Arquivo .uf2
Copie o arquivo .uf2 gerado para o drive montado da Pico (ele aparecerá como um dispositivo de armazenamento USB):

```bash
cp build/meu_projeto.uf2 /media/$USER/RPI-RP2
```
