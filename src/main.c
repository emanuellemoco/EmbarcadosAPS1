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
#include "musicas.h"
// #include "gfx_mono_ug_2832hsweg04.h"
// #include "gfx_mono_text.h"
// #include "sysfont.h"

/************************************************************************/
/* defines                                                              */
/************************************************************************/
	
// Configuracoes do led same70
#define LED_PIO           PIOC                 // periferico que controla o LED
#define LED_PIO_ID        12                  // ID do periférico PIOC (controla LED)
#define LED_PIO_IDX       8                    // ID do LED no PIO
#define LED_PIO_IDX_MASK  (1 << LED_PIO_IDX)   // Mascara para CONTROLARMOS o LED

// Configuracoes dos LEDs
#define LED1_PIO			PIOA
#define LED1_PIO_ID			ID_PIOA
#define LED1_PIO_IDX		0
#define LED1_PIO_IDX_MASK	(1 << LED1_PIO_IDX)

#define LED2_PIO			PIOC
#define LED2_PIO_ID			ID_PIOC
#define LED2_PIO_IDX		30
#define LED2_PIO_IDX_MASK	(1 << LED2_PIO_IDX)

#define LED3_PIO			PIOB
#define LED3_PIO_ID			ID_PIOB
#define LED3_PIO_IDX		2
#define LED3_PIO_IDX_MASK	(1 << LED3_PIO_IDX)

// Configuracoes do buzzer 
#define BUZ_PIO			  PIOC
#define BUZ_PIO_ID		  ID_PIOC
#define BUZ_PIO_IDX		  13
#define BUZ_PIO_IDX_MASK (1 << BUZ_PIO_IDX)
	
// Configuracoes do botao
#define BUT_PIO			  PIOA
#define BUT_PIO_ID		  ID_PIOA
#define BUT_PIO_IDX		  11
#define BUT_PIO_IDX_MASK (1u << BUT_PIO_IDX)

// Configuracoes do botoes OLED
#define BUT1_PIO			PIOD
#define BUT1_PIO_ID			16
#define BUT1_PIO_IDX		28
#define BUT1_PIO_IDX_MASK	(1u << BUT1_PIO_IDX)

#define BUT2_PIO			PIOC
#define BUT2_PIO_ID			12
#define BUT2_PIO_IDX		31
#define BUT2_PIO_IDX_MASK	(1u << BUT2_PIO_IDX)

#define BUT3_PIO			PIOA
#define BUT3_PIO_ID			10
#define BUT3_PIO_IDX		19
#define BUT3_PIO_IDX_MASK	(1u << BUT3_PIO_IDX)

/************************************************************************/
/* constants                                                            */
/************************************************************************/
typedef struct 
{
	int *notes;
	int *times;
	int size;
	
} musica;

/************************************************************************/
/* variaveis globais                                                    */
/************************************************************************/
volatile char pause;
volatile char but1_flag;
volatile char but2_flag;
volatile char but3_flag;

/************************************************************************/
/* prototypes                                                           */
/************************************************************************/

void init(void);
void playMusic(musica music);
void pio_enable_interrupt(Pio *p_pio, const uint32_t ul_mask);
void LED_init(int estado);

/************************************************************************/
/* interrupcoes                                                         */
/************************************************************************/

/************************************************************************/
/* funcoes                                                              */
/************************************************************************/

