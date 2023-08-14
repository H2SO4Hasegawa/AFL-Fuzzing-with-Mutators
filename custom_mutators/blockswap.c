/*
  Simple Custom Mutator for AFL

  Written by Khaled Yakdan <yakdan@code-intelligence.de>

  This a simple mutator that assumes that the generates messages starting with
  one of the three strings GET, PUT, or DEL followed by a payload. The mutator
  randomly selects a commend and mutates the payload of the seed provided as
  input.
*/

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static size_t data_size = 100;

size_t afl_custom_mutator(uint8_t *data, size_t size, uint8_t *mutated_out,
                          size_t max_size, unsigned int seed) {

  // Seed the PRNG
  srand(seed);

  // Make sure that the packet size does not exceed the maximum size expected by
  // the fuzzer
  size_t mutated_size = data_size <= max_size ? data_size : max_size;
size_t block_size = 4;  // Size of each block to swap (adjust as needed)
 
    // Copy the original data into mutated_data
    size_t i;
    for (i = 0; i < size && i < max_size; i++) {
        mutated_out[i] = data[i];
    }

    // Perform block swapping
    size_t num_blocks = size / block_size;
    for (i = 0; i < num_blocks; i++) {
        size_t block1 = rand() % num_blocks;  // Randomly select first block
        size_t block2 = rand() % num_blocks;  // Randomly select second block

        // Swap the selected blocks
        for (size_t j = 0; j < block_size; j++) {
            size_t index1 = block1 * block_size + j;
            size_t index2 = block2 * block_size + j;
            uint8_t temp = mutated_out[index1];
            mutated_out[index1] = mutated_out[index2];
            mutated_out[index2] = temp;
        }
    }


  return mutated_size;

}

