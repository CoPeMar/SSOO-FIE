#ifndef INCLUDE_DRIVER_H_
#define INCLUDE_DRIVER_H_

#include <rtems.h>


/**
 * \brief Initialize primitive of the UART driver.
 *
 * @param major the major number corresponding to the device driver.
 * @param dummy Ignored.
 * @param arg   pointer to a rtems_libio_rw_args_t structure that will store
 *              the input and output parameters of the operation.
 * @return      RTEMS_SUCCESSFUL if the initialization was completed
 *              successfully or RTEMS_INTERNAL_ERROR otherwise.
 */
rtems_device_driver riscv_uart_driver_initialize (
        rtems_device_major_number major,
        rtems_device_minor_number dummy,
        void *arg);

/**
 * \brief Open primitive of the UART driver.
 *
 * @param major the major number corresponding to the device driver.
 * @param minor the minor number corresponding to the device. It shall always
 *              be zero.
 * @param arg   Ignored.
 * @return      RTEMS_SUCCESSFUL if the minor number is 0, RTEMS_INVALID_NAME
 *              otherwise.
 */
rtems_device_driver riscv_uart_driver_open(
        rtems_device_major_number major,
        rtems_device_minor_number minor,
        void *arg);

/**
 * \brief Read primitive of the UART driver.
 *
 * @param major the major number corresponding to the device driver.
 * @param minor the minor number corresponding to the device. It shall always
 *              be zero.
 * @param arg   pointer to a rtems_libio_rw_args_t structure that will store
 *              the input and output parameters of the operation.
 * @return      RTEMS_SUCCESSFUL in all cases.
 */
rtems_device_driver riscv_uart_driver_read (
        rtems_device_major_number major,
        rtems_device_minor_number minor,
        void * arg);

/**
 * \brief Write primitive of the UART driver.
 *
 * @param major the major number corresponding to the device driver.
 * @param minor the minor number corresponding to the device. It shall always
 *              be zero.
 * @param arg   pointer to a rtems_libio_rw_args_t structure that will store
 *              the input and output parameters of the operation.
 * @return      RTEMS_SUCCESSFUL in all cases.
 */
rtems_device_driver riscv_uart_driver_write (
        rtems_device_major_number major,
        rtems_device_minor_number minor,
        void *arg);

/**
 * \brief Close primitive of the UART driver.
 *
 * @param major the major number corresponding to the device driver.
 * @param minor the minor number corresponding to the device. It shall always
 *              be zero.
 * @param arg   Ignored.
 * @return      RTEMS_SUCCESSFUL in all cases.
 */
rtems_device_driver riscv_uart_driver_close (
        rtems_device_major_number major,
        rtems_device_minor_number minor, void *arg);


#endif /* INCLUDE_DRIVER_H_ */
