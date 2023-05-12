#include <rtems.h>
#include <string.h>
#include <stdio.h>
#include "tmtc_channel.h"
#include "tc_descriptor.h"
#include "tmtc_pool.h"
#include "emu_tc_rx.h"

rtems_id emu_tc_rx_task_id;

/**
 * \brief Test TC (17,1) and sequence number 0x0000.
 */
const static uint8_t test_tc_17_1[] = {
        0x1B, 0x2C, 0xC0, 0x00, 0x00, 0x05,
        0x19, 0x11, 0x01, 0x78, 0xE4, 0xEC
};

/**
 * \brief Test TC[20,1] with erroneous CRC and sequence number 0x0001.
 */
const static uint8_t test_tc_with_crc_error[] = {
        0x1B, 0x2C, 0xC0, 0x01, 0x00, 0x09,
        0x19, 0x14, 0x01, 0x78, 0x00, 0x01,
        0x00, 0x00, 0x6E, 0xCF
};

/**
 * \brief Test TC[20,3] with valid CRC and sequence number 0x0002.
 */
const static uint8_t test_tc_illegal_type[] = {
        0x1B, 0x2C, 0xC0, 0x02, 0x00, 0x11,
        0x19, 0x14, 0x03, 0x78, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0xAA, 0x7E, 0xAA
};

/**
 * \brief Test TC[3,6] with valid CRC and sequence number 0x0003.
 */
const static uint8_t test_tc_illegal_subtype[] = {
        0x1B, 0x2C, 0xC0, 0x03, 0x00, 0x05,
        0x19, 0x03, 0x06, 0x78, 0x88, 0xFA
};

/**
 * \brief Test TC[3,31] with valid CRC that sets the HK report interval
 *        for SID 0 to 20 seconds.
 */
const static uint8_t test_tc_3_31[] = {
        0x1B, 0x2C, 0xC0, 0x04, 0x00, 0x07,
        0x19, 0x03, 0x1F, 0x78, 0x00, 0x14,
        0xB7, 0x49
};

/**
 * \brief Test TC[3,31] with valid CRC that tries to set the HK report
 *        interval of invalid SID 2.
 */
const static uint8_t test_tc_3_31_invalid_sid[] = {
        0x1B, 0x2C, 0xC0, 0x05, 0x00, 0x07,
        0x19, 0x03, 0x1F, 0x78, 0x02, 0x20,
        0x4C, 0xDF
};


rtems_task emu_tc_rx_task (rtems_task_argument ignored) {

    // Send TC (17,1) and sequence number 0x0000

    tc_descriptor_t tc_descriptor;

    tc_descriptor.tc_num_bytes = 0;

    tc_descriptor.p_tc_bytes = tmtc_pool_alloc();

    memcpy(tc_descriptor.p_tc_bytes, test_tc_17_1, 12);
    tc_descriptor.tc_num_bytes = 12;

    rtems_message_queue_send(tc_message_queue_id, &tc_descriptor,
            sizeof(tc_descriptor_t));

    // Send TC[20,1] with erroneous CRC and sequence number 0x0001

    tc_descriptor.p_tc_bytes = tmtc_pool_alloc();

    memcpy(tc_descriptor.p_tc_bytes, test_tc_with_crc_error, 16);
    tc_descriptor.tc_num_bytes = 16;

    rtems_message_queue_send(tc_message_queue_id, &tc_descriptor,
            sizeof(tc_descriptor_t));

    // Send TC[20,3] with valid CRC and sequence number 0x0002

    tc_descriptor.p_tc_bytes = tmtc_pool_alloc();

    memcpy(tc_descriptor.p_tc_bytes, test_tc_illegal_type, 24);
    tc_descriptor.tc_num_bytes = 24;

    rtems_message_queue_send(tc_message_queue_id, &tc_descriptor,
            sizeof(tc_descriptor_t));

    // Send TC[3,6] with valid CRC and sequence number 0x0003

    tc_descriptor.p_tc_bytes = tmtc_pool_alloc();

    memcpy(tc_descriptor.p_tc_bytes, test_tc_illegal_subtype, 12);
    tc_descriptor.tc_num_bytes = 12;

    rtems_message_queue_send(tc_message_queue_id, &tc_descriptor,
            sizeof(tc_descriptor_t));

    // Send TC[3,31] with valid CRC that sets the HK report interval for SID 0
    // to 20 seconds

    tc_descriptor.p_tc_bytes = tmtc_pool_alloc();

    memcpy(tc_descriptor.p_tc_bytes, test_tc_3_31, 14);
    tc_descriptor.tc_num_bytes = 14;

    rtems_message_queue_send(tc_message_queue_id, &tc_descriptor,
            sizeof(tc_descriptor_t));

    // Send TC[3,31] with valid CRC that tries to set the HK report interval
    // of invalid SID 2

    tc_descriptor.p_tc_bytes = tmtc_pool_alloc();

    memcpy(tc_descriptor.p_tc_bytes, test_tc_3_31_invalid_sid, 14);
    tc_descriptor.tc_num_bytes = 14;

    rtems_message_queue_send(tc_message_queue_id, &tc_descriptor,
            sizeof(tc_descriptor_t));

    rtems_task_delete(RTEMS_SELF);

    // We should not be reaching this point
    rtems_shutdown_executive(1);

}
