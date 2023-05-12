#ifndef INCLUDE_TMTC_CHANNEL_H_
#define INCLUDE_TMTC_CHANNEL_H_

#include <rtems.h>
#include "tm_descriptor.h"

/**
 * \brief Identifier of the TC message queue.
 */
extern rtems_id tc_message_queue_id;

/**
 * \brief Initializes the TM channel.
 *
 * \return RTEMS_SUCCESS if the channel was initialized correctly or an error
 *         otherwise.
 */
rtems_status_code init_tm_channel();

/**
 * \brief Obtains the next value of the sequence counter.
 *
 * \return the next value of the TM counter.
 */
uint16_t tm_channel_get_next_tm_count();

/**
 * \brief Sends a telemetry through the channel.
 *
 * \param descriptor the descriptor with the telemetry packet to send.
 *
 * \return RTEMS_SUCCESS if the packet was transmitted successfully or
 *         RTEMS_IO_ERROR otherwise.
 */
rtems_status_code tm_channel_send_tm(tm_descriptor_t descriptor);

#endif /* INCLUDE_TMTC_CHANNEL_H_ */
