#ifndef INCLUDE_MANAGER_H_
#define INCLUDE_MANAGER_H_

#include <rtems.h>

/**
 * \brief Identifier of the manager task.
 */
extern rtems_id manager_task_id;

/**
 * \brief Entry point of the manager task.
 */
rtems_task manager_task (rtems_task_argument ignored);

#endif /* INCLUDE_MANAGER_H_ */
