
#ifndef list_dumper_h
#define list_dumper_h

typedef struct mlist_dumper s_mlist_dumper;

#include "list.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>

typedef enum mlist_dumper_error {
    LIST_DUMPER_OK,
    LIST_DUMPER_OUT_OF_MEMORY
} e_mlist_dumper_error;

typedef struct mlist_dumper {
    const char* target_folder_name;
    FILE* html_file;
    s_mlist* observed_list;
    int dump_images;
    
    unsigned long target_folder_name_length;
} s_mlist_dumper;

s_mlist_dumper* mlist_dumper_new(void);
e_mlist_dumper_error mlist_dumper_construct(s_mlist_dumper* thou);

void mlist_dumper_write_dot(s_mlist_dumper* thou, s_mlist* list, FILE* target);
const char* mlist_dumper_resource_path(s_mlist_dumper* thou, const char* resource, int length);
    
void mlist_dumper_add_image(s_mlist_dumper* thou, s_mlist* list);
void mlist_dumper_add_text(s_mlist_dumper* thou, const char* format, ...);

void mlist_dumper_set_target_folder(s_mlist_dumper* thou, const char* path);

void mlist_dumper_deconstruct(s_mlist_dumper* thou);
void mlist_dumper_release(s_mlist_dumper* thou);
void mlist_dumper_flush(s_mlist_dumper* thou);

void mlist_dump_resize_command(s_mlist_dumper* thou, s_mlist* list, mlist_index capacity);

void mlist_dump_insert_command(s_mlist_dumper* thou, s_mlist* list, mlist_index index, mlist_element_type element);
void mlist_dump_delete_command(s_mlist_dumper* thou, s_mlist* list, mlist_index index);
void mlist_dump_sort_command(s_mlist_dumper* thou, s_mlist* list);

#endif /* list_dumper_h */
