#include <rtems.h>

#include "delay.h"

rtems_status_code task_delay_until(rtems_interval ticks_since_boot) {

    rtems_interval current_time;
    rtems_status_code status = 0;

    current_time = rtems_clock_get_ticks_since_boot();

    if (ticks_since_boot > current_time){
        status = rtems_task_wake_after(ticks_since_boot - current_time);
    }

    return status;

}
