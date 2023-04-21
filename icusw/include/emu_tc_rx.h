#ifndef INCLUDE_EMU_TC_RX_H_
#define INCLUDE_EMU_TC_RX_H_

#include <rtems.h>

/**
 * \brief Identifier of the TC reception task.
 */
extern rtems_id emu_tc_rx_task_id;

/**
 * \brief Entry point of the TC reception task.
 */
rtems_task emu_tc_rx_task (rtems_task_argument ignored);

#endif /* INCLUDE_EMU_TC_RX_H_ */
