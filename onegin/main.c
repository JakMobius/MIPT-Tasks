#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEASURE_TIME
#ifdef MEASURE_TIME
#include <time.h>
#endif

typedef struct Node Node;

struct Node {
    char item;
    Node* left;
    Node* right;
};

/**
 * @brief Dispays allocation error message and crashes the program afterwards
 */
void memory_failure() {
    perror("Failed to allocate memory\n");
    exit(-1);
}

/**
 * @brief Creates a new tree node
 *
 * @returns newly-created tree node
 */
Node* create_node() {
    Node* map = (Node*)malloc(sizeof(Node));
    
    if(!map) {
        memory_failure();
    }
    
    map -> item = '\0';
    map -> left = NULL;
    map -> right = NULL;
    return map;
}

/**
 * @brief Constructs new tree based on string
 *
 * This function should be used to
 * initialize long branches. It's much faster
 * than calling add_string method with NULL
 * map.
 *
 * @param string The string that should be stored in new tree
 * @param length The length of given string
 */
Node* build_string(char* string, size_t length) {
    Node* maps = NULL;
    Node* new_map = NULL;
    Node* previous = NULL;
    for(size_t i = 0; i < length; i++) {
        new_map = (Node*)malloc(sizeof(Node));
        
        if(!new_map) {
            memory_failure();
        }
        
        if(!maps) {
            maps = new_map;
        }
        
        new_map -> item = string[i];
        new_map -> left = NULL;
        new_map -> right = NULL;
        
        if(previous) {
            previous -> right = new_map;
        }
        
        previous = new_map;
    }
    
    return maps;
}

/**
 * @brief Adding new entry to existing map
 *
 * @param map Map which should store this string
 * @param string String that should be stored
 * @param length Length of the string
 *
 * @returns updated map entry point if it should change
 */

Node* add_string(Node* map, char* string, size_t length) {
    if(length == 0) return NULL;

    char start = string[0];

    Node* current;
    Node* previous = NULL;
    
    for(current = map; current; current = current -> left) {
        if(current -> item == start) {
            Node* result = add_string(current -> right, string + 1, length - 1);
            if(result) {
                current -> right = result;
            }
            
            return NULL;
        } else if(current -> item > start) {
            Node* new_map = create_node();
            new_map -> item = start;
            new_map -> left = current;
            if(previous) {
                previous -> left = new_map;
            }
            
            new_map -> right = build_string(string + 1, length - 1);
            
            if(previous) {
                return NULL;
            } else {
                return new_map;
            }
        }
        
        previous = current;
    }
    
    Node* new_map = create_node();
    new_map -> item = start;
    new_map -> right = build_string(string + 1, length - 1);
    
    if(previous) {
        previous -> left = new_map;
        return NULL;
    } else {
        return new_map;
    }
}

/**
 * @brief Prints the tree
 *
 * @param map Tree to pring
 * @param buffer Char buffer to use for printing
 * @param depth Internal flag. Should be set to zero
 */

void print_tree(Node* map, char* buffer, size_t depth) {
    if(!map) {
        return;
    }
    
    Node* current;
    
    for(current = map; current; current = current -> left) {
        buffer[depth] = current -> item;
        if(current -> right) {
            print_tree(current -> right, buffer, depth + 1);
        } else {
            buffer[depth + 1] = '\0';
            printf("%s\n", buffer);
        }
    }
}

/**
 * @brief Deallocates the tree
 *
 * @param tree Tree to deallocare
 */

void free_tree(Node* tree) {
    while(tree) {
        if(tree -> right) free_tree(tree -> right);
        Node* next = tree -> left;
        free(tree);
        tree = next;
    }
}

/**
 * @brief Entry point
 *
 * Execution of the program
 * starts here.
 *
 * @param argc Number of arguments
 * @param argv List of arguments
 *
 * @return Program exit status
*/
int main(int argc, const char * argv[]) {
    
    FILE* input = fopen(argv[1], "r");
    
    size_t fraction_length = 1024;
    size_t buffer_length = 256;
    size_t delta_buffer_length = 256;
    
    char* buffer = (char*) malloc(buffer_length);
    char* fraction = (char*) malloc(fraction_length);
    
    if(!fraction) {
        memory_failure();
    }
    
    if(!buffer) {
        memory_failure();
    }
    
    size_t fraction_index = fraction_length;
    size_t line_length = 0;

    Node* begin = NULL;
    
#ifdef MEASURE_TIME
    clock_t start, end;
    clock_t total = 0;
#endif
    
    while (1) {
        char c;
        
        if(fraction_index == fraction_length) {
            fraction_index = 0;
            size_t readen = fread(fraction, 1, fraction_length, input);
            if(readen < fraction_length) {
                fraction[readen] = EOF;
            }
        }
        
        c = fraction[fraction_index];
        
        fraction_index++;
        
        if(c == '\n' || c == EOF) {
            
#ifdef MEASURE_TIME
            start = clock();
#endif
            Node* result = add_string(begin, buffer, line_length);
            if(result != NULL) {
                begin = result;
            }

#ifdef MEASURE_TIME
            end = clock();
            
            total += end - start;
#endif
        
            line_length = 0;
            if(c == EOF) {
                break;
            }
        } else {
            buffer[line_length] = c;
            line_length++;
            if(line_length == buffer_length) {
                buffer_length += delta_buffer_length;
                buffer = realloc(buffer, buffer_length);
            }
        }
    }
    
#ifdef MEASURE_TIME
    double time_taken = ((double)total)/CLOCKS_PER_SEC;
    
    printf("### it took %f seconds to sort ###\n", time_taken);
#endif
    
    if(begin) {
        print_tree(begin, buffer, 0);
    }
    
    fclose(input);
    free_tree(begin);
    free(buffer);
    
    return 0;
}
 
