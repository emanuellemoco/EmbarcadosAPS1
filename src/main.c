/**
 * 5 semestre - Eng. da Computação - Insper
 * Rafael Corsi - rafael.corsi@insper.edu.br
 *
 * Projeto 0 para a placa SAME70-XPLD
 *
 * Objetivo :
 *  - Introduzir ASF e HAL
 *  - Configuracao de clock
 *  - Configuracao pino In/Out
 *
 * Material :
 *  - Kit: ATMEL SAME70-XPLD - ARM CORTEX M7
 */

/************************************************************************/
/* includes                                                             */
/************************************************************************/

#include "asf.h"

/************************************************************************/
/* defines                                                              */
/************************************************************************/

	#define LED_PIO           PIOC                 // periferico que controla o LED
	#define LED_PIO_ID        12                  // ID do periférico PIOC (controla LED)
	#define LED_PIO_IDX       8                    // ID do LED no PIO
	#define LED_PIO_IDX_MASK  (1 << LED_PIO_IDX)   // Mascara para CONTROLARMOS o LED

	// Configuracoes do botao
	#define BUT_PIO			  PIOA
	#define BUT_PIO_ID		  10
	#define BUT_PIO_IDX		  11
	#define BUT_PIO_IDX_MASK (1u << BUT_PIO_IDX)

	//.Configuracoes do buzzer no pino PD11
 	#define BUZ_PIO			  PIOC
 	#define BUZ_PIO_ID		  ID_PIOC
 	#define BUZ_PIO_IDX		  13
 	#define BUZ_PIO_IDX_MASK (1 << BUZ_PIO_IDX)
	
	//Musica Mario (fonte: https://www.princetronics.com/supermariothemesong/) //arrumar fonte

	#define NOTE_C4  262   //Defining note frequency
	#define NOTE_D4  294
	#define NOTE_E4  330
	#define NOTE_F4  349
	#define NOTE_G4  392
	#define NOTE_A4  440
	#define NOTE_B4  494
	#define NOTE_C5  523
	#define NOTE_D5  587
	#define NOTE_E5  659
	#define NOTE_F5  698
	#define NOTE_G5  784
	#define NOTE_A5  880
	#define NOTE_B5 988
	#define c  261
	#define d  294
	#define e  329
	#define f  349
	#define g  391
	#define gS  415
	#define a  440
	#define aS  455
	#define b  466
	#define cH  523
	#define cSH  554
	#define dH  587
	#define dSH  622
	#define eH  659
	#define fH  698
	#define fSH  740
	#define gH  784
	#define gSH  830
	#define aH  880
	#define NOTE_B0  31
	#define NOTE_C1  33
	#define NOTE_CS1 35
	#define NOTE_D1  37
	#define NOTE_DS1 39
	#define NOTE_E1  41
	#define NOTE_F1  44
	#define NOTE_FS1 46
	#define NOTE_G1  49
	#define NOTE_GS1 52
	#define NOTE_A1  55
	#define NOTE_AS1 58
	#define NOTE_B1  62
	#define NOTE_C2  65
	#define NOTE_CS2 69
	#define NOTE_D2  73
	#define NOTE_DS2 78
	#define NOTE_E2  82
	#define NOTE_F2  87
	#define NOTE_FS2 93
	#define NOTE_G2  98
	#define NOTE_GS2 104
	#define NOTE_A2  110
	#define NOTE_AS2 117
	#define NOTE_B2  123
	#define NOTE_C3  131
	#define NOTE_CS3 139
	#define NOTE_D3  147
	#define NOTE_DS3 156
	#define NOTE_E3  165
	#define NOTE_F3  175
	#define NOTE_FS3 185
	#define NOTE_G3  196
	#define NOTE_GS3 208
	#define NOTE_A3  220
	#define NOTE_AS3 233
	#define NOTE_B3  247
	#define NOTE_C4  262
	#define NOTE_CS4 277
	#define NOTE_D4  294
	#define NOTE_DS4 311
	#define NOTE_E4  330
	#define NOTE_F4  349
	#define NOTE_FS4 370
	#define NOTE_G4  392
	#define NOTE_GS4 415
	#define NOTE_A4  440
	#define NOTE_AS4 466
	#define NOTE_B4  494
	#define NOTE_C5  523
	#define NOTE_CS5 554
	#define NOTE_D5  587
	#define NOTE_DS5 622
	#define NOTE_E5  659
	#define NOTE_F5  698
	#define NOTE_FS5 740
	#define NOTE_G5  784
	#define NOTE_GS5 831
	#define NOTE_A5  880
	#define NOTE_AS5 932
	#define NOTE_B5  988
	#define NOTE_C6  1047
	#define NOTE_CS6 1109
	#define NOTE_D6  1175
	#define NOTE_DS6 1245
	#define NOTE_E6  1319
	#define NOTE_F6  1397
	#define NOTE_FS6 1480
	#define NOTE_G6  1568
	#define NOTE_GS6 1661
	#define NOTE_A6  1760
	#define NOTE_AS6 1865
	#define NOTE_B6  1976
	#define NOTE_C7  2093
	#define NOTE_CS7 2217
	#define NOTE_D7  2349
	#define NOTE_DS7 2489
	#define NOTE_E7  2637
	#define NOTE_F7  2794
	#define NOTE_FS7 2960
	#define NOTE_G7  3136
	#define NOTE_GS7 3322
	#define NOTE_A7  3520
	#define NOTE_AS7 3729
	#define NOTE_B7  3951
	#define NOTE_C8  4186
	#define NOTE_CS8 4435
	#define NOTE_D8  4699
	#define NOTE_DS8 4978
		int mario_theme_notes[] = { //by Prince @ princetronics.com
			NOTE_E7, NOTE_E7, 0, NOTE_E7,
			0, NOTE_C7, NOTE_E7, 0,
			NOTE_G7, 0, 0,  0,
			NOTE_G6, 0, 0, 0,
			
			NOTE_C7, 0, 0, NOTE_G6,
			0, 0, NOTE_E6, 0,
			0, NOTE_A6, 0, NOTE_B6,
			0, NOTE_AS6, NOTE_A6, 0,
			
			NOTE_G6, NOTE_E7, NOTE_G7,
			NOTE_A7, 0, NOTE_F7, NOTE_G7,
			0, NOTE_E7, 0, NOTE_C7,
			NOTE_D7, NOTE_B6, 0, 0,
			
			NOTE_C7, 0, 0, NOTE_G6,
			0, 0, NOTE_E6, 0,
			0, NOTE_A6, 0, NOTE_B6,
			0, NOTE_AS6, NOTE_A6, 0,
			
			NOTE_G6, NOTE_E7, NOTE_G7,
			NOTE_A7, 0, NOTE_F7, NOTE_G7,
			0, NOTE_E7, 0, NOTE_C7,
			NOTE_D7, NOTE_B6, 0, 0
		};
		//Mario main them tempo by Prince
		int mario_theme_tempo[] = {
			83, 83, 83, 83,
			83, 83, 83, 83,
			83, 83, 83, 83,
			83, 83, 83, 83,
			
			83, 83, 83, 83,
			83, 83, 83, 83,
			83, 83, 83, 83,
			83, 83, 83, 83,
			
			112, 112, 112,
			83, 83, 83, 83,
			83, 83, 83, 83,
			83, 83, 83, 83,
			
			83, 83, 83, 83,
			83, 83, 83, 83,
			83, 83, 83, 83,
			83, 83, 83, 83,
			
			112, 112, 112,
			83, 83, 83, 83,
			83, 83, 83, 83,
			83, 83, 83, 83,
		};
