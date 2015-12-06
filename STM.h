/*
 * STM.h
 *
 * Created: 11/29/2015 6:09:40 PM
 *  Author: mosiu
 */ 


#ifndef STM_H_
#define STM_H_



typedef enum {
	ADC_STATE,
	READY_STATE,
	DRINKING_STATE,
	VERIFICATION_STATE,
	DISPLAY_STATE,
} state_t;

void STM_switch_superstate(void);

state_t STM_get_state(void);

inline void STM_tick(void);

void STM_set_state(state_t state);


void led_init();

/*
To be called from main. Responsible for changing displayed time every 0.1s
*/
void STM_refresh(void);

void reed_init(void);

#endif /* STM_H_ */