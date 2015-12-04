#ifndef FUN_H_
#define FUN_H_



/*
Sets TIMER2 to function as a time measurement unit
*/
void init_timer (void);
uint16_t* get_current_time(void);

/*

*/
void start_time_measurement(void);
void stop_time_measurement(void);


#endif // FUN_H_