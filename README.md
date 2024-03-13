# Blue Jamma
## Uma placa para ligar o PC em um arcade no padrão Jamma
  
### INTRODUÇÃO  
------------------------
A Blue Jamma é um projeto open-hardware baseada no microcontrolador STM32F103C8T6, o mesmo utilizado nas placas de avaliação "Blue Pill", de onde herdou seu nome e coloração.  
  
Utiliza todos os GPIOs disponíveis na MCU para suportar todos botões Jamma. Para boa exibição em monitores Arcade, possui um amplificador de vídeo THS7314, com regulagem de ganho individual nas cores e um atenuador para o sincronismo. Para áudio, utiliza o módulo amplificador digital PAM8610.  
  
![Blue Jamma Fulli Assembled](https://github.com/ninomegadriver/BlueJamma/blob/main/images/BlueJammaAssembled.jpg?raw=true)  
  
------------------------
### CONEXÃO USB COM O PC
------------------------
  
A ligação dos controles no PC é feita através de uma porta USB que, para tal, estão disponibilizados três firmwares:  
  
| Firmware | Setup |
|----------|-------|
| [Blue_Jamma_as_Xinput.bin](https://github.com/ninomegadriver/BlueJamma/blob/main/Blue_Jamma_as_Xinput.bin?raw=true) | Dois controles Xinput detectados pelo PC como controles de Xbox 360 |  
| [Blue_Jamma_as_MAME_Keyboard.bin](https://github.com/ninomegadriver/BlueJamma/blob/main/Blue_Jamma_as_MAME_Keyboard.bin?raw=true) | Um teclado com os botões mapeados utilizando as teclas no padrão do emulador MAME |  
| [Blue_Jamma_as_Simple_Keyboard.bin](https://github.com/ninomegadriver/BlueJamma/blob/main/Blue_Jamma_as_Simple_Keyboard.bin?raw=true) | Um teclado com os botões mapeados utilizando apenas teclas simples, letras, para evitar problemas típicos com alt, ctrl, etc |  
  
  Para gravar o Firmware na Blue Jamma recomendo utilizar o acessório ST-Link e a ferranenta de linha de comando "st-flash". Exemplo do comando "st-flash":  
  ```st-flash write Blue_Jamma_as_Xinput.bin 0x8000000 ```

  **Compatibilidade com a Blue Pill:** Estes firmwares também são compatíveis com as placas genéricas "**Blue Pill**". Se você possui uma e gostaria de utilizar alguns destes firmwares nela ao invés da Blue Jamma, fique a vontade. Basta utilizar a mesma configuração de GPIOs nos botões ou editar o código fonte como desejado. De qualquer forma, note que a Blue Jamma possui resistores pullups em todas as portas Jamma para um melhor controle de corrente. **Logo, se for utilizar uma Blue Pill, utilize a versão do firmware com "INPUT_PULLUP" no nome do arquivo**:      
  
  Versões para Blue Pill:  
  [Blue_Jamma_as_Xinput-INPUT_PULLUP.bin](https://github.com/ninomegadriver/BlueJamma/blob/main/Blue_Jamma_as_Xinput-INPUT_PULLUP.bin?raw=true)  
  [Blue_Jamma_as_MAME_Keyboard-INPUT_PULLUP.bin](https://github.com/ninomegadriver/BlueJamma/blob/main/Blue_Jamma_as_MAME_Keyboard-INPUT_PULLUP.bin?raw=true)  
  [Blue_Jamma_as_Simple_Keyboard-INPUT_PULLUP.bin](Blue_Jamma_as_Simple_Keyboard-INPUT_PULLUP.bin?raw=true)  
  
  
Segue abaixo relação dos Pinos vs GPIO vs Função  

### Blue_Jamma_as_Xinput.bin
| Função Jamma | GPIO STM32 | Botão            |
|--------------|------------|------------------|
Serviço        | PA0        | Player 1 click do analógico esquerdo  |
Teste          | PA1        | Player 1 click do analógico direito   |
Ficha          | PA2        | Player 1 Back                         |
P2 START       | PA3        | Player 2 Start                        |
P1 START       | PA4        | Player 1 Start                        |
P2 UP          | PA5        | Player 2 Direcional Cima              |
P1 UP          | PA6        | Player 1 Direcional Cima              |
P2 DOWN        | PA7        | Player 2 Direcional Baixo             |
P1 DOWN        | PB0        | Player 1 Direcional Baixo             |
P2 LEFT        | PB1        | Player 2 Direcional Esqurda           |
P1 LEFT        | PB10       | Player 1 Direcional Esquerda          |
P2 RIGHT       | PB11       | Player 2 Direcional Direita           |
P1 RIGHT       | PB12       | Player 2 Direcional Direita           |
P2 Button 1    | PB13       | Player 2 X  |
P1 Button 1    | PB14       | Player 1 X  |
P2 Button 2    | PB15       | Player 2 Y  |
P1 Button 2    | PA8        | Player 1 Y  |
P2 Button 3    | PA15       | Player 2 RB |
P1 Button 3    | PB3        | Player 1 RB |
P2 Button 4    | PB4        | Player 2 A  |
P1 Button 4    | PB5        | Player 1 A  |
P2 Button 5    | PB6        | Player 2 B  |
P1 Button 5    | PB7        | Player 1 B  |
P2 Button 6    | PB8        | Player 2 R Trigger |
P1 Button 6    | PB9        | Player 1 R Trigger |
  
  Para facilitar a configuração e aumentar a compatibilidade, os pios PA9 e PA10 também estão configurados conforme abaixo:
  
| GPIO STM32 | Função |  
|------------|--------|  
| PA9        | Conecte ao terra para inverter a órdem dos controles. Muitas vezes o sistema operacional ou algum jogo específico pode alternar de maneira aleatória a ordem dos controles USB. Com esta funcionalidade, basta adicionar uma chave à este GPIO para alternar o player1 com o player2 sem precisar reiniciar o computador ou o jogo |  
| PA10       | Quando conectado ao terra, o directional do Jamma corresponte ao DPAD do controlte de X360. Quando flutuante ou desconectado, o direcional do Jamma funcionará como o analógico esquerdo. Muito útil quando precisar alternar entre jogos de luta ou de corrida, por exemplo |  
  
### Blue_Jamma_as_Simple_Keyboard.bin
| Função Jamma | GPIO STM32 | Tecla            |
|--------------|------------|------------------|
Serviço        | PA0        | q                |
Teste          | PA1        | w                |
Ficha          | PA2        | e                |
P2 START       | PA3        | t                |
P1 START       | PA4        | ENTER            |
P2 UP          | PA5        | y                |
P1 UP          | PA6        | Seta Cima        |
P2 DOWN        | PA7        | u                |
P1 DOWN        | PB0        | Seta Bairo       |
P2 LEFT        | PB1        | i                |
P1 LEFT        | PB10       | Seta Esquerda    |
P2 RIGHT       | PB11       | o                |
P1 RIGHT       | PB12       | Seta Direita     |
P2 Button 1    | PB13       | p                |
P1 Button 1    | PB14       | a                |
P2 Button 2    | PB15       | s                |
P1 Button 2    | PA8        | d                |
P2 Button 3    | PA15       | f                |
P1 Button 3    | PB3        | g                |
P2 Button 4    | PB4        | h                |
P1 Button 4    | PB5        | j                |
P2 Button 5    | PB6        | k                |
P1 Button 5    | PB7        | l                |
P2 Button 6    | PB8        | z                |
P1 Button 6    | PB9        | x                |
  
### Blue_Jamma_as_MAME_Keyboard.bin
| Função Jamma | GPIO STM32 | Tecla            |
|--------------|------------|------------------|
Serviço        | PA0        | F2               |
Teste          | PA1        | 9                |
Ficha          | PA2        | 5                |
P2 START       | PA3        | 2                |
P1 START       | PA4        | 1                |
P2 UP          | PA5        | r                |
P1 UP          | PA6        | Seta Cima        |
P2 DOWN        | PA7        | f                |
P1 DOWN        | PB0        | Seta Bairo       |
P2 LEFT        | PB1        | d                |
P1 LEFT        | PB10       | Seta Esquerda    |
P2 RIGHT       | PB11       | g                |
P1 RIGHT       | PB12       | Seta Direita     |
P2 Button 1    | PB13       | a                |
P1 Button 1    | PB14       | Control Esquerdo |
P2 Button 2    | PB15       | s                |
P1 Button 2    | PA8        | Alt Esquerdo     |
P2 Button 3    | PA15       | q                |
P1 Button 3    | PB3        | Espaço           |
P2 Button 4    | PB4        | w                |
P1 Button 4    | PB5        | Shift Esquerdo   |
P2 Button 5    | PB6        | e                |
P1 Button 5    | PB7        | z                |
P2 Button 6    | PB8        | h                |
P1 Button 6    | PB9        | z                |
  
Ambos os firmwares de teclado possuem algumas combinações de botões, para facilitar a navegação através do M.A.M.E. e outros programas ou jogos:

| Combinação de Botões               | Tecla enviada       |
|------------------------------------|---------------------|
| Player 1 Cima + Player 1 Start     | TAB (M.A.M.E. Menu) |
| Player 1 Baixo + Player 1 Start    | ESC                 |
| Player 2 Esquerda + Player 1 Start | ESC                 |
| Player 2 Direita + Player 2 Start  | Enter               |
  
  
  ------------------------
  ### CONEXÃO VGA COM O PC
  ------------------------
    
  O objetivo da Blue Jamma é obter o melhor resultado de imagem no monitor arcade. Então, a porta VGA da Blue Jamma **NÃO POSSUI CONVERSOR DE VÍDEO**, o sinal deve ser enviado por **placas que suportam saída analógica na faixa de frequência suportada por monitores arcade**. Porém, ela possui um circuito para amplificação e balanceamento do sinal de vídeo, baseado no THS7314, para obter a melhor imagem possível no monitor arcade.  
  
  A Blue Jamma é compatível com os modos de vídeo "15KHz" gerados pelo [Groovy Arcade](https://github.com/substring/os). Se sua placa de vídeo é capaz de exibir os modos 224p, 240p e 480i do [Groovy Arcade](https://github.com/substring/os), você pode utilizar a solução de vídeo presente na Blue Jamma.
  
  **Placas NVIDIA com saída VGA (ou DVI-I via adaptador) são capazes de exibir resoluções compatíveis utilizando o driver oficial da GeForce**. Segue abaixo um passo-a-passo de como configurar a resolução na versão para Windows do **driver oficial da NVIDIA**:  
    
### 1 - Clique com o botão esquerdo do mouse em seu desktop e entre no painel de controle da NVIDIA
    
![Passo 1](https://github.com/ninomegadriver/BlueJamma/blob/main/images/VGA-HowTo-Step01.jpg?raw=true)
  
### 2 - Se sua conexão estiver realizada de forma correta, um monitor genérico analógico irá aparecer. Na tela de configuração de resolução, clique em customizar.

![Passo 2](https://github.com/ninomegadriver/BlueJamma/blob/main/images/VGA-HowTo-Step02.jpg?raw=true)  
  
### 3 - Selecione a opção para exibir todos os modos. Depois clique no botão para criar uma nova resolução personalizada.
  
![Passo 3](https://github.com/ninomegadriver/BlueJamma/blob/main/images/VGA-HowTo-Step03.jpg?raw=true)  

### 4 - Para o padrão 384x224@60, utilizado nas placas CPS2, Neo Geo e muitas outras, utilize os timings abaixo. Quanto for testar resoluções diferentes, atente-se na polaridade dos pulsos, monitores arcade utilizam sempre **negativo**. Após configurado todos os parâmetros clique em "Testar" e faça a regulagem do seu monitor. 

![Passo 4](https://github.com/ninomegadriver/BlueJamma/blob/main/images/VGA-HowTo-Step04.jpg?raw=true)  

### 5 - Se estiver satisfeito, basta clicar em Ok e fechar o painel de controle da NVIDIA.

![Passo 5](https://github.com/ninomegadriver/BlueJamma/blob/main/images/VGA-HowTo-Step05.jpg?raw=true)  
  
### 6 - Para a maioria das resoluções, ela deverá aparecer na lista. Porém, já aconteceu de eu adicionar uma resolução muito fora do padrão e ela não aparecer na lista. Mas, a imagem sendo exibida como esperado no monitor arcade, podemos fechar o aplicativo que ele continuará configurado, mesmo após reiniciar o PC.  
  
![Passo 6](https://github.com/ninomegadriver/BlueJamma/blob/main/images/VGA-HowTo-Step06.jpg?raw=true)  

### 7 - Et voilà! Seu arcade foi contaminado pelo vírus do Bill Gates :D

![Passo 7](https://github.com/ninomegadriver/BlueJamma/blob/main/images/VGA-HowTo-Step07.jpg?raw=true)  
  
### EXTRA: Segue abaixo os timings de uma resolução em 480i, para jogos com resoluções de 24Khz. Notem que "Interlaced" está selecionado ao invés de "Progressive"
  
![Passo 8](https://github.com/ninomegadriver/BlueJamma/blob/main/images/VGA-HowTo-Step08.jpg?raw=true)  
  
### Observação Adicional:  
Neste repositório também está disponibilizada a ferramenta antiga, porém super prática, "Custom Resolution Utility (CRU)", na versão binária e código fonte.  
  
----------------------
### ESQUEMA ELETRÔNICO
----------------------
  
![GPIO](https://github.com/ninomegadriver/BlueJamma/blob/main/images/schematic-gpio.jpg?raw=true)  
  
![Oscillator](https://github.com/ninomegadriver/BlueJamma/blob/main/images/schematic-oscillator.jpg?raw=true)  

![Power](https://github.com/ninomegadriver/BlueJamma/blob/main/images/schematic-power.jpg?raw=true)  

![Video](https://github.com/ninomegadriver/BlueJamma/blob/main/images/schematic-video.jpg?raw=true)  

![USB](https://github.com/ninomegadriver/BlueJamma/blob/main/images/schematic-usb.jpg?raw=true)  

![flash](https://github.com/ninomegadriver/BlueJamma/blob/main/images/schematic-flash.jpg?raw=true)  

![audio](https://github.com/ninomegadriver/BlueJamma/blob/main/images/schematic-audio.jpg?raw=true)  
  
-----------------
### GLAMOUR SHOTS
-----------------

![Glamour Shot](https://github.com/ninomegadriver/BlueJamma/blob/main/images/BlueJammaInShell1.jpg?raw=true)  
![Glamour Shot](https://github.com/ninomegadriver/BlueJamma/blob/main/images/BlueJammaInShell2.jpg?raw=true)  
![Glamour Shot](https://github.com/ninomegadriver/BlueJamma/blob/main/images/BlueJammaInShell3.jpg?raw=true)  
![Glamour Shot](https://github.com/ninomegadriver/BlueJamma/blob/main/images/pcb-cad-top.jpg?raw=true)  
![Glamour Shot](https://github.com/ninomegadriver/BlueJamma/blob/main/images/pcbs.jpg?raw=true)  
![Glamour Shot](https://github.com/ninomegadriver/BlueJamma/blob/main/images/pcb-top.jpg?raw=true)  
  
-----------------
Nino MegaDriver - 2024  
nino@nino.com.br  
-----------------