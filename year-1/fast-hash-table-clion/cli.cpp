//
// Created by Темыч on 12.04.2021.
//

#include "cli.h"

void doCLI() {
    hash_table table = {0};
    table.construct(300000);
    table.max_load_factor = 70;

    long long length = 0;
    char* file = read_file("../dict-en-rus.dic", &length);
    convert_file_to_dictionary(file, length);

    clock_t read_time = clock();
    read_dictionary_to_table(file, length, &table, nullptr);
    read_time = clock() - read_time;

    printf("Reading took %lf s.\n", (double)read_time / (double)CLOCKS_PER_SEC);

    char key[64] = {0};

    while(strcmp(key, "@exit") != 0) {
        memset(key, 0, 64);
        scanf("%63s", key);

        printf("%s\n", table.get(key));
    }

    table.release();
    free(file);
}