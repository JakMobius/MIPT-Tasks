
#include "list.h"
#include "list-tester.h"

#define bad_syntax() {                                                                              \
        fprintf(stderr, "Bad syntax on line %d\n", line);                                           \
        return;                                                                                     \
    }

#define read_command()                                                                              \
    if(sscanf(script, "%15s%n", command, &scanned_characters) != 1) bad_syntax();                   \
    script += scanned_characters;

#define read_element()                                                                              \
    if(sscanf(script, "%lf%n", &element, &scanned_characters) != 1) bad_syntax();                   \
    script += scanned_characters;

#define read_index()                                                                                \
    if(sscanf(script, "%d%n", &index, &scanned_characters) != 1) bad_syntax();                      \
    script += scanned_characters;

#define wrap_list_call(call) {                                                                      \
        e_mlist_error error;                                                                        \
        error = call;                                                                               \
        if(error) {                                                                                 \
            fprintf(stderr, "Test failed on line %d: %s\n", line, MLIST_ERROR_DESCRIPTIONS[error]); \
            return;                                                                                 \
        }                                                                                           \
    }
#define validate_list() {                                                                           \
    e_mlist_error error = mlist_validate(list);                                                     \
    if(error) {                                                                                     \
        fprintf(stderr, "List validation failed on line %d: %s\n", line,                            \
            MLIST_ERROR_DESCRIPTIONS[error]);                                                       \
        return;                                                                                     \
    }                                                                                               \
}

void run_test_script(const char* script) {
    
    s_mlist* list = mlist_new(8);
    
    double element = 0;
    char command[16];
    int scanned_characters = 0;
    int index = 0;
    int line = 1;
    
    while(1) {
        while(*script == '\n' || *script == ' ') {
            if(*script == '\n') line++;
            script++;
        }
        if(*script == '\0') break;
        
        read_command();
        
        if(strcmp(command, "add") == 0) {
            
            read_element();
            read_command();
            
            if(strcmp(command, "to") == 0) {
                read_command();
                
                if(strcmp(command, "head") == 0) {
                    wrap_list_call(mlist_insert_head(list, element));
                } else if(strcmp(command, "tail") == 0) {
                    wrap_list_call(mlist_insert_tail(list, element));
                }
            } else if(strcmp(command, "at") == 0) {
                read_index();
                
                mlist_index address;
                wrap_list_call(mlist_linear_address_search(list, index, &address));
                
                wrap_list_call(mlist_insert(list, address, element));
            }
            validate_list();
        } else if(strcmp(command, "del") == 0) {
            read_command();
            
            if(strcmp(command, "head") == 0) {
                wrap_list_call(mlist_delete_head(list, NULL));
            } else if(strcmp(command, "tail") == 0) {
                wrap_list_call(mlist_delete_tail(list, NULL));
            } else if(strcmp(command, "index") == 0) {
                read_index();
                
                mlist_index address;
                wrap_list_call(mlist_linear_address_search(list, index, &address));
                
                wrap_list_call(mlist_delete(list, address, NULL));
            }
            validate_list();
        } else if(strcmp(command, "chk") == 0) {
            double element;
            int index = 0;
            int failed = 0;
            
            while(*script == ' ') script++;
            
            for(mlist_index i = list -> head; i != list -> tail; i = list -> next_indices[i]) {
                if(*script == '\n' || *script == '\0') {
                    printf("Check failed: list is not empty as expected\n");
                    failed = 1;
                    break;
                }
                
                read_element();
                    
                if(list -> element_array[i] != element) {
                    printf("Check failed on line %d: element on index %d expected to be %lg, but found %lg\n", line, index, element, list -> element_array[i]);
                    failed = 1;
                    break;
                }
                
                index++;
                
                while(*script == ' ') script++;
                if(*script == '\n' || *script == '\0') {
                    if(list -> next_indices[i] != list -> tail) {
                        printf("Check failed: list is longer than expected\n");
                        failed = 1;
                        break;
                    }
                }
            }
            
            if(*script != '\n' && *script != '\0') {
                printf("Check failed: list is shorter than expected\n");
                break;
            }
            
        } else if(strcmp(command, "srt") == 0) {
            wrap_list_call(mlist_engage_madness(list));
            validate_list();
        } else if(strcmp(command, "assert_mad") == 0) {
            if(!list -> is_mad) {
                printf("Assert failed: list is not mad\n");
                return;
            }
        } else {
            bad_syntax();
        }
        
        while(*script != '\n' && *script != '\0') script++;
        if(*script == '\0') break;
        line++;
        script++;
    }
    
    mlist_release(list);
}

void run_test_file(const char* path) {
    FILE* file = fopen(path, "rb");
    
    if(!file) {
        fprintf(stderr, "Failed to open test script %s: %s\n", path, strerror(errno));
        return;
    }
    
    struct stat stat;
    if(fstat(fileno(file), &stat)) {
        fprintf(stderr, "Failed to open test script %s: %s\n", path, strerror(errno));
        return;
    }
    
    char* buffer = malloc(stat.st_size + 1);
    
    if(!buffer) {
        fprintf(stderr, "Buy more memory\n");
        return;
    }
    
    fread(buffer, 1, stat.st_size, file);
    buffer[stat.st_size] = 0;
    
    run_test_script(buffer);
    
    fclose(file);
}

void run_test_folder(const char* path) {
    struct dirent *entry;
    DIR *directory;
    
    unsigned long file_name_length = 128;
    unsigned long path_length = strlen(path);
    char* each_file_path = malloc(path_length + file_name_length);
    strcpy(each_file_path, path);
    
    if(!each_file_path) {
        fprintf(stderr, "Buy more memory\n");
        return;
    }

    directory = opendir(path);
    if (directory == NULL)
    {
        perror("Failed to open test script folder");
    }

    while((entry = readdir(directory))) {
        if(DT_REG == entry -> d_type) {
            unsigned long bytes = snprintf(each_file_path + path_length, file_name_length, "%s", entry -> d_name);
            
            if(bytes >= file_name_length) {
                file_name_length = bytes + 1;
                char* reallocated = realloc(each_file_path, path_length + file_name_length);
                
                if(!reallocated) {
                    free(each_file_path);
                    fprintf(stderr, "Buy more memory\n");
                    return;
                }
                each_file_path = reallocated;
                
                // This must succeed
                strcpy(each_file_path + path_length, entry -> d_name);
            }
            
            printf("-- running %s --\n", entry -> d_name);
            run_test_file(each_file_path);
            
        }
    }
    
    free(each_file_path);
    closedir(directory);
}
