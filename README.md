# EmbarcadosAPS1
APS 1 -  Sistema embarcado que reproduz uma música monofonia
Este é um projeto no Atmel Studio utiliza de um buzzer, uma placa ATSAME70Q21, um OLED com 3 botões e um LCD. Ele se baseia em tocar músicas 8 bits por meio do buzzer, tendo a disposição apenas listas de frequências de cada nota e os tempos que cada nota toca.
Nele tocamos apenas 3 músicas a música tema clássica do super mario bros (nes), underworld também do super mario bros (nes) e por último a imperial march do Star Wars.

Requisitos: 

- SAME70-XPLD
- OLED1 XPLAINED PRO
- BUZZER
- Atmel Studio

Para rodar este projeto é necessário alguns passos:

1- Abrir o projeto OLED1 no Atmel Studio.

2- Conectar a placa SAME70-XPLD no seu computador na saída DEBUG USB.

3- Conectar o buzzer na placa na SAME70-XPLD, sendo o +VCC no pino PC13 e -VCC no pino +VCC.

4- Conectar o OLED1 XPLAINED PRO na saída EXT1 da SAME70-XPLD.

5- Dar Start no Atmel Studio.

Como funciona:
Basicamente cada um dos três botões da OLED1 XPLAINED PRO faz tocar uma música diferente, sendo:
OLED1 Button 1 : música tema do Mário
OLED1 Button 2 : Imperial March 
OLED1 Button 3 : Underworld
Quando tocada, cada musíca faz o led da SAME70-XPLD piscar conforme mudam as notas da música e o Led respectivo de cada botão da OLED1 fica ligado, indicando qual música está tocando. 
Para pausar a música, basta apertar o botão SW300 da SAME70-XPLD, para continuar, aperte denovo o mesmo botão que a música voltara a tocar.
Também é possível trocar de música enquanto uma música ainda está tocando, basta pressionar o botão da outra música desejada.

No OLED display aparece uma mensagem para o usuário escolher a música quando não há nada tocando, e mostra o nome da música que está tocando.

Explição do software:
A primeira parte do código envolve basicamente definições de todas as coisas que vamos usar na lógica posteriormente, como portas dos leds, botões e o buzzer assim como um struct para músicas, definições de interrupções(callbacks) e inicialização de portas.

A função playMusic percorre o array de notas e tempo tocando cada frequência pela duração dela, tendo em vista também que identifica o callback do pause e fica preso no loop até voltar o botão ser apertado novamente ou se outro botão for apertado assim dando break no loop para mudar a música e por último ela também liga e desliga o led indicando qual música que esta tocando.

A função main cria os objetos de cada música conforme a struct e roda o while(1) que identifica as flags de callback de cada botão da placa externa e chama a função playMusic para tocar a respectiva música escolhida. 

Por último, temos também o arquivo musicas.h que define de todas as notas usadas, bem como arrays para cada música com a suas respectivas notas e tempos.

O código está com as músicas organizadas em vetores de structs (ponteiros), sendo que as funções recebem o struct que contém a música a ser tocada.

Link para o vídeo do projeto funcionando:
