#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include "/home/ubuntu/gujianing/AFLplusplus/include/afl-fuzz.h"

int afl_custom_init(afl_state_t *afl, unsigned int seed) {
    // Perform any necessary initialization here
    // (e.g., seed your random number generator)
    srand(seed);
    return 0; // Return 0 on success, or a negative value on failure
}

void afl_custom_fuzz(afl_state_t *afl, uint8_t *data, size_t size, uint8_t **out_buf, size_t *out_size) {
    // Allocate memory for the mutated data
    *out_size = size;
    *out_buf = afl_realloc(NULL, size);
    if (!*out_buf) {
        perror("afl_realloc");
        return;
    }

    // Copy the original data into mutated_data
    memcpy(*out_buf, data, size);

    // Find variable names and replace/remove them
    size_t i, j, k;
    size_t new_size = size;
    for (i = 0; i < new_size - 2; i++) {
        if ((*out_buf)[i] == 'v' && (*out_buf)[i + 1] == 'a' && (*out_buf)[i + 2] == 'r' && (i == 0 || !isalnum((*out_buf)[i - 1]))) {
            // Found the start of a potential variable name
            size_t var_start = i;
            size_t var_end = i + 3;
            while (var_end < new_size && isalnum((*out_buf)[var_end])) {
                var_end++;
            }

            // Randomly choose a mutation operation
            int mutation = rand() % 2;
            switch (mutation) {
                case 0:
                    // Replace the variable name with a new random name
                    for (j = var_start; j < var_end; j++) {
                        (*out_buf)[j] = (uint8_t)('a' + rand() % 26);
                    }
                    break;
                case 1:
                    // Remove the variable name
                    size_t var_length = var_end - var_start;
                    for (k = var_start; k < new_size - var_length; k++) {
                        (*out_buf)[k] = (*out_buf)[k + var_length];
                    }
                    new_size -= var_length;
                    break;
                // Add more mutation cases as needed
            }
        }
    }

    // Adjust the output buffer size after mutation
    *out_size = new_size;
    *out_buf = afl_realloc(*out_buf, new_size);
}

void afl_custom_post_process(afl_state_t *afl, uint8_t *data, size_t size, uint8_t **out_buf, size_t *out_size) {
    // No post-processing needed for this mutator
}

void afl_custom_deinit(afl_state_t *afl) {
    // No cleanup needed for this mutator
}

