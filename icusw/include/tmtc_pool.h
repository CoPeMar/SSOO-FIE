#ifndef INCLUDE_TMTC_POOL_H_
#define INCLUDE_TMTC_POOL_H_

/**
 * \brief Initializes the TM/TC pool static structures.
 *
 * @return RTEMS_SUCCESSFULL if the pool structures were initialized correctly
 *         or an error value otherwise.
 */
rtems_status_code init_tmtc_pool();
/**
 * \brief Allocates a new block from the memory pool.
 *
 * @return The address of the allocated block or NULL if there were no free
 *         blocks in the pool.
 */
uint8_t * tmtc_pool_alloc();

/**
 * \brief Frees a previously allocated block of the memory pool.
 *
 * @param p_block the address of the block to deallocate.
 */
void tmtc_pool_free(uint8_t * p_block);

#endif /* INCLUDE_TMTC_POOL_H_ */
