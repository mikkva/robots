/*
 * drive.c
 *
 * Created: 21.04.2015 18:57:17
 *  Author: Mikk Vachtmeister
 */

# define WALL 110

#include <avr/io.h>
#include "drivers/board.h"
#include "drivers/motor.h"
#include "drivers/adc.h"
#include "drivers/com.h"
#include <util/delay.h>
#include <stdio.h>
#include "drive.h"


#define L 0
#define R 1
#define FW 2
int which_decision=L;

int error = 0;
int out = 0;
int Kp1 = 4;
int motorL = 0;
int motorR = 0;
# define N 0
# define E 1
# define S 2
# define W 3
uint8_t array[7][7];
int i=0, j=0;
int direction=0;
int count_e=0;
int tupik_i[20];
int tupik_j[20];
char buf[100];
char BUF[30];
int vahe;

void look ()
{
	rgb_set(BLUE);
	_delay_ms(500);

	if(direction==N)
	{
		if (adc_read(L_SIDE)>60)
		{
			array[i][j] |=(1<<3);
		}
		if(adc_read(R_SIDE)>60)
		{
			array[i][j] |=(1<<1);
		}
		if(adc_read(R_FWD)>60 && adc_read(L_FWD)>60)
		{
			array[i][j] |=(1<<0);
		}

	}
	else if(direction==E)
	{
		if (adc_read(L_SIDE)>60)
		{
			array[i][j] |=(1<<0);
		}
		if(adc_read(R_SIDE)>60)
		{
			array[i][j] |=(1<<2);
		}

		if(adc_read(R_FWD)>60 && adc_read(L_FWD)>60)
		{
			array[i][j] |=(1<<1);
		}
	}
	else if(direction==S)
	{
		if (adc_read(L_SIDE)>60)
		{
			array[i][j] |=(1<<1);
		}
		if(adc_read(R_SIDE)>60)
		{
			array[i][j] |=(1<<3);
		}

		if(adc_read(R_FWD)>60 && adc_read(L_FWD)>60)
		{
			array[i][j] |=(1<<2);
		}
	}
	else if(direction==W)
	{
		if (adc_read(L_SIDE)>60)
		{
			array[i][j] |=(1<<2);
		}
		if(adc_read(R_SIDE)>60)
		{
			array[i][j] |=(1<<0);
		}

		if(adc_read(R_FWD)>60 && adc_read(L_FWD)>60)
		{
			array[i][j] |=(1<<3);
		}
	}
	array[i][j] |=(1<<4);
	
	rgb_set(OFF);
	_delay_ms(200);
}

void decide ()
{
	
	if (i<3 && j<3)
	{
		
		if (direction==N) // CL
		{
			
			central_left_v();
		}
		else if (direction==E)// Left hand
		{
			left_hv();
		}
		else if(direction==S)//Right hand Rule
		{
			right_hv();
		}
		else if (direction==W)
		{
			
			central_right_v();
		}
	}
	else if (i>=4 && j<3)
	{
		if(direction==N)
		{
			left_hv();
		}
		else if (direction==E)
		{
			right_hv();
		}
		else if(direction==S)
		{
			central_right_v();
		}
		else if(direction==W)
		{
			central_left_v();
		}
		
	}
	else if(j>=4 && i<3)// j=4,5,6 i=0,1,2
	{
		if(direction==N)
		{
			central_right_v();
		}
		else if (direction==E)
		{
			central_left_v();
		}
		else if(direction==S)
		{
			left_hv();
		}
		else if(direction==W)
		{
			right_hv();
		}
	}
	else if(j>=4 && i>=4)
	{
		if(direction==N)
		{
			right_hv();
		}
		else if (direction==E)
		{
			central_right_v();
		}
		else if(direction==S)
		{
			central_left_v();
		}
		else if(direction==W)
		{
			left_hv();
		}
	}
	else if(i==3 && j<3)
	{
		if(direction==N)
		{
			left_hv();
		}
		else if (direction==E)
		{
			left_hv();
		}
		else if(direction==S)
		{
			right_hv();
		}
		else if(direction==W)
		{
			central_left_v();
		}
	}
	else if(j==3 && i>3)
	{
		if(direction==N)
		{
			right_hv();
		}
		else if (direction==E)
		{
			right_hv();
		}
		else if(direction==S)
		{
			central_left_v();
		}
		else if(direction==W)
		{
			left_hv();
		}
	}
	else if (i==3 && j>3)
	{
		if(direction==N)
		{
			right_hv();
		}
		else if (direction==E)
		{
			central_left_v();
		}
		else if(direction==S)
		{
			left_hv();
		}
		else if(direction==W)
		{
			right_hv();
		}
	}
	else if(j==3 && i<3)
	{
		if(direction==N)
		{
			central_left_v();
		}
		else if (direction==E)
		{
			left_hv();
		}
		else if(direction==S)
		{
			right_hv();
		}
		else if(direction==W)
		{
			right_hv();
		}
	}
}


