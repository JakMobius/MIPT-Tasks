#include <stdio.h>
#include <zconf.h>
#include "file_io.h"
#include "tree.h"
#include "tree_parser.h"
#include "akinator.h"

int main(__unused int argc, __unused const char * argv[]) {

    s_tree* tree = tree_new(72);

    const char* database_path = "./database.db";

    const char* database;
    file_op_result result = read_file(database_path, &database, NULL);

    if(result != FILE_OP_OK) {
        printf("Error reading file\n");
        return 0;
    }

    tree_parser_read_tree(tree, database);

    main_akinator_loop(tree);

    tree_release(tree);
    
    return 0;
}
