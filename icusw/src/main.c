#include <rtems.h>

// Include the configuration header file
#include "rtems_config.h"

// Luckily, we have libc! :)
#include <stdio.h>

#include "housekeeping.h"

#include "delay.h"
#include "ccsds_pus_format.h"
#include "tmtc_pool.h"
#include "serialize.h"
#include "tc_descriptor.h"
#include "tmtc_channel.h"
#include "emu_tc_rx.h"
#include "manager.h"

rtems_id housekeeping_task_id;

rtems_task housekeeping_task(rtems_task_argument ignored){

	rtems_interval current_time = 0;
	init_housekeeping();
	rtems_interval next_time = rtems_clock_get_ticks_per_second();

	while(1){

		current_time = rtems_clock_get_ticks_since_boot();

		printf("Current time: %d\n",current_time);

		task_delay_until(next_time);

		do_housekeeping();

		next_time = next_time + rtems_clock_get_ticks_per_second();
	}

}

rtems_task Init(rtems_task_argument ignored)
{
	rtems_task_create(
			rtems_build_name('H', 'k', 'T', 'k'),
			10,
			RTEMS_MINIMUM_STACK_SIZE,
			RTEMS_DEFAULT_MODES,
			RTEMS_DEFAULT_ATTRIBUTES,
			&housekeeping_task_id);
	rtems_task_create(
			rtems_build_name('T', 'C', 'R', 'X'),
			5,
			RTEMS_MINIMUM_STACK_SIZE,
			RTEMS_DEFAULT_MODES,
			RTEMS_DEFAULT_ATTRIBUTES,
			&emu_tc_rx_task_id);
	rtems_task_create(
			rtems_build_name('M','N','G','T'),
			8,
			RTEMS_MINIMUM_STACK_SIZE,
			RTEMS_DEFAULT_MODES,
			RTEMS_DEFAULT_ATTRIBUTES,
			&manager_task_id);
	rtems_task_start(housekeeping_task_id,housekeeping_task,ignored);
	rtems_task_start(manager_task_id,manager_task,ignored);
	rtems_task_start(emu_tc_rx_task_id,emu_tc_rx_task,ignored);
    printf("Hello World\n");
    rtems_message_queue_create(
    		rtems_build_name('T', 'C', 'M', 'Q'),
    		5,
			sizeof(tc_descriptor_t),
			RTEMS_FIFO,
			&tc_message_queue_id); // Creamos la cola de mensajes
    init_tmtc_pool();
    printf("Hello World\n");
    rtems_task_delete(RTEMS_SELF);
    rtems_shutdown_executive(0);
}
