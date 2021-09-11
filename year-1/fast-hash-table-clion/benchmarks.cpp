
#include "benchmarks.h"

void doWriteBenchmark() {

    long long length = 0;
    char* file = read_file("../dict-en-rus.dic", &length);
    s_dynamic_array<char*> keys = {};
    keys.construct(160000);
    convert_file_to_dictionary(file, length);

    for(int i = 75; i < 100; i++) {
        hash_table table = {0};
        table.construct(300000);
        table.max_load_factor = i;

        clock_t read_time = clock();

        int key_amount = 0;
        char* key_array = read_dictionary_to_table(file, length, &table, &key_amount);

        read_time = clock() - read_time;

        printf("%d;%f\n", 75, (double)read_time / (double)CLOCKS_PER_SEC);
        //printf("collisions = %f\n", table.get_collision_factor());

        table.destruct();
        free(key_array);
    }

    free(file);
}

void doReadBenchmark() {
    long long length = 0;
    char* file = read_file("../dict-en-rus.dic", &length);
    convert_file_to_dictionary(file, length);

    for(int i = 70; i < 100; i++) {

        s_dynamic_array<char*> keys = {};
        keys.construct(256);
        hash_table table = {};
        table.construct(64);
        table.max_load_factor = i;
        int key_amount = 0;
        char* key_array = read_dictionary_to_table(file, length, &table, &key_amount);
        free(key_array);

        srand(time(nullptr));

        int max_operations = 1000000;

        clock_t read_time = clock();

        char word[64] = {0};

        for(int j = 0; j < max_operations; j++) {
            if(j % 1000 == 0) srand(time(nullptr));
            for(int k = 0; k < 16; k++) {
                ((int*)word)[k] = rand();
            }
            word[63] = 0;

            volatile const char* result = table.get(word);
        }

        read_time = clock() - read_time;

        table.destruct();
        keys.destruct();

        printf("%d;%f\n", i, (double)read_time / (double)CLOCKS_PER_SEC);
        //printf("collisions = %f\n", table.get_collision_factor());
    }

    free(file);
}