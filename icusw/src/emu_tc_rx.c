#include <rtems.h>
#include <string.h>
#include <stdio.h>
#include "tmtc_channel.h"
#include "tc_descriptor.h"
#include "tmtc_pool.h"
#include "emu_tc_rx.h"

rtems_id emu_tc_rx_task_id;

/**
 * \brief Test TC (17,1) to be sent to the manager task
 */
const static uint8_t test_tc_17_1[] = {
        0x1B, 0x2C, 0xC0, 0x00, 0x00, 0x05,
        0x19, 0x11, 0x01, 0x78, 0xE4, 0xEC
};

rtems_task emu_tc_rx_task (rtems_task_argument ignored) {

    tc_descriptor_t tc_descriptor;

    tc_descriptor.tc_num_bytes = 0;

    tc_descriptor.p_tc_bytes = tmtc_pool_alloc();

    // Copy the test TC into the memory block
    memcpy(tc_descriptor.p_tc_bytes, test_tc_17_1, 12);
    tc_descriptor.tc_num_bytes = 12;

    rtems_message_queue_send(tc_message_queue_id, &tc_descriptor, sizeof(tc_descriptor_t)); //Enviamos mensaje

    // The descriptor has been moved! We cannot use it beyond this point!

    rtems_task_delete(RTEMS_SELF);


}