void drive_fwd()// sõidab otse
{
    uint32_t aeg;
	cli();
    aeg=systick_read();
	sei();
    while(systick_read()<aeg+FWD_TIME)
    {
        if(adc_read(L_DIAGO)> 110 && adc_read(R_DIAGO)> 110)// Mõlemal pool seinad ROHELINE
        {
            rgb_set(GREEN);
            error= adc_read(L_DIAGO)-adc_read(R_DIAGO);
        }
        else if(adc_read(L_DIAGO)>1 )//vasak sein SININE
        {
            rgb_set(BLUE);
            error= adc_read(L_DIAGO)-WALL;
        }
        else if(adc_read(R_DIAGO)> 110)//parem sein PUNANE
        {
            rgb_set(RED);
            error= WALL-adc_read(R_DIAGO);
        }
        else// seina pole VALGE
        {
            error=0;
            rgb_set(WHITE);
        }

        // ÄRA NÄPI SIIT ALLAPOOLE!!!
        out= Kp1*error;
        if (out>300)
        {
            out=300;
        }
        else if (out<-300)
        {
            out=-300;
        }

        if(error>=0)
        {
            motorR = SPEED - out ;
            motorL= SPEED ;
        }
        if(error<0)
        {
            motorR = SPEED  ;
            motorL= SPEED + out;
        }
        motor_set(motorL,motorR);	
		
    }
    motor_set(0,0);
    _delay_ms(200);

    if(adc_read(L_FWD)>90 || adc_read(R_FWD)>90)// kui otseandurid näevad seina
    {
        aeg=systick_read()+CENTER_TIME;
        while(systick_read()<aeg)
        {
            rgb_set(YELLOW); // kollane led põlema ja keskele
            center();
        }
         // kollane led põlema ja keskele

        motor_set(0,0);
        _delay_ms(200);
    }
    rgb_set(OFF);
}


void turn_R_time()// turn right
{
    uint32_t aeg =systick_read() +TR_TIME;
    motor_set(SPEED,-SPEED);
    /*while(systick_read()<aeg)
    {

    }*/
	_delay_ms(TR_TIME);
    motor_set(0,0);
}

void turn_R(int speed)
{
		while(LEFTENC<330)
		{
			rgb_set(BLUE);
			motor_set(speed,-speed);
		}
}
void center ()
{
   
			int error= adc_read(L_FWD)-OTSE_SEIN;
			int error_r=adc_read(R_FWD)-OTSE_SEIN;
			int out_l= Kp1*error;
			int out_r=Kp1*error_r;
			if (out_l>300)
			{
				out_l=300;
			}

			else if (out_l<-300)
			{
				out_l=-300;
			}
			if(out_r>300)
			{
				out_r=300;
			}
			if(out_r<-300)
			{
				out_r=-300;
			}

			motorR=- out_r ;
			motorL=- out_l  ;

			motor_set(motorL,motorR);
		
	
	

}


	








