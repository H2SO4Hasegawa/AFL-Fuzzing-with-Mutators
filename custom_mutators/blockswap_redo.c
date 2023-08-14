/*
  Block Swapping Mutator Custom Mutator for AFL

  Written by Gu Jianing
*/

#include "/home/ubuntu/gujianing/AFLplusplus/include/afl-fuzz.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct my_mutator {
    afl_state_t *afl;

    // Add any additional data here
} my_mutator_t;

// Function to swap two blocks of code
void swapCodeBlocks(uint8_t *buf, size_t block1Start, size_t block1End, size_t block2Start, size_t block2End) {
    uint8_t tmp;
    while (block1Start <= block1End && block2Start <= block2End) {
        tmp = buf[block1Start];
        buf[block1Start] = buf[block2Start];
        buf[block2Start] = tmp;
        block1Start++;
        block2Start++;
    }
}

/**
 * Initialize this custom mutator
 *
 * @param[in] afl a pointer to the internal state object. Can be ignored for
 * now.
 * @param[in] seed A seed for this mutator - the same seed should always mutate
 * in the same way.
 * @return Pointer to the data object this custom mutator instance should use.
 *         There may be multiple instances of this mutator in one afl-fuzz run!
 *         Return NULL on error.
 */
my_mutator_t *afl_custom_init(afl_state_t *afl, unsigned int seed) {
    srand(seed); // Seed the random number generator

    my_mutator_t *data = (my_mutator_t *)malloc(sizeof(my_mutator_t));
    if (!data) {
        perror("afl_custom_init alloc");
        return NULL;
    }

    data->afl = afl;

    return data;
}

/**
 * Perform custom mutations on a given input
 *
 * (Optional for now. Required in the future)
 *
 * @param[in] data pointer returned in afl_custom_init for this fuzz case
 * @param[in] buf Pointer to input data to be mutated
 * @param[in] buf_size Size of input data
 * @param[out] out_buf the buffer we will work on. we can reuse *buf. NULL on
 * error.
 * @param[in] add_buf Buffer containing the additional test case
 * @param[in] add_buf_size Size of the additional test case
 * @param[in] max_size Maximum size of the mutated output. The mutation must not
 *     produce data larger than max_size.
 * @return Size of the mutated output.
 */

size_t afl_custom_fuzz(my_mutator_t *data, uint8_t *buf, size_t buf_size,
                       uint8_t **out_buf, uint8_t *add_buf,
                       size_t add_buf_size, size_t max_size) {

    // Select random positions for block swapping
    size_t block1Start = rand() % buf_size;
    size_t block1End = block1Start + (rand() % (buf_size - block1Start));
    size_t block2Start = rand() % buf_size;
    size_t block2End = block2Start + (rand() % (buf_size - block2Start));

    // Swap the selected blocks of code
    uint8_t *mutated_buf = (uint8_t *)malloc(buf_size);
    memcpy(mutated_buf, buf, buf_size);
    swapCodeBlocks(mutated_buf, block1Start, block1End, block2Start, block2End);

    *out_buf = mutated_buf;
    return buf_size;
}

void afl_custom_deinit(my_mutator_t *data) {
    free(data);
}

