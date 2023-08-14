#include "/home/ubuntu/gujianing/AFLplusplus/include/afl-fuzz.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VARIABLE_NAME_LENGTH 20
#define VARIABLE_NAME_CHARS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_"

typedef struct my_mutator {
    afl_state_t *afl;

    // Add any additional data here
} my_mutator_t;

// Generate a random variable name
void generateRandomVariableName(char *name) {
    size_t length = rand() % (MAX_VARIABLE_NAME_LENGTH - 1) + 1;
    for (size_t i = 0; i < length; i++) {
        name[i] = VARIABLE_NAME_CHARS[rand() % (sizeof(VARIABLE_NAME_CHARS) - 1)];
    }
    name[length] = '\0';
}

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

size_t afl_custom_fuzz(my_mutator_t *data, uint8_t *buf, size_t buf_size,
                       uint8_t **out_buf, uint8_t *add_buf,
                       size_t add_buf_size, size_t max_size) {

    // Make a copy of the input buffer
    uint8_t *mutated_buf = (uint8_t *)malloc(buf_size);
    memcpy(mutated_buf, buf, buf_size);

    // Find variable names and replace them
    for (size_t i = 0; i < buf_size; i++) {
        if (buf[i] == 'v' && i + 1 < buf_size && buf[i + 1] == 'a' && i + 2 < buf_size && buf[i + 2] == 'r') {
            // Generate a new random variable name
            char newVariableName[MAX_VARIABLE_NAME_LENGTH];
            generateRandomVariableName(newVariableName);

            // Replace the variable name
            size_t newNameLength = strlen(newVariableName);
            if (newNameLength < MAX_VARIABLE_NAME_LENGTH - 2) {
                memcpy(&mutated_buf[i], newVariableName, newNameLength);
                i += newNameLength - 1;
            }
        }
    }

    *out_buf = mutated_buf;
    return buf_size;
}

void afl_custom_deinit(my_mutator_t *data) {
    free(data);
}

