#include <rtems.h>
#include <fcntl.h>
#include <unistd.h>
#include "tmtc_channel.h"
#include "tm_descriptor.h"
#include "tc_descriptor.h"
#include "riscv_uart.h"
#include "epd_pus_mission.h"
#include "crc.h"
#include "epd_pus_tmtc.h"
#include "serialize.h"
#include "tmtc_pool.h"
#include "ccsds_pus_format.h"

rtems_id tc_message_queue_id;

/**
 * \brief Identifier of the TM channel mutex.
 */
static rtems_id tm_channel_mutex_id;

/**
 * \brief Global variable that stores the next telemetry sequence count to be
 *        assigned.
 */
static uint16_t tm_count = 0;

static int uart_fd;

rtems_status_code init_tm_channel() {

    rtems_status_code status;

    uart_fd = open("uart0", O_RDWR);

    if (uart_fd < 0) {
        return RTEMS_IO_ERROR;
    }

    // TODO: Create the mutex
    status = rtems_semaphore_create(rtems_build_name('T', 'M', 'C', 'H'),
        						1, //Número de semáforos
        						RTEMS_BINARY_SEMAPHORE | RTEMS_PRIORITY | RTEMS_PRIORITY_CEILING, //Tipo de semáforo
        						8, //Techo de prioridad
        						&tm_channel_mutex_id);

    return status;

}

uint16_t tm_channel_get_next_tm_count() {

    uint16_t ret;

    rtems_semaphore_obtain(tm_channel_mutex_id, RTEMS_WAIT, RTEMS_NO_TIMEOUT);

    ret = tm_count;
    tm_count = (tm_count + 1) & 0x3FFF;

    rtems_semaphore_release(tm_channel_mutex_id);

    return ret;

}

rtems_status_code tm_channel_send_tm(tm_descriptor_t descriptor) {

    uint8_t frame_header[6];

    frame_header[0] = 0xBE;
    frame_header[1] = 0xBA;
    frame_header[2] = 0xBE;
    frame_header[3] = 0xEF;

    serialize_uint16(descriptor.tm_num_bytes, &frame_header[4]); //Serializamos cabecera

    rtems_semaphore_obtain(tm_channel_mutex_id, RTEMS_WAIT,
            RTEMS_NO_TIMEOUT); //Obtenemos el semáforo

    write(uart_fd, frame_header, 6); //Transmitimos cabecera
    write(uart_fd, descriptor.p_tm_bytes, descriptor.tm_num_bytes); //Obtenemos el número de bytes a transmitir y los transmitimos

    rtems_semaphore_release(tm_channel_mutex_id); //Soltamos el semáforo

    tmtc_pool_free(descriptor.p_tm_bytes); //Liberamos el espacio ocupado en el pool

    return RTEMS_SUCCESSFUL;

}

