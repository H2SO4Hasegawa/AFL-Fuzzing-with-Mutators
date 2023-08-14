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

  unsigned int seed;

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

// Character set for generating variable-like names
const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";

// Function to generate a random variable-like name
char *generate_variable_name(unsigned int seed, size_t name_length) {
    srand(seed);
    char *name = (char *)malloc((name_length + 1) * sizeof(char));
    if (name) {
        for (size_t i = 0; i < name_length; i++) {
            int index = rand() % (sizeof(charset) - 1);
            name[i] = charset[index];
        }
        name[name_length] = '\0';
    }
    return name;
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
  
  memcpy(*out_buf, buf, buf_size);
  
  *out_buf = afl_realloc(NULL,mutated_size);
  if(!*out_buf){
  	perror("afl_relloc");
  	return 0;
  }
  
  // Generate a random variable-like name
    char *random_name = generate_variable_name(data->seed, 8); // Change the 8 to desired name length

    // Replace a portion of the output buffer with the generated name
    size_t name_length = strlen(random_name);
    if (name_length > 0) {
        size_t replace_pos = rand() % (max_size - name_length);
        memcpy(*out_buf + replace_pos, random_name, name_length);
    }

    // Free the generated name as it is not needed anymore
    free(random_name);

    return max_size;

}

void afl_custom_deinit(my_mutator_t *data) {

}

