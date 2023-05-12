/*
 * riscv_uart.c
 *
 *  Created on: 22 sept 2022
 *      Author: atcsol
 */

#include "riscv_uart.h"
#include "basic_types.h"

uint8_t *check;

struct UART_regs
{
	/** \brief UART  Data Register */
	volatile uint32_t Data;   	/* 0xFC001000 */
	/** \brief UART  Status Register */
	volatile uint32_t Status; 	/* 0xFC001004 */
	/** \brief UART  Control Register */
	volatile uint32_t Ctrl; 	/* 0xFC001008 */
	/** \brief UART  Scaler Register */
	volatile uint32_t Scaler; 	/* 0xFC00100C */
};

volatile struct UART_regs * const pUART_REGS= (struct UART_regs*) 0xFC001000;

//***************************
#define riscv_UART_TF_IS_FULL() ( RISCV_UART_TF & pUART_REGS -> Status )

//***************************
int8_t riscv_putchar(char c)
{
	uint32_t write_timeout=0;

	while( (riscv_UART_TF_IS_FULL()) && (write_timeout < 0xAAAAA))
	{
		write_timeout++;
	}

	if(write_timeout < 0xAAAAA)

		pUART_REGS -> Data = c;

	// return !0 (true) if write_timeout reach maximun value
	// 0 (false) otherwise
	return (write_timeout >= 0xAAAAA);
}

//***************************
int32_t riscv_getchar()
{
	int32_t uart_data = -1;

	if((pUART_REGS->Status & RISCV_UART_DR)==RISCV_UART_DR){
		if(pUART_REGS->Data == 0x1B){
			*check = 0x1B;
		}
		if(*check == 0x1B){
			uart_data = pUART_REGS -> Data;
		}
	}
	return uart_data;
}
//***************************
int8_t riscv_uart_tx_fifo_is_empty()
{
	// Return 0 if TE Status bit is not set, !0 otherwise
	return ( RISCV_UART_TE & pUART_REGS -> Status );
}

//***************************
void riscv_uart_enable_TX()
{
    // Set TE bit in Control register
    pUART_REGS -> Ctrl = pUART_REGS -> Ctrl | RISCV_UART_TXE; //Same as pUART_REGS->Ctrl |= RISCV_UART_TXE;
}

//***************************
void riscv_uart_disable_TX()
{
    // Clear TE bit in Control register
	pUART_REGS->Ctrl &= ~RISCV_UART_TXE;
}

//***************************
void riscv_uart_enable_RX()
{
	//Set RE bit in Control register
	pUART_REGS->Ctrl |= RISCV_UART_RXE;
}
//***************************
void riscv_uart_disable_RX()
{
	//Clear RE bit in Control register
	pUART_REGS->Ctrl &= ~RISCV_UART_RXE;
}

//***************************
/*void print_fields(uint8_t fieldata[255]){
	uint16_t pID = (fieldata[0] << 8)|fieldata[1];
	uint16_t pS = (fieldata[2] << 8)|fieldata[3];
	uint16_t pLEN = (fieldata[4] << 8)|fieldata[5];
	CONSOLE_LOG("TYPE: %d\n",(pID & 0x1000));
	CONSOLE_LOG("PKT SEC HDR FLAG: %d\n",(pID & 0x0800));
	CONSOLE_LOG("APID: %d\n",(pID & 0x07FF));
	CONSOLE_LOG("SEQUENCE FLAGS: %d\n",(pS & 0xC000));
	CONSOLE_LOG("PACKET SEQUENCE: %d\n",(pS & 0x3FFF));
	CONSOLE_LOG("PKT DATA LEN: %d\n",pLEN);
}*/

void clear_check(){
	*check = 0x00;
}
