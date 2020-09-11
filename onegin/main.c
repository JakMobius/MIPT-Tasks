#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define MEASURE_TIME
#ifdef MEASURE_TIME
#include <time.h>
#endif

#define CHECK_POINTER(pointer) if(!pointer) memory_failure();

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
Node* build_string(const char* string, size_t length) {
    Node* pointer = (Node*)malloc(sizeof(Node) * length);
    
    CHECK_POINTER(pointer)
    
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

Node* add_string(Node* map, const char* string, size_t length) {
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
 * @brief Allocates a buffer and reads file at given path into it
 * @param [in] filepath Path to file to be readen
 * @param [out] length Length of readen file
 * @returns buffer with file data
 */

const char* read_file(const char* filepath, long long* length) {
    struct stat filedata;
    stat(filepath, &filedata);
    *length = filedata.st_size;
    
    char* buffer = (char*) malloc(*length);
    
    FILE* input = fopen(filepath, "rb");
    fread(buffer, 1, *length, input);
    fclose(input);
    
    return buffer;
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
    
    long long filesize;
    
    const char* filepath = "/Users/Temich/Downloads/input.txt";//argv[1];
    const char* buffer = read_file(filepath, &filesize);
    
    size_t file_walker = 0;
    size_t line_index = 0;
    size_t largest_line = 0;
    
    CHECK_POINTER(buffer)

    Node* begin = NULL;
    
#ifdef MEASURE_TIME
    clock_t start, end;
    clock_t total = 0;
#endif
    
    while (file_walker < filesize) {
        char c = buffer[file_walker];
        file_walker++;
        
        if(c == '\n') {
            
            size_t line_length = file_walker - line_index;
            if(line_length > largest_line) {
                largest_line = line_length;
            }
            
#ifdef MEASURE_TIME
            start = clock();
#endif
            Node* result = add_string(begin, buffer + line_index, line_length - 1);
            if(result != NULL) {
                begin = result;
            }

#ifdef MEASURE_TIME
            end = clock();
            
            total += end - start;
#endif
        
            line_index = file_walker;
            if(c == EOF) {
                break;
            }
        }
    }
    
#ifdef MEASURE_TIME
    double time_taken = ((double)total)/CLOCKS_PER_SEC;
    
    printf("### it took %f seconds to sort ###\n", time_taken);
#endif
    
    if(begin) {
        char* print_buffer = (char*) malloc(largest_line);
        
        CHECK_POINTER(print_buffer);
        
        print_tree(begin, print_buffer, 0);
        
        free(print_buffer);
    }
    
    free_tree(begin);
    free((void*) buffer);
    
    return 0;
}
 
