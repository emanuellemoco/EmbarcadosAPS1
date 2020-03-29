# EmbarcadosAPS1
APS 1 -  Sistema embarcado que reproduz uma música monofonia
Este é um projeto no Atmel Studio utiliza de um buzzer, um placa externa com 3 botões, um LCD e uma placa ATSAME70Q21. Ele se baseia em tocar músicas 8 bits por meio do buzzer, tendo a disposição apenas listas de frequências de cada nota e os tempos que cada nota toca.
Nele tocamos apenas 3 músicas a música tema clássica do super mario bros (nes), underworld também do super mario bros (nes) e por último a imperial march do Star Wars.


Para rodar este projeto é necessário alguns passos:

1- Abrir o projeto SAME70 no Atmel Studio.

2- Conectar a placa ATSAME70Q21 no seu computador.

3- Conectar o buzzer na placa na entrada PC13 e no VCC.

4- Conectar a placa exterior na placa ATSAME70Q21.

5- Dar Start no Atmel Studio.

Como funciona:
Basicamente cada um dos três botões da placa exterior toca uma música diferente, equanto cada musíca toca o led da placa principal pisca conforme mudam as notas da música. Para pausar a música basta apertar o botão da placa principal ao lada do botão de reset, para continuar aperte denovo o mesmo botão que a música voltara a tocar.

Explição do software:
A primeira parte do código envolve basicamente em definições de todas as coisas que vamos usar na lógica posteriormente, como portas dos leds, botões e o buzzer assim como um struct para músicas, definições de interrupções(callbacks) e inicialização de portas.

A função playMusic percorre a lista de notas e tempo tocando cada frequência pela duração dela, tendo em vista também que identifica o callback do pause e fica preso no loop até voltar o botão ser apertado novamente ou se outro botão for apertado assim dando break no loop para mudar a música e por último ela também liga e desliga o led conforme a música.

A int main cria os objetos de cada música conforme a struct e roda o while(1) que identifica as flags de callback de cada botão da placa externa e chama a função playMusic para tocar a respectiva música.

Por último  temos também o arquivo musicas.h que tem o define de todas as notas usadas com as suas frequências e as listas tempo e notas de cada música.