/************************************************************************/
/* constants                                                            */
/************************************************************************/

/************************************************************************/
/* variaveis globais                                                    */
/************************************************************************/

/************************************************************************/
/* prototypes                                                           */
/************************************************************************/

void init(void);

/************************************************************************/
/* interrupcoes                                                         */
/************************************************************************/

/************************************************************************/
/* funcoes                                                              */
/************************************************************************/

// Função de inicialização do uC e config correta dos perifericos e pinos
void init(void)
{
	//inicializa board clock
	sysclk_init();
	
	//desativa WatchDog Timer
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	//Ativa o PIO na qual o LED foi conectado
	// para que possamos controlar o LED
	pmc_enable_periph_clk(LED_PIO_ID);
	
	//Inicializa o PC8 como saida
	pio_set_output(LED_PIO, LED_PIO_IDX_MASK, 0, 0, 0);
	
	// Inicializa PIO do botao
	pmc_enable_periph_clk(BUT_PIO_ID);
	
	// configura pino ligado ao botão como entrada com um pull-up.
	pio_set_input(BUT_PIO, BUT_PIO_IDX_MASK, PIO_DEFAULT);
	
	//Ativar o pull-up
	pio_pull_up(BUT_PIO, BUT_PIO_IDX_MASK, 1);
	
	//BUZZER
	pmc_enable_periph_clk(BUZ_PIO_ID); //inicializa pio do buzzer
	pio_set_output(BUZ_PIO, BUZ_PIO_IDX_MASK, 0, 0,0 );
}

