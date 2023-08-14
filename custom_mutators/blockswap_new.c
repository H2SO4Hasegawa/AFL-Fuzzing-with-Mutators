/*
  Simple Custom Mutator for AFL

  Written by Khaled Yakdan <yakdan@code-intelligence.de>

  This a simple mutator that assumes that the generates messages starting with
  one of the three strings GET, PUT, or DEL followed by a payload. The mutator
  randomly selects a commend and mutates the payload of the seed provided as
  input.
*/

#include "/home/ubuntu/gujianing/AFLplusplus/include/afl-fuzz.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define DATA_SIZE (100)

typedef struct my_mutator {

  afl_state_t *afl;

  u8 *mutated_out;

} my_mutator_t;

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

  srand(seed);  // needed also by surgical_havoc_mutate()

  my_mutator_t *data = calloc(1, sizeof(my_mutator_t));

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
                       u8 **out_buf, uint8_t *add_buf,
                       size_t add_buf_size,  // add_buf can be NULL
                       size_t max_size) {

  // Make sure that the packet size does not exceed the maximum size expected by
  // the fuzzer
  size_t mutated_size = DATA_SIZE <= max_size ? DATA_SIZE : max_size;
  
  size_t block_size = 4;

  memcpy(data->mutated_out, buf, buf_size);
  
  *out_buf = afl_realloc(NULL,mutated_size);
  if(!*out_buf){
  	perror("afl_relloc");
  	return 0;
  }
  
  // Copy the original data into mutated_data
    size_t i;
    for (i = 0; i < buf_size; i++) {
        (*out_buf)[i] = buf[i];
    }

    // Perform block swapping
    size_t num_blocks = buf_size / block_size;
    for (i = 0; i < num_blocks; i++) {
        size_t block1 = rand() % num_blocks;  // Randomly select first block
        size_t block2 = rand() % num_blocks;  // Randomly select second block

        // Swap the selected blocks
        for (size_t j = 0; j < block_size; j++) {
            size_t index1 = block1 * block_size + j;
            size_t index2 = block2 * block_size + j;
            uint8_t temp = (*out_buf)[index1];
            (*out_buf)[index1] = (*out_buf)[index2];
            (*out_buf)[index2] = temp;
        }
    }


  return mutated_size;

}

void afl_custom_deinit(my_mutator_t *data) {

}

