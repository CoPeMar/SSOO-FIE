#include <rtems.h>
#include <rtems/libio.h>
#include <bsp/irq.h>
#include <fcntl.h>
#include <unistd.h>


#include "uart_rx_queue.h"
#include "riscv_uart.h"

#define UART_RX_IRQ_NUMBER 0x3

static rtems_id uart_rx_sem;

static void riscv_uart_driver_isr (void * dummy);

rtems_device_driver riscv_uart_driver_initialize (
        rtems_device_major_number major,
        rtems_device_minor_number dummy,
        void *arg) {

    // Setup interrupt handler
    if(rtems_interrupt_handler_install(
            RISCV_INTERRUPT_VECTOR_EXTERNAL(UART_RX_IRQ_NUMBER),
            "uart0", RTEMS_INTERRUPT_UNIQUE,
            riscv_uart_driver_isr, NULL) != RTEMS_SUCCESSFUL) {
        return RTEMS_INTERNAL_ERROR;
    }

    // Register the name "uart0"
    if (rtems_io_register_name("uart0", major, 0) != RTEMS_SUCCESSFUL) {
        return RTEMS_INTERNAL_ERROR;
    }

    // TODO: Create the binary semaphore

    rtems_semaphore_create(rtems_build_name('U', 'A', 'R', 'X'),
			0,
			RTEMS_SIMPLE_BINARY_SEMAPHORE, //Tipo de semáforo
			5, //Techo de prioridad
			&uart_rx_sem);

    return RTEMS_SUCCESSFUL;
}

rtems_device_driver riscv_uart_driver_open(rtems_device_major_number major,
        rtems_device_minor_number minor, void *arg) {

    if (minor != 0) {
        return RTEMS_INVALID_NAME;
    }

    riscv_uart_enable_RX();
    riscv_uart_enable_TX();
    riscv_uart_enable_RI();

    return RTEMS_SUCCESSFUL;
}

rtems_device_driver riscv_uart_driver_read (
        rtems_device_major_number major,
        rtems_device_minor_number minor,
        void * arg)
{

    rtems_libio_rw_args_t * rw_args = (rtems_libio_rw_args_t *) arg;
    rtems_interrupt_level oldLevel;
    uint32_t count = 0;

    uint8_t * buf = (uint8_t *)rw_args->buffer;

    // Protect access to the software FIFO
    rtems_interrupt_disable(oldLevel);

    do {

        /* Read from SW fifo */
        if (uart_rx_queue_extract_char(&buf[count]) != 0) {

            // Enable interrupts before blocking
            rtems_interrupt_enable(oldLevel);

            // TODO: Block the task until a chararacter is received
            rtems_semaphore_obtain(uart_rx_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);

            // Protect access to the software FIFO
            rtems_interrupt_disable(oldLevel);

            continue;

        }

        // Got a character from the queue!
        count = count + 1;

    } while (count < rw_args->count);

    rtems_interrupt_enable(oldLevel);

    rw_args->bytes_moved = count;

    return RTEMS_SUCCESSFUL;
}

static void riscv_uart_driver_isr (void * dummy) {

    int32_t value;

    // Empty RX FIFO into software FIF0
    while ((value = riscv_getchar()) != -1) {

        if (uart_rx_queue_is_full()) {
            break;
        }
        /* put into fifo */
        uart_rx_queue_insert_char((uint8_t) value);
    }

    // TODO: Wake up any waiting task
    rtems_semaphore_release(uart_rx_sem);

}

rtems_device_driver riscv_uart_driver_write (
        rtems_device_major_number major,
        rtems_device_minor_number minor,
        void *arg) {

    rtems_libio_rw_args_t *rw_args = (rtems_libio_rw_args_t *) arg;

    for (uint32_t i = 0; i < rw_args->count; i++) {

        if (riscv_putchar(rw_args->buffer[i]) != 0) {
            return RTEMS_IO_ERROR;
        }
    }

    while(!riscv_uart_tx_fifo_is_empty());

    rw_args->bytes_moved = rw_args->count;

    return RTEMS_SUCCESSFUL;
}

rtems_device_driver riscv_uart_driver_close (
        rtems_device_major_number major,
        rtems_device_minor_number minor, void *arg) {

    riscv_uart_disable_RI();
    riscv_uart_disable_RX();
    riscv_uart_disable_TX();

    return RTEMS_SUCCESSFUL;
}
