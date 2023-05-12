#include <rtems.h>

#include <stdio.h>
#include <stdlib.h>

#include "housekeeping.h"
#include "tm_descriptor.h"
#include "tmtc_channel.h"
#include "epd_pus_tmtc.h"
#include "tmtc_pool.h"

#define N_HK_DATA 32
#define HK_INITIAL_INTERVAL 10

static uint8_t hk_parameters[N_HK_DATA];

uint32_t interval = HK_INITIAL_INTERVAL;
uint32_t interval_control = 0;

// This variable will store the random seed
static unsigned int seed;
rtems_id hk_message_queue_id;

void do_housekeeping(void) {

    hk_parameters[interval_control] = rand_r(&seed); //Guardamos valor aleatorio
    interval_control++; //Incrementamos el valor del intervalo
    if(interval_control == interval){ //Si ya se han leído todos, construye la telemetría
    	tm_descriptor_t tm_descriptor_3_25;
    	tm_descriptor_3_25.p_tm_bytes = tmtc_pool_alloc();
    	uint16_t tm_count = tm_channel_get_next_tm_count();
    	epd_pus_build_tm_3_25_sid0(&tm_descriptor_3_25, tm_count,
    								&hk_parameters,interval);
    	tm_channel_send_tm(tm_descriptor_3_25);
    	interval_control = 0; //Reinicia interval_control
    }

}

void init_housekeeping(void) {

    // Initialize random seed with a default value
    seed = 0;

}
