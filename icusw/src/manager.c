#include <rtems.h>

#include <stdio.h>

#include "manager.h"
#include "tc_descriptor.h"

#include "tmtc_channel.h"
#include "tmtc_pool.h"
#include "delay.h"
#include "ccsds_pus_format.h"


rtems_id manager_task_id;

rtems_task manager_task (rtems_task_argument ignored) {

    while (1) {

        tc_descriptor_t tc_descriptor;
        size_t size;

        printf("Test");

        // TODO: Receive message from the queue
        rtems_message_queue_receive(
        		tc_message_queue_id,
        		&tc_descriptor,
				&size,
				RTEMS_WAIT,
				RTEMS_NO_TIMEOUT);

        struct ccsds_pus_tmtc_packet_header tc_packet_header;
        struct ccsds_pus_tc_df_header tc_df_header;
        uint16_t tc_packet_err_ctrl;

        ccsds_pus_tc_get_fields(tc_descriptor.p_tc_bytes,
                &tc_packet_header, &tc_df_header, &tc_packet_err_ctrl);

        printf("Received telecommand (%d,%d)\n", tc_df_header.type,
                tc_df_header.subtype);

        printf("Test2");

        // TODO: Free the memory block of the descriptor
        tmtc_pool_free(tc_descriptor.p_tc_bytes);

    }

}
