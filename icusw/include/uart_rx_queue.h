#ifndef INCLUDE__UART_RX_QUEUE_H_
#define INCLUDE__UART_RX_QUEUE_H_

#include "basic_types.h"

/**
 * \brief Initializes THE RX queue. This function has to be called from the
 *        driver's initialization function.
 */
void init_uart_rx_queue();

/**
 * \brief Checks whether THE RX queue is full or not.
 *
 * @return returns 1 if the FIFO is full, 0 if not.
 */
uint8_t uart_rx_queue_is_full();

/**
 * \brief Checks whether THE RX F is empty or not.
 *
 * @return returns 1 if the queue is empty, 0 if not.
 */
uint8_t uart_rx_queue_is_empty();

/**
 * \ brief Extracts the first character from the queue.
 *
 * @param output reference to the structure into which the character
 *               will be stored.
 * @return returns 1 if there was an error during extraction (if the queue was
 *         empty), 0 if the extraction was successful.
 */
uint8_t uart_rx_queue_extract_char(uint8_t * output);

/**
 * \ brief Inserts a character descriptor at the end of the queue.
 *
 * @param input character to be stored.
 *
 * @return returns 1 if there was an error during insertion (if the queue was
 *         full), 0 if the insertion was successful.
 */
uint8_t uart_rx_queue_insert_char(uint8_t input);

#endif /* INCLUDE__UART_RX_QUEUE_H_ */
