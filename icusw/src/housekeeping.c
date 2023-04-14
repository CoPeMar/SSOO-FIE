#include <rtems.h>

#include <stdio.h>
#include <stdlib.h>

#include "housekeeping.h"

#define N_HK_DATA 16
#define HK_INITIAL_INTERVAL 10

static uint8_t hk_parameters[N_HK_DATA];

static uint32_t interval = HK_INITIAL_INTERVAL;
static uint32_t interval_control = 0;

// This variable will store the random seed
static unsigned int seed;

void do_housekeeping(void) {

    hk_parameters[interval_control] = rand_r(&seed); //Guardamos valor aleatorio
    interval_control++; //Incrementamos el valor del intervalo
    if(interval_control == interval){ //Si ya se han leído todos, imprime los valores
    	for(uint8_t i = 0; i < HK_INITIAL_INTERVAL; i++){
    		printf("Parameter %d - Value: %d\n", i+1, hk_parameters[i]);
    	}
    	interval_control = 0; //Reinicia interval_control
    }

}

void init_housekeeping(void) {

    // Initialize random seed with a default value
    seed = 0;

}
