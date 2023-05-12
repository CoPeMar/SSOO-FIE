#ifndef __RTEMS_CONFIG_H__
#define __RTEMS_CONFIG_H__

#include <rtems.h>
#include "tc_descriptor.h"

rtems_task Init(rtems_task_argument arg);

// We require the Console Driver
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

// For the time being, we will not require the clock driver (Now we do)
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

// Default value of microseconds per tick
#define CONFIGURE_MICROSECONDS_PER_TICK (10000)

// Maximum number of tasks
#define CONFIGURE_MAXIMUM_TASKS (4)

// Maximum number of semaphores
#define CONFIGURE_MAXIMUM_SEMAPHORES (2)

#define CONFIGURE_MAXIMUM_MESSAGE_QUEUES (2)

#define CONFIGURE_MESSAGE_BUFFER_MEMORY CONFIGURE_MESSAGE_BUFFERS_FOR_QUEUE(20, sizeof(tc_descriptor_t))

// Ensure that the default initialization table is defined
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_INIT

#include <rtems/confdefs.h>

#endif /* __RTEMS_CONFIG_H__ */
