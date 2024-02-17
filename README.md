# Blue Jamma
## Uma placa para ligar o PC em um arcade no padrão Jamma
  
A Blue Jamma é um projeto open-hardware baseada no microcontrolador STM32F103C8T6, o mesmo utilizado nas placas de avaliação "Blue Pill", de onde herdou seu nome e coloração.  
  
Utiliza todos os GPIOs disponíveis na MCU para suportar todos botões Jamma. Para boa exibição em monitores Arcade, possui um amplificador de vídeo THS7314, com regulagem de ganho individual nas cores e um atenuador para o sincronismo. Para áudio, utiliza o módulo amplificador digital PAM8610.  
  
![Blue Jamma Fulli Assembled](https://github.com/ninomegadriver/BlueJamma/blob/main/images/BlueJammaAssembled.jpg?raw=true)  
---------
A ligação dos controles no PC é feita através de uma porta USB que, preferencialmente, é configurada como um [teclado](https://github.com/ninomegadriver/BlueJamma/tree/main/Blue_Jamma_as_Keyboard), com os botões mapeados no padrão do emulador M.A.M.E.  
  
Segue abaixo relação dos Pinos vs GPIO vs Teclas  

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
  
O [firmware](https://github.com/ninomegadriver/BlueJamma/tree/main/Blue_Jamma_as_Keyboard) também possui algumas combinações de botões, para facilitar a navegação através do M.A.M.E. e outros programas ou jogos:

| Combinação de Botões               | Tecla enviada       |
|------------------------------------|---------------------|
| Player 1 Cima + Player 1 Start     | TAB (M.A.M.E. Menu) |
| Player 1 Baixo + Player 1 Start    | ESC                 |
| Player 2 Esquerda + Player 1 Start | ESC                 |
| Player 2 Direita + Player 2 Start  | Enter               |
  
  **CURIOSIDADE:** Este [firmware](https://github.com/ninomegadriver/BlueJamma/tree/main/Blue_Jamma_as_Keyboard) também é compatível com as placas genéricas "Blue Pill". Se você possui uma e gostaria de utilizar este firmware nela, ao invés da Blue Jamma, fique a vontade. Basta utilizar a mesma configuração de GPIOs nos botões ou editar o código fonte como desejado.  
  
---------
  O objetivo da Blue Jamma é obter o melhor resultado de imagem no monitor arcade. Então, a porta VGA da Blue Jamma **NÃO POSSUI CONVERSOR DE VÍDEO**, o sinal deve ser enviado por **placas que suportam saída analógica na faixa de frequência suportada por monitores arcade**. Porém, ela possui um circuito para amplificação e balanceamento do sinal de vídeo, baseado no THS7314, para obter a melhor imagem possível no monitor arcade.  
  
  A Blue Jamma é compatível com os modos de vídeo "15KHz" gerados pelo **Groovy Arcade**. Se sua placa de vídeo é capaz de exibir os modos 224p, 240p e 480i do Groovy Arcade, você pode utilizar a solução de vídeo presente na Blue Jamma.
  
  **Placas NVIDIA com saída VGA (ou DVI-I via adaptador) são capazes de exibir resoluções compatíveis utilizando o driver oficial da GeForce**. Segue abaixo um passo-a-passo de como configurar a resolução na versão Windows do Driver:  
    
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

### 7 - Et voilà! Seu arcade foi contaminado pela Microsoft!

![Passo 7](https://github.com/ninomegadriver/BlueJamma/blob/main/images/VGA-HowTo-Step07.jpg?raw=true)  
  
### EXTRA: Segue abaixo os timings de uma resolução em 480i caso necessitar para jogos com resoluções de 24Khz. Notem o checkbox de interpolado está ativado.
  
![Passo 8](https://github.com/ninomegadriver/BlueJamma/blob/main/images/VGA-HowTo-Step08.jpg?raw=true)  
  
  