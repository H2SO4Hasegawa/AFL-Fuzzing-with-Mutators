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

  size_t num_mutations = rand() % (size / 4 + 1);  // Number of mutations (up to 1/4 of input size)

    // Copy the original data into mutated_data
    size_t i;
    for (i = 0; i < size && i < max_size; i++) {
        mutated_out[i] = data[i];
    }

    // Perform insertion and deletion mutations
    for (i = 0; i < num_mutations; i++) {
        if (rand() % 2 == 0) {
            // Insertion mutation
            size_t index = rand() % size;
            uint8_t value = (uint8_t)(rand() % 256);

            // Shift subsequent bytes to make space for the inserted byte
            for (size_t j = size - 1; j > index; j--) {
                mutated_out[j] = mutated_out[j - 1];
            }

            // Insert the new byte
            mutated_out[index] = value;

            size++;  // Increase the size of the mutated data
        } else {
            // Deletion mutation
            size_t index = rand() % size;

            // Shift subsequent bytes to fill the gap left by the deleted byte
            for (size_t j = index; j < size - 1; j++) {
                mutated_out[j] = mutated_out[j + 1];
            }

            size--;  // Decrease the size of the mutated data
        }
    }


  return mutated_size;

}