uint8_t accept_tc(tc_descriptor_t * tc_descriptor) {

    uint8_t ret = 1;

    struct ccsds_pus_tmtc_packet_header tc_packet_header;
    struct ccsds_pus_tc_df_header tc_df_header;
    uint16_t tc_packet_err_ctrl;

    ccsds_pus_tc_get_fields(tc_descriptor->p_tc_bytes,
            &tc_packet_header, &tc_df_header, &tc_packet_err_ctrl);

    // Calculate CRC
    uint16_t crc_value = cal_crc_16(tc_descriptor->p_tc_bytes,
            tc_descriptor->tc_num_bytes - 2);

    if (crc_value != tc_packet_err_ctrl) {

        // Generate TM (1,2) - Reject Incorrect CRC

        tm_descriptor_t tm_descriptor;

        tm_descriptor.p_tm_bytes = tmtc_pool_alloc();

        if(tm_descriptor.p_tm_bytes != NULL){

            uint16_t tm_count = tm_channel_get_next_tm_count();

            epd_pus_build_tm_1_2_crc_error(&tm_descriptor, tm_count,
                    tc_packet_header.packet_id,
                    tc_packet_header.packet_seq_ctrl,
                    tc_packet_err_ctrl, crc_value);

            tm_channel_send_tm(tm_descriptor);

            // We should not use the TM descriptor beyond this point!

        }

        ret = 0;

    }

    if (ret == 1) {

        // If we are here, it means that the CRC of the TC was OK

        tm_descriptor_t tm_descriptor;

        switch (tc_df_header.type) {

            case 17:

                // Check if the TC is TC[17,1]. Otherwise, send TM[1,2]
                if (tc_df_header.subtype != 1) {

                    // Generate TM[1,2] - Reject Illegal subtype

                    tm_descriptor.p_tm_bytes = tmtc_pool_alloc();

                    if(tm_descriptor.p_tm_bytes != NULL){

                        uint16_t tm_count = tm_channel_get_next_tm_count();

                        epd_pus_build_tm_1_2_illegal_subtype(&tm_descriptor, tm_count,
                                tc_packet_header.packet_id,
                                tc_packet_header.packet_seq_ctrl);

                        tm_channel_send_tm(tm_descriptor);

                        // We should not use the TM descriptor beyond this point!
                    }
                    ret = 0;
                }
                break;

            case 3:

                 // Check if the TC is TC[3,31]. Otherwise, send TM[1,2]
                if (tc_df_header.subtype != 31) {

                    // Generate TM[1,2] - Reject Illegal subtype

                    tm_descriptor.p_tm_bytes = tmtc_pool_alloc();

                    if(tm_descriptor.p_tm_bytes != NULL){

                        uint16_t tm_count = tm_channel_get_next_tm_count();

                        epd_pus_build_tm_1_2_illegal_subtype(&tm_descriptor, tm_count,
                                tc_packet_header.packet_id,
                                tc_packet_header.packet_seq_ctrl);

                        tm_channel_send_tm(tm_descriptor);

                        // We should not use the TM descriptor beyond this point!
                    }
                    ret = 0;
                }
                break;

            default:

                // Generate TM[1,2] - Reject Illegal type

                tm_descriptor.p_tm_bytes = tmtc_pool_alloc();

                if(tm_descriptor.p_tm_bytes != NULL){

                    uint16_t tm_count = tm_channel_get_next_tm_count();

                    epd_pus_build_tm_1_2_illegal_type(&tm_descriptor, tm_count,
                            tc_packet_header.packet_id,
                            tc_packet_header.packet_seq_ctrl);

                    tm_channel_send_tm(tm_descriptor);

                    // We should not use the TM descriptor beyond this point!
                }
                ret = 0;
                break;

        }
    }

    // Check if there have been any errors
    if (ret == 1) {

    	tm_descriptor_t tm_descriptor;

        // If we are here, it means that the TC was not rejected

        // TODO: Generate TM[1,1] - Accept

        tm_descriptor.p_tm_bytes = tmtc_pool_alloc();

        if(tm_descriptor.p_tm_bytes != NULL){

            uint16_t tm_count = tm_channel_get_next_tm_count();

            epd_pus_build_tm_1_1(&tm_descriptor, tm_count,
                    tc_packet_header.packet_id,
                    tc_packet_header.packet_seq_ctrl);

            tm_channel_send_tm(tm_descriptor);

            // We should not use the TM descriptor beyond this point!
        }

    }

    return ret;

}

rtems_id tc_rx_task_id;

rtems_task tc_rx_task (rtems_task_argument ignored) {

    // Send message
    tc_descriptor_t tc_descriptor;

    if(init_tm_channel() == RTEMS_IO_ERROR){
    	rtems_shutdown_executive(1);
    }

    while (1) {

        uint8_t frame_header[6];

        tc_descriptor.tc_num_bytes = 0;

        //Allocate a new block from the pool to store the telecommand
        tc_descriptor.p_tc_bytes = tmtc_pool_alloc();

        //Read the frame header (6 bytes)
        read(uart_fd,frame_header,6); //PROBLEMA

        //Deserialize the size field
        tc_descriptor.tc_num_bytes = deserialize_uint16(&frame_header[4]);

        //Read the telecommand and store it into the descriptor
        read(uart_fd,tc_descriptor.p_tc_bytes,tc_descriptor.tc_num_bytes);

        //Deliver the telecommand through the message queue
        rtems_message_queue_send(tc_message_queue_id, &tc_descriptor,
                sizeof(tc_descriptor_t));

    }

    rtems_shutdown_executive(1);

}
