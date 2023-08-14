README

This project is a fuzzing project that used the combination of dictionary file and custom mutators to fuzz target clang++ and clang format. 

The file structure of the project is as follows:

.
└── AFL_Fuzzing_with_Mutators/
    ├── custom_mutators/
    │   └── (custom mutator files and shared library files)
    ├── dictionary/
    │   └── clang.dict
    ├── scripts/
    │   ├── 0-create-swap
    │   ├── 0-install_afl_plusplus_from_src
    │   ├── 0-install-compilers
    │   └── 1-instrument-llvm
    └── test_data/
        └── test_data_dir (example bitflip_1hr)/
            └── default/
                ├── .synced
                ├── crashes/
                │   └── files_that_triggered_crash
                ├── hangs/
                │   └── files_that_triggered_hangs
                ├── queue/
                │   ├── .state/
                │   │   ├── auto_extras/
                │   │   ├── deterministic_done/
                │   │   ├── redundant_edges/
                │   │   │   └── redundant_test_cases
                │   │   └── variable_behaviour/
                │   └── input_test_cases_in_queue
                ├── cmdline
                ├── fuzz_bitmap
                ├── fuzzer_setup
                ├── fuzzer_stats
                └── plot_data

The project can be set up by going through the following steps:

1.	Set up an ubuntu machine. 
2.	At the home folder, create a 2 GB, 4 GB and 16 GB swap file respectively. Running 0-create-swap.sh will do. Adjust the size of the swap files according to needs.
3.	Create a folder where the Fuzzing Process is to be done. Go into the folder.
4.	Run 0-install-compilers.sh.
5.	Run 0-install-afl_plusplus_from_src.sh.
6.	Run 1-instrument-llvm.sh.

To run AFL++ with desired custom mutator, an example command is shown below:
AFL_CUSTOM_MUTATOR_LIBRARY="/home/ubuntu/gujianing/custom_mutators/example.so;" afl-fuzz -m 90000000000 -i corpus -o output13 -f a.c -x clang.dict build/bin/clang++ a.c

AFL_CUSTOM_MUTATOR is the location of the custom mutators.
-m overrides the amount of memory limit for the executed process
-f indicates the output of the mutated data (mostly for viewing while the fuzzing session is going on)
-x loads the dictionary file
-i is the input corpus
-o is the ouput data

To compile the mutator file, afl-clang must be used. An example command would be:
afl-clang -O3 -Wall -shared -o insertdelete.so insertdelete.c

