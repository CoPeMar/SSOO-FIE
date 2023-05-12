#ifndef __HOUSEKEEPING__H__
#define __HOUSEKEEPING__H__

/**
 * \brief Initialize housekeeping parameters
 */
void init_housekeeping(void);

/**
 * \brief Perform housekeeping and telemetry generation
 */
void do_housekeeping(void);

/**
 * \brief Identifier of the housekeeping task's message queue.
 */
extern rtems_id hk_message_queue_id;
extern uint32_t interval;
extern uint32_t interval_control;

#endif // __HOUSEKEEPING__H__
