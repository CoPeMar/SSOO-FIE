#include <rtems.h>

#include "tmtc_pool.h"

/**
 * \brief Maximum length of the memory block.
 */
#define TMTC_MAX_LENGTH       256

/**
 * \brief Maximum number of elements that can be allocated.
 */
#define TMTC_POOL_MAX_NOE    20

/**
 * \brief Identifier of the pool mutex.
 */
static rtems_id tmtc_pool_mutex_id;

/**
 * \brief A memory pool for managing blocks of 256 unsigned 8-bit integers.
 *
 * This structure keeps track of the available free blocks and provides an
 * array of pre-allocated blocks for usage.
 */
struct tmtc_pool {

    uint8_t free_blocks[TMTC_POOL_MAX_NOE];

    uint8_t blocks[TMTC_POOL_MAX_NOE][TMTC_MAX_LENGTH];

};

/**
 * \brief The one and only instance of the pool.
 */
static struct tmtc_pool the_tmtc_pool;

rtems_status_code init_tmtc_pool() {

    rtems_status_code status = RTEMS_SUCCESSFUL;

    for (uint8_t i = 0; i < TMTC_POOL_MAX_NOE; i++){
    	the_tmtc_pool.free_blocks[i] = 1; //1 significa que el bloque está libre. 0 es que está ocupado
    }

    // Creación del mutex
    status = rtems_semaphore_create(rtems_build_name('P', 'M', 'T', 'X'),
    						1,
    						RTEMS_BINARY_SEMAPHORE,
    						0,
    						&tmtc_pool_mutex_id);

    return status;
}


uint8_t * tmtc_pool_alloc() {

    uint8_t * ret = NULL;

    for(uint8_t i = 0; i < TMTC_POOL_MAX_NOE; i++){// Búsqueda lineal O(n)
    	rtems_semaphore_obtain(tmtc_pool_mutex_id, RTEMS_WAIT, RTEMS_NO_TIMEOUT); // Coge el semáforo
    	if(the_tmtc_pool.free_blocks[i] == 1){ // Vemos si el bloque está libre
    		the_tmtc_pool.free_blocks[i] = 0; // Ocupamos el bloque
    		ret = the_tmtc_pool.blocks[i]; // Transferimos la dirección del bloque a ret
    		break;
    	}
    	rtems_semaphore_release(tmtc_pool_mutex_id); // Suelta el semáforo
    }
    return ret;

}


void tmtc_pool_free(uint8_t * p_block) {

    uint32_t index, alignment;

    // Obtain the index of the block within the array
    index = (p_block - the_tmtc_pool.blocks[0]) / TMTC_MAX_LENGTH;
    alignment = (p_block - the_tmtc_pool.blocks[0]) % TMTC_MAX_LENGTH;

    // Check that the index is within the limits of the memory pool and that
    // the address is aligned to the beginning of a block.

    if (index < TMTC_POOL_MAX_NOE && alignment == 0) {
    	rtems_semaphore_obtain(tmtc_pool_mutex_id, RTEMS_WAIT, RTEMS_NO_TIMEOUT); // Obtiene el semáforo
    	// Mark the block as free
        the_tmtc_pool.free_blocks[index] = 1;
        rtems_semaphore_release(tmtc_pool_mutex_id);
    }

}
