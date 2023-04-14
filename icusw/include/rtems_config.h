#ifndef __RTEMS_CONFIG_H__
#define __RTEMS_CONFIG_H__

#include <rtems.h>

rtems_task Init(rtems_task_argument arg);

// We require the Console Driver
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

// For the time being, we will not require the clock driver (Now we do)
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

// Default value of microseconds per tick
#define CONFIGURE_MICROSECONDS_PER_TICK (10000)

// Maximum number of tasks
#define CONFIGURE_MAXIMUM_TASKS (2)

// Ensure that the default initialization table is defined
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_INIT

#include <rtems/confdefs.h>

#endif /* __RTEMS_CONFIG_H__ */
