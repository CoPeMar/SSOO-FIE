#include <rtems.h>

// Include the configuration header file
#include "rtems_config.h"

// Luckily, we have libc! :)
#include <stdio.h>

#include "housekeeping.h"
#include "tm_descriptor.h"
#include "delay.h"
#include "ccsds_pus_format.h"
#include "tmtc_pool.h"
#include "serialize.h"
#include "tc_descriptor.h"
#include "tmtc_channel.h"
#include "emu_tc_rx.h"
#include "manager.h"
#include "epd_pus_tmtc.h"

rtems_id housekeeping_task_id;

rtems_task housekeeping_task (rtems_task_argument ignored) {

    rtems_interval next_time = 0;

    init_housekeeping();

    rtems_interval period = rtems_clock_get_ticks_per_second();

    while (1) {

        // Check if we have any TC:
        tc_descriptor_t tc_descriptor;
        rtems_status_code status;

        size_t size;

        // Let's check if there is a telecommand
        if (rtems_message_queue_receive(hk_message_queue_id,
                    &tc_descriptor, &size,
                    RTEMS_NO_WAIT, 0) == RTEMS_SUCCESSFUL) {

            // If we are here, it means that we have received a TC

            // Decode the telecommand
            struct ccsds_pus_tmtc_packet_header tc_packet_header;
            struct ccsds_pus_tc_df_header tc_df_header;
            uint16_t tc_packet_err_ctrl;

            ccsds_pus_tc_get_fields(tc_descriptor.p_tc_bytes,
                    &tc_packet_header, &tc_df_header, &tc_packet_err_ctrl);

            // NOTE: we are assuming that the packet is well formed

            // Get the SID
            uint8_t sid = tc_descriptor.p_tc_bytes[10];

            // Check SID
            if (sid == 0) {

                // TODO: Deserialize the new housekeeping interval
            	uint16_t new_interval = (tc_descriptor.p_tc_bytes[12] << 8) | tc_descriptor.p_tc_bytes[11];
                // TODO: Update interval with the new housekeeping interval
            	interval = new_interval;
                // TODO: Clear current interval control
            	interval_control = 0;
                // TODO: Generate TM (1,7)
            	tm_descriptor_t tm_descriptor_1_7;
            	tm_descriptor_1_7.p_tm_bytes = tmtc_pool_alloc();
            	uint16_t tm_count = tm_channel_get_next_tm_count();
            	epd_pus_build_tm_1_7(&tm_descriptor_1_7,tm_count,
            	            			tc_packet_header.packet_id,
            							tc_packet_header.packet_seq_ctrl);
            	tm_channel_send_tm(tm_descriptor_1_7);

            } else {

                // Wrong SID

                // TODO: Generate TM (1,8)
            	tm_descriptor_t tm_descriptor_1_8;
            	tm_descriptor_1_8.p_tm_bytes = tmtc_pool_alloc();
            	uint16_t tm_count = tm_channel_get_next_tm_count();
            	epd_pus_build_tm_1_8(&tm_descriptor_1_8,tm_count,
            							tc_packet_header.packet_id,
										tc_packet_header.packet_seq_ctrl);
            	tm_channel_send_tm(tm_descriptor_1_8);

            }

            tmtc_pool_free(tc_descriptor.p_tc_bytes);

        }

        do_housekeeping();

        next_time = next_time + period;

        task_delay_until(next_time);

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
    rtems_message_queue_create(
    		rtems_build_name('T', 'C', 'M', 'Q'),
    		10,
			sizeof(tc_descriptor_t),
			RTEMS_FIFO,
			&tc_message_queue_id); // Creamos la cola de mensajes de telecomandos
    rtems_message_queue_create(
    		rtems_build_name('H', 'K', 'M', 'Q'),
			10,
			sizeof(tc_descriptor_t),
			RTEMS_FIFO,
			&hk_message_queue_id);
    init_tmtc_pool();
    init_tm_channel();
    rtems_task_delete(RTEMS_SELF);
    rtems_shutdown_executive(0);
}