/************************************************************************/
/* Main                                                                 */
/************************************************************************/

// Funcao principal chamada na inicalizacao do uC.
int main(void)
{
	// inicializa sistema e IOs
	init();
	// super loop
	// aplicacoes embarcadas não devem sair do while(1).
	
	while(1){
//						pio_set(PIOC, LED_PIO_IDX_MASK);      // Coloca 1 no pino LED
// 						pio_set(PIOC, BUZ_PIO_IDX_MASK);		  //.
// 						delay_ms(2);                        // Delay por software de 200 ms
// 						pio_clear(PIOC, LED_PIO_IDX_MASK);    // Coloca 0 no pino do LED
// 						pio_clear(PIOC, BUZ_PIO_IDX_MASK);	  //.
// 						delay_ms(2);                        // Delay por software de 200 ms

//arrrumar o 78(tamanho de tempo)


// 	for (int i=0 ;  i< 78 ; i++){
// 			float qtd;
// 			if (melody[i]!=0){float qtd = tempo[i]/ ( ((1.0/(float)(melody[i]))*1000000.0) * 2) ;}
// 			else{						delay_ms(tempo[i]); }
// 				//if (melody[i]!=0){
// 				for (int j=0 ; j <= qtd ; j++){
// 					//if (melody[i]!=0){
// 						pio_set(PIOC, LED_PIO_IDX_MASK);      // Coloca 1 no pino LED
// 						pio_set(PIOC, BUZ_PIO_IDX_MASK);
// 						delay_us( (1.0/ (melody[i]) ) *1000000);
// 						pio_clear(PIOC, LED_PIO_IDX_MASK);    // Coloca 0 no pino do LED
// 						pio_clear(PIOC, BUZ_PIO_IDX_MASK);
// 						delay_us( (1.0/ (melody[i]) ) *1000000);
// 					//}
// 				}
// 				//delay_ms(tempo[i] * 10.3);
// 				}	
// 				

	int size_mario = sizeof(mario_theme_tempo) / sizeof(int);
	for (int i=0 ;  i< size_mario ; i++){
		if (mario_theme_notes[i] == 0 ){
				delay_ms(mario_theme_tempo[i]);
		}
		
		else {
			float t_delay = 1000.0 /(int)(mario_theme_notes[i])	;
			for (long j=0 ; j < (mario_theme_tempo[i]/t_delay ); j++){
				if (mario_theme_tempo[i]!= 0){
					pio_set(PIOC, LED_PIO_IDX_MASK);      // Coloca 1 no pino LED
					pio_set(PIOC, BUZ_PIO_IDX_MASK);		  
					delay_us(t_delay*1000.0);
					pio_clear(PIOC, LED_PIO_IDX_MASK);    // Coloca 0 no pino do LED
					pio_clear(PIOC, BUZ_PIO_IDX_MASK);	  
					delay_us(t_delay*1000.0);
				} 				
			}
		}
		}
	}
	return 0;
}
