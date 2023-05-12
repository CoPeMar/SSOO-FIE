/*
 * riscv_uart.h
 *
 *  Created on: 22 sept 2022
 *      Author: atcsol
 */

#ifndef INCLUDE_RISCV_UART_H_
#define INCLUDE_RISCV_UART_H_

#include <basic_types.h>

// STATUS REGISTER MASKS
#define RISCV_UART_DR   0x00000001  // UART DATA READY
#define RISCV_UART_TF   0x00000200 // UART TX FIFO is full
#define RISCV_UART_TE   0x00000004 // UART TX FIFO is empty


// CONTROL REGISTER MASKS
#define RISCV_UART_RXE   0x00000001 /* RX enable */
#define RISCV_UART_TXE   0x00000002 /* TX enable */
#define RISCV_UART_LB    0x00000080 /* Loop Back enable */
#define RISCV_UART_RI    0x00000004 /* Receiver interrupt enable */

int8_t riscv_putchar(char c);

int32_t riscv_getchar();

int8_t riscv_uart_tx_fifo_is_empty();

void riscv_uart_enable_TX();
void riscv_uart_disable_TX();

void riscv_uart_enable_RX();
void riscv_uart_disable_RX();

void print_fields(uint8_t fieldata[255]);
void clear_check();


#endif /* INCLUDE_RISCV_UART_H_ */
