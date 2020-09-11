#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEASURE_TIME
#ifdef MEASURE_TIME
#include <time.h>
#endif

typedef struct Node Node;

struct Node {
    char allocated;
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
 * @brief Constructs new tree based on string
 *
 * @param [in] string The string that should be stored in new tree
 * @param [in] length The length of given string
 */
Node* build_string(char* string, size_t length) {
    Node* pointer = (Node*)malloc(sizeof(Node) * length);
    
    if(!pointer) {
        memory_failure();
    }
    
    Node* maps = NULL;
    Node* new_map = NULL;
    Node* previous = NULL;
    for(size_t i = 0; i < length; i++) {
        new_map = pointer;
        pointer = (Node*)((char*)pointer + sizeof(Node));
        
        if(!maps) {
            maps = new_map;
        }
        
        new_map -> item = string[i];
        new_map -> left = NULL;
        
        if(previous) {
            previous -> right = new_map;
            new_map -> allocated = 0;
        } else {
            new_map -> allocated = 1;
        }
        
        previous = new_map;
    }
    
    previous -> right = NULL;
    
    return maps;
}

/**
 * @brief Adding new entry to existing map
 *
 * @param [in] map Map which should store this string
 * @param [in] string String that should be stored
 * @param [in] length Length of the string
 *
 * @returns updated map entry point if it should change
 */

Node* add_string(Node* map, char* string, size_t length) {
    if(length == 0) {
        return NULL;
    }

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
            Node* new_map = build_string(string, length);
            new_map -> left = current;
            if(previous) {
                previous -> left = new_map;
            }
            
            if(previous) {
                return NULL;
            } else {
                return new_map;
            }
        }
        
        previous = current;
    }
    
    Node* new_map = build_string(string, length);
    
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
 * @param [in] map Tree to pring
 * @param [in] buffer Char buffer to use for printing
 * @param [in] depth Internal flag. Should be set to zero
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
 * @param [in] tree Tree to deallocare
 */

void free_tree(Node* tree) {
    while(tree) {
        if(tree -> right) free_tree(tree -> right);
        Node* next = tree -> left;
        if(tree -> allocated) free(tree);
        tree = next;
    }
}

/**
 * @brief Entry point
 *
 * Execution of the program
 * starts here.
 *
 * @param [in] argc Number of arguments
 * @param [in] argv List of arguments
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
    free(fraction);
    
    return 0;
}
 