void but_callBack(void){
	pause = !pause;
}
void but1_callBack(void){
	but1_flag = 1;
}
void but2_callBack(void){
	but2_flag = 1;
}
void but3_callBack(void){
	but3_flag = 1;
}
// Função de inicialização do uC e config correta dos perifericos e pinos
void init(void)
{
	//inicializa board clock
	sysclk_init();
	
	//desativa WatchDog Timer
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	//BUZZER
	pmc_enable_periph_clk(BUZ_PIO_ID); //inicializa pio do buzzer
	pio_set_output(BUZ_PIO, BUZ_PIO_IDX_MASK, 0, 0,0 );
	
	//Ativa o PIO na qual o LED foi conectado
	// para que possamos controlar o LED
	pmc_enable_periph_clk(LED_PIO_ID);
	pmc_enable_periph_clk(LED1_PIO_ID);
	pmc_enable_periph_clk(LED2_PIO_ID);
	pmc_enable_periph_clk(LED3_PIO_ID);
	
	//Inicializa o PC8 como saida
	pio_set_output(LED_PIO, LED_PIO_IDX_MASK, 0, 0, 0);
	pio_set_output(LED1_PIO, LED1_PIO_IDX_MASK, 0, 0, 0);
	pio_set_output(LED2_PIO, LED2_PIO_IDX_MASK, 0, 0, 0);
	pio_set_output(LED3_PIO, LED3_PIO_IDX_MASK, 0, 0, 0);
	
	// Inicializa PIO do botao
	pmc_enable_periph_clk(BUT_PIO_ID);
	pmc_enable_periph_clk(BUT1_PIO_ID);
	pmc_enable_periph_clk(BUT2_PIO_ID);
	pmc_enable_periph_clk(BUT3_PIO_ID);
	
	// configura pino ligado ao botão como entrada com um pull-up.
	pio_configure(BUT_PIO, PIO_INPUT, BUT_PIO_IDX_MASK, PIO_PULLUP);
//	pio_set_input(BUT_PIO, BUT_PIO_IDX_MASK, PIO_DEFAULT);
	pio_set_input(BUT1_PIO, BUT1_PIO_IDX_MASK, PIO_PULLUP); //??? PIO DEFAULT?
	pio_set_input(BUT2_PIO, BUT2_PIO_IDX_MASK, PIO_PULLUP);
	pio_set_input(BUT3_PIO_ID, BUT3_PIO_IDX_MASK, PIO_PULLUP);
	
	//?
	// Configura interrupção no pino referente ao botao e associa
	// função de callback caso uma interrupção for gerada
	// a função de callback é a: but_callback()
	pio_handler_set(BUT_PIO, BUT_PIO_ID, BUT_PIO_IDX_MASK, PIO_IT_RISE_EDGE, but_callBack);
	pio_handler_set(BUT1_PIO, BUT1_PIO_ID, BUT1_PIO_IDX_MASK, PIO_IT_FALL_EDGE, but1_callBack);
	pio_handler_set(BUT2_PIO, BUT2_PIO_ID, BUT2_PIO_IDX_MASK, PIO_IT_FALL_EDGE, but2_callBack);
	pio_handler_set(BUT3_PIO, BUT3_PIO_ID, BUT3_PIO_IDX_MASK, PIO_IT_FALL_EDGE, but3_callBack);



	
	//Ativar o pull-up
	//pio_pull_up(BUT_PIO, BUT_PIO_IDX_MASK, 1);
	pio_pull_up(BUT1_PIO, BUT1_PIO_IDX_MASK, 1);
	pio_pull_up(BUT2_PIO, BUT2_PIO_IDX_MASK, 1);
	pio_pull_up(BUT3_PIO, BUT3_PIO_IDX_MASK, 1);
	
	//?
	// Ativa interrupção
	pio_enable_interrupt(BUT_PIO, BUT_PIO_IDX_MASK);
	pio_enable_interrupt(BUT1_PIO, BUT1_PIO_IDX_MASK);
	pio_enable_interrupt(BUT2_PIO, BUT2_PIO_IDX_MASK);
	pio_enable_interrupt(BUT3_PIO, BUT3_PIO_IDX_MASK);

	
	// Configura NVIC para receber interrupcoes do PIO do botao
	// com prioridade 4 (quanto mais próximo de 0 maior)
	NVIC_EnableIRQ(BUT_PIO_ID);
	NVIC_EnableIRQ(BUT1_PIO_ID);
	NVIC_EnableIRQ(BUT2_PIO_ID);
	NVIC_EnableIRQ(BUT3_PIO_ID);

	NVIC_SetPriority(BUT_PIO_ID, 3); // Prioridade 4
	NVIC_SetPriority(BUT1_PIO_ID, 4); 
	NVIC_SetPriority(BUT2_PIO_ID, 4);
	NVIC_SetPriority(BUT3_PIO_ID, 4);
	
	
}

//Inicializa o pino do LED
void LED_init(int estado){
	pmc_enable_periph_clk(LED_PIO_ID);
	pio_set_output(LED_PIO, LED_PIO_IDX_MASK, estado, 0, 0);
};

/************************************************************************/
/* Main                                                                 */
/************************************************************************/

