/*
  Insertion and Deletion Mutator Custom Mutator for AFL

  Written by Gu Jianing
*/
#include "/home/ubuntu/gujianing/AFLplusplus/include/afl-fuzz.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#define AFL_DEFER_SIZE 4096

typedef struct my_mutator {
    afl_state_t *afl;

    // Add any additional data here
} my_mutator_t;

// Generate a random Statement
void generateRandomStmt(char *Stmt) {
    // You can modify this function to generate different types of Statements, here it simply adds hello world
    snprintf(Stmt, AFL_DEFER_SIZE, "printf(\"Hello, world!\");");
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

    size_t mutated_size = buf_size;

    // Perform insertion or deletion of Statements
    for (size_t i = 0; i < buf_size; i++) {
        if (rand() % 2 == 0) {
            // Insert a new Statement
            char newStmt[AFL_DEFER_SIZE];
            generateRandomStmt(newStmt);

            // Insert the new Statement
            size_t newStmtLen = strlen(newStmt);
            if (mutated_size + newStmtLen + 1 <= max_size) {
                memmove(&mutated_buf[i + newStmtLen], &mutated_buf[i], mutated_size - i);
                memcpy(&mutated_buf[i], newStmt, newStmtLen);
                mutated_size += newStmtLen;
                i += newStmtLen;
            }
        } else {
            // Delete a Statement
            size_t StmtStart = i;
            while (i < buf_size && buf[i] != '\n') {
                i++;
            }
            if (i < buf_size && buf[i] == '\n') {
                // Check for a semicolon at the end of the line
                size_t lineEnd = i;
                while (lineEnd > StmtStart && (buf[lineEnd] == ' ' || buf[lineEnd] == '\t')) {
                    lineEnd--;
                }
                if (lineEnd > StmtStart && buf[lineEnd] == ';') {
                    memmove(&mutated_buf[StmtStart], &mutated_buf[i + 1], mutated_size - i - 1);
                    mutated_size -= i - StmtStart + 1;
                    i = StmtStart - 1;
                }
            }
        }
    }

    *out_buf = mutated_buf;
    return mutated_size;
}

void afl_custom_deinit(my_mutator_t *data) {
    free(data);
}

