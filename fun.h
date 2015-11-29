#ifndef FUN_H_
#define FUN_H_



/*
Ustawia TIMER2 pod katem "Timera systemowego" o czestotliwosci 122,5 Hz
*/
void init_timer (void);
uint16_t* get_current_time(void);

void start_time_measurement(void);
void stop_time_measurement(void);


#endif // FUN_H_