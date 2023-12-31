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
#include "/home/ubuntu/gujianing/AFLplusplus/include/afl-fuzz.h"

static size_t data_size = 100;

size_t afl_custom_mutator(uint8_t *data, size_t size, uint8_t *mutated_out,
                          size_t max_size, unsigned int seed) {

  // Seed the PRNG
  srand(seed);

  // Make sure that the packet size does not exceed the maximum size expected by
  // the fuzzer
  size_t mutated_size = data_size <= max_size ? data_size : max_size;

  size_t num_mutations = rand() % (size / 8+1);
  size_t i;
  for(i = 0; i< size && i < max_size; i++){
  	mutated_out[i] = data[i];
  }
  
  for(i = 0; i< num_mutations; i++){
  	size_t index = rand() % size;
  	mutated_out[index] ^= (1 << (rand() %8));
  }
  

  // Mutate the payload of the packet
  /*for (int i = 3; i < mutated_size; i++) {

    mutated_out[i] = (data[i] + rand() % 10) & 0xff;

  }*/

  return mutated_size;

}

