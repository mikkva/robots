/*
 * drive.h
 *
 * Created: 21.04.2015 18:55:08
 *  Author: Dell
 */

 // 300 kiiruse juures
#ifndef DRIVE_H_
#define DRIVE_H_
#define SPEED 200 //300
#define FWD_TIME 850 // 2150
#define CENTER_TIME 400 // 2000
#define TR_TIME 250 // 600
#define TL_TIME 250 //600
#define OTSE_SEIN 150


void drive_fwd();

void center();
void turn_time();
void look();
void decide();
void central_left_v();
void central_right_v();
void right_hv();
void left_hv();
void lisa_tupik();
int next_step();
char decision_fwd();
char decision_right();
char decision_left();
void print_fun();
char el_decision_fwd();
char el_decision_right();
char el_decision_left();
void dr_fwd();


#endif /* DRIVE_H_ */