#include "basic_types.h"

#include "uart_rx_queue.h"

// Maximum number of enqueued elements
#define UART_RX_QUEUE_MAX_NOE 64

/**
 * \brief Type of the main structure that will be used to manage THE RX queue.
 */
struct uart_rx_queue {
    uint8_t head_index;
    uint8_t queued_elements;
    uint8_t uart_rx_buffer[UART_RX_QUEUE_MAX_NOE];
};

typedef struct uart_rx_queue uart_rx_queue_t;

/**
 * \brief THE one and only UART RX queue.
 */
static uart_rx_queue_t rx_queue;


void init_system_uart_rx_queue() {

    rx_queue.head_index = 0;
    rx_queue.queued_elements = 0;

}

uint8_t uart_rx_queue_is_full() {

    return (UART_RX_QUEUE_MAX_NOE == rx_queue.queued_elements);

}

uint8_t uart_rx_queue_is_empty() {

    return (0 == rx_queue.queued_elements);

}

uint8_t uart_rx_queue_extract_char(uint8_t * c) {

    uint8_t is_empty = uart_rx_queue_is_empty();

    if (!is_empty) {

        // Copy head_index element to *c
        *c = rx_queue.uart_rx_buffer[rx_queue.head_index];

        // Update head_index
        rx_queue.head_index = (rx_queue.head_index + 1) %
                UART_RX_QUEUE_MAX_NOE;

        // Decrease the number of queued elements
        rx_queue.queued_elements = rx_queue.queued_elements - 1;

    }

    return (is_empty);

}

uint8_t uart_rx_queue_insert_char(uint8_t c) {

    uint8_t is_full = uart_rx_queue_is_full();

    if (!is_full) {

        uint8_t tail_index;

        // Obtain the last used element of the queue and store its index
        // into tail_index
        tail_index = (rx_queue.head_index +
                rx_queue.queued_elements) % UART_RX_QUEUE_MAX_NOE;

        // Store the descriptor
        rx_queue.uart_rx_buffer[tail_index] = c;

        // Increase the number of queued elements
        rx_queue.queued_elements = rx_queue.queued_elements + 1;

    }

    return (is_full);

}
