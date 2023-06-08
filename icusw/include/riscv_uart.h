#ifndef RISCV_UART_H_
#define RISCV_UART_H_

#include <basic_types.h>

void debug_putchar(char);

/** \brief put a char in the UART FIFO buffer for sending it.
 *
 *	\param c char to send
 *
 *	\return 0 if there is no error, 1 if char cannot be inserted in the FIFO buffer
 */

  int8_t riscv_putchar(char c);

/** \brief check if UART Tx FIFO is empty
 *
 *	\param c char to send
 *
 *	\return 1 if UART Tx FIFO is empty
 */
int8_t riscv_uart_tx_fifo_is_empty();

int32_t riscv_getchar();

void riscv_uart_enable_TX();

void riscv_uart_enable_RX();

void riscv_uart_enable_LB();

void riscv_uart_enable_RI();

void riscv_uart_disable_LB();

void riscv_uart_disable_RI();

void riscv_uart_disable_RX();

void riscv_uart_disable_TX();



#define UART_IRQ 3

#endif /* riscv_UART_H_ */
