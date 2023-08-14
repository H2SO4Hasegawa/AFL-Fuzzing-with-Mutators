/*
  Replace Values with Random Values Mutator Custom Mutator for AFL

  Written by Gu Jianing
*/
#include "/home/ubuntu/gujianing/AFLplusplus/include/afl-fuzz.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct my_mutator {
    afl_state_t *afl;

    // Add any additional data here
} my_mutator_t;

// Check if a character is a digit
bool isDigit(char c) {
    return c >= '0' && c <= '9';
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

    // Make a copy of the input buffer
    uint8_t *mutated_buf = (uint8_t *)malloc(max_size);
    memcpy(mutated_buf, buf, buf_size);

    // Perform replacement of numerical values
    for (size_t i = 0; i < buf_size; i++) {
        if (isDigit(buf[i])) {
            // Generate a random number to replace the digit
            int newNumber = rand() % 10; // Modify this range as needed. 10 is good to prevent generating too big of an integer

            // Convert the number to string and replace the digit
            char numberStr[32];
            snprintf(numberStr, sizeof(numberStr), "%d", newNumber);

            size_t newNumberLen = strlen(numberStr);
            size_t oldNumberLen = 1;

            // Replace the numerical value with the new number
            if (newNumberLen > oldNumberLen) {
                memmove(&mutated_buf[i + newNumberLen], &mutated_buf[i + oldNumberLen], buf_size - i - oldNumberLen);
                memcpy(&mutated_buf[i], numberStr, newNumberLen);
                buf_size += newNumberLen - oldNumberLen;
                i += newNumberLen - 1;
            }
        }
    }

    *out_buf = mutated_buf;
    return buf_size;
}

void afl_custom_deinit(my_mutator_t *data) {
    free(data);
}