//so ter uma volatile char pros tres call back
//cada funcao retorna um valor diferente
//fazer case switch
//se o id da musica for diferente do volatile char (apertou musica difernte)
//dar break na musica e roda a funcao com o arg da musica
void playMusic(musica music){
	//ver se o botao flag = 1 -> pausa
	for (int i=0 ;  i< music.size ; i++){

		//se a nota for 0, toca o 0 (silencio) pelo tempo
		if (music.notes[i] == 0 ){
			delay_ms(music.times[i]);
			pio_set(LED_PIO, LED_PIO_IDX_MASK); //desliga
		}
		//nota diferente de 0
		else {
			float t_delay = 1000.0 /(int)(music.notes[i])	;
			pio_clear(LED_PIO, LED_PIO_IDX_MASK);    // Coloca 0 no pino do LED (liga)
			for (long j=0 ; j < (music.times[i]/t_delay); j++){
				if (pause){
					pio_set(LED_PIO, LED_PIO_IDX_MASK);//desliga
					j--;
				} 
				else {
					//if (music.times[i]!= 0){
						pio_set(BUZ_PIO, BUZ_PIO_IDX_MASK);
						delay_us(t_delay*1000.0);
						pio_clear(BUZ_PIO, BUZ_PIO_IDX_MASK);
						delay_us(t_delay*1000.0);
					//}
				}	
				pio_set(LED_PIO, LED_PIO_IDX_MASK);    // Coloca 0 no pino do LED (liga)
			}
		}
		
		//pio_set(LED_PIO, LED_PIO_IDX_MASK);      // Coloca 1 no pino LED (desliga)
		delay_us(75); //para dar um pause entre cada nota
	}// fim primeiro for
	
}

// Funcao principal chamada na inicalizacao do uC.
int main(void)
{
	// inicializa sistema e IOs
	init();
	but1_flag = 0;
	but2_flag = 0;
	but3_flag = 0;
	pause = 0;

	musica mario;
	mario.notes = &mario_theme_notes;
	mario.times = &mario_theme_tempo;
	mario.size = sizeof(mario_theme_tempo) / sizeof(int);

	musica imperialmarch;
	imperialmarch.notes = &imperial_march_notes;
	imperialmarch.times = &imperial_march_tempo;
	imperialmarch.size = sizeof(imperial_march_tempo) / sizeof(int);

	musica underworld;
	underworld.notes = &underworld_notes;
	underworld.times = &underworld_tempo;
	underworld.size = sizeof(underworld_tempo) / sizeof(int);

	//por padrao os leds estao ligados
	pio_set(LED_PIO, LED_PIO_IDX_MASK);//desliga
	pio_set(LED1_PIO, LED1_PIO_IDX_MASK); 
	pio_set(LED2_PIO, LED2_PIO_IDX_MASK);
	pio_set(LED3_PIO, LED3_PIO_IDX_MASK);
	
	// // Init OLED
	// gfx_mono_ssd1306_init();

	// /* Configura Leds */
	// LED_init(0);

  	// Escreve na tela um circulo e um texto
    // gfx_mono_draw_filled_circle(20, 16, 16, GFX_PIXEL_SET, GFX_WHOLE);
    // gfx_mono_draw_string("Choose 1, 2 or 3", 50,16, &sysfont);

	// super loop
	// aplicacoes embarcadas não devem sair do while(1).
	while(1){		
	if (but1_flag){
		pio_clear(LED1_PIO, LED1_PIO_IDX_MASK); //liga led
		playMusic(mario);
		pio_set(LED1_PIO, LED1_PIO_IDX_MASK); //desliga led
		but1_flag = 0;
	}
	if (but2_flag){
		pio_clear(LED2_PIO, LED2_PIO_IDX_MASK);
	 	playMusic(imperialmarch);
	 	pio_set(LED2_PIO, LED2_PIO_IDX_MASK);		
		but2_flag = 0;
	}
	if (but3_flag){
		pio_clear(LED3_PIO, LED3_PIO_IDX_MASK);
	 	playMusic(underworld);
	 	pio_set(LED3_PIO, LED3_PIO_IDX_MASK);
		but3_flag = 0;
	}
	
	//pmc_sleep(SAM_PM_SMODE_SLEEP_WFI);
// 	pio_clear(LED_PIO,LED_PIO_IDX_MASK);
// 	delay_ms(5000);
	
	

	
		
	}//fim do while
	//return 0;
}
