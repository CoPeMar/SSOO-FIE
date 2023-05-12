#include <rtems.h>

#include <stdio.h>

#include "manager.h"
#include "tc_descriptor.h"

#include "tmtc_channel.h"
#include "tmtc_pool.h"
#include "delay.h"
#include "ccsds_pus_format.h"
#include "housekeeping.h"
#include "epd_pus_tmtc.h"
#include "tmtc_channel.h"

rtems_id manager_task_id;

rtems_task manager_task (rtems_task_argument ignored) {

    while (1) {

        tc_descriptor_t tc_descriptor;

        size_t size;

        // Receive message
        rtems_message_queue_receive(tc_message_queue_id,
                &tc_descriptor, &size,
                RTEMS_WAIT, RTEMS_NO_TIMEOUT);

        // Let's see if we shall accept the TC...
        uint8_t tc_accepted = accept_tc(&tc_descriptor);

         if (tc_accepted == 1) {

            // Yep, we accepted the TC!

            struct ccsds_pus_tmtc_packet_header tc_packet_header;
            struct ccsds_pus_tc_df_header tc_df_header;
            uint16_t tc_packet_err_ctrl;

            ccsds_pus_tc_get_fields(tc_descriptor.p_tc_bytes,
                    &tc_packet_header, &tc_df_header, &tc_packet_err_ctrl);

            if (tc_df_header.type == 3 && tc_df_header.subtype == 31) {

                // TODO: Deliver the TC to the housekeeping task
               rtems_message_queue_send(hk_message_queue_id, &tc_descriptor, size);

            } else {
            	tm_descriptor_t tm_descriptor_1_7;
            	tm_descriptor_t tm_descriptor_17_2;

            	tm_descriptor_1_7.p_tm_bytes = tmtc_pool_alloc();
            	tm_descriptor_17_2.p_tm_bytes = tmtc_pool_alloc();

                // TODO: Generate TM[17,2]
            	uint16_t tm_count = tm_channel_get_next_tm_count();
            	epd_pus_build_tm_17_2(&tm_descriptor_17_2,tm_count); // El problema está aquí
            	tm_channel_send_tm(tm_descriptor_17_2);

                // TODO: Generate TM[1,7]
            	tm_count = tm_channel_get_next_tm_count();
            	epd_pus_build_tm_1_7(&tm_descriptor_1_7,tm_count,
            							tc_packet_header.packet_id,
										tc_packet_header.packet_seq_ctrl);
            	tm_channel_send_tm(tm_descriptor_1_7);

                tmtc_pool_free(tc_descriptor.p_tc_bytes);

            }


        } else {

             // We did not accept the TC, so nothing further has to be done
             tmtc_pool_free(tc_descriptor.p_tc_bytes);

        }
    }
}
