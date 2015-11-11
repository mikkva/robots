/*
* main.c
*
* Created: 18.12.2014 17:50:18
*  Author: Mikk Vachtmeister
*/

/*
look();
save();
decide(what_i_see);
move(); // F, R, L
*/
# define WALL 40

#include <avr/io.h>
#include "drivers/board.h"
#include "drivers/motor.h"
#include "drivers/adc.h"
#include "drivers/com.h"
#include <util/delay.h>
#include <stdio.h>
#include "drive.h"

void motor_forward(int ticks) {
	LEFTENC = 0;
	RIGHTENC = 0;
	int current_l_speed = 0;
	int current_r_speed = 0;
	while(LEFTENC<ticks && RIGHTENC < ticks) {
		 if (current_l_speed< SPEED) {current_l_speed += 2;}
		 if (current_r_speed< SPEED) {current_r_speed += 2;}
		motor_set(current_l_speed, current_r_speed);
		_delay_ms(1);
	}
}

int main(void)
{
	quadrature_init();
	clock_init();		// Seadista süsteemi kell 32MHz peale
	adc_init();
	motor_init();
	radio_init(57600);
	board_init();
	char BUF[30];
	int i;
	
	for(i=0; i<3; i++)
	{
		_delay_ms(500);
		rgb_set(BLUE);
		_delay_ms(500);
		rgb_set(OFF);
	}
	while(!sw1_read());
	_delay_ms(500);
	rgb_set(RED);
	
	_delay_ms(1000);
	



	while(1)
	{
		/*
		sprintf(BUF, "%d %d %d %d %d %d\n",adc_read(L_FWD), adc_read(L_DIAGO), adc_read(L_SIDE), adc_read(R_SIDE), adc_read(R_DIAGO), adc_read(R_FWD) );
		*/
		
		sprintf(BUF, "%d %d\n", LEFTENC, RIGHTENC);
		
		_delay_ms(500);
		radio_puts(BUF);
		rgb_set(PINK);
		motor_forward(500, 300);

		rgb_set(OFF);
		
		motor_set(0,0);
		
		sprintf(BUF, "%d %d\n", LEFTENC, RIGHTENC);
		
		_delay_ms(500);
		radio_puts(BUF);
		
		_delay_ms(2000);

	}



}



