
#include "list.h"

s_mlist_dumper* mlist_dumper_new() {
    s_mlist_dumper* thou = calloc(1, sizeof(s_mlist_dumper));
    
    if(thou == NULL) {
        return NULL;
    }
    
    if(mlist_dumper_construct(thou) != LIST_DUMPER_OK) {
        mlist_dumper_release(thou);
        return NULL;
    }
    
    return thou;
}

e_mlist_dumper_error mlist_dumper_construct(s_mlist_dumper* thou) {
    
    return LIST_DUMPER_OK;
}

const char* mlist_dumper_resource_path(s_mlist_dumper* thou, const char* resource, int length) {
    char* full_file_name = malloc(length + thou -> target_folder_name_length);
    
    strcpy(full_file_name, thou -> target_folder_name);
    strcpy(full_file_name + thou -> target_folder_name_length, resource);
    
    return full_file_name;
}

void mlist_dumper_write_dot(s_mlist_dumper* thou, s_mlist* list, FILE* target) {
    
    e_mlist_error error = mlist_validate(list);
    
    if(error != MLIST_OK) {
        mlist_dumper_add_text(thou, "<b><font color=red>validator check failed: %s</font></b>", MLIST_ERROR_DESCRIPTIONS[error]);
    }
    
    fprintf(target, "digraph list {\n");
    fprintf(target, "\trankdir=LR;\n\n");
    
    for(mlist_index i = 0; i < list -> capacity; i++) {
        fprintf(target, "\tf%u [", i);
        
        // Printing the label
        fprintf(target, " label=\"");
        if(i == 0) fprintf(target, "fictive");
        else fprintf(target, "[%d] = %lg ", i, list -> element_array[i]);
        fprintf(target, "\"");
        
        // Setting the node color
        fprintf(target, " color=");
        if(i <= list -> size) fprintf(target, "black");
        else fprintf(target, "gray");
        
        // Setting the font color
        fprintf(target, " fontcolor=");
        if(i <= list -> size) fprintf(target, "black");
        else fprintf(target, "gray");
        
        // Setting the shape
        fprintf(target, " shape=");
        if(i == 0) fprintf(target, "diamond");
        else fprintf(target, "box");
        
        fprintf(target, "]\n");
    }
    
    fprintf(target, "\n\tsubgraph clusterhead {\n");
    fprintf(target, "\t\tf%u\n", list -> head);
    fprintf(target, "\t\tcolor=white\n");
    fprintf(target, "\t\tlabel=\"%s\"\n", list -> head == list -> tail ? "head & tail" : "head");
    fprintf(target, "\t}\n");
    
    if(list -> head != list -> tail) {
        fprintf(target, "\n\tsubgraph clustertail {\n");
        fprintf(target, "\t\tf%u\n", list -> tail);
        fprintf(target, "\t\tcolor=white\n");
        fprintf(target, "\t\tlabel=\"%s\"\n", "tail");
        fprintf(target, "\t}\n");
    }
    
    int corrupted_indices = 0;
    
    for(mlist_index i = 0; i < list -> capacity; i++) {
        if(list -> next_indices[i] < 0 || list -> next_indices[i] >= list -> capacity) {
            fprintf(target, "\tc%u [fontcolor=red label=X color=red]\n", corrupted_indices);
            fprintf(target, "\tf%u -> c%u [color=green]\n", i, corrupted_indices);
            corrupted_indices++;
        } else {
            fprintf(target, "\tf%u -> f%u [color=green]\n", i, list -> next_indices[i]);
        }
        
        if(list -> prev_indices[i] < 0 || list -> prev_indices[i] >= list -> capacity) {
            fprintf(target, "\tc%u [fontcolor=red label=X color=red]\n", corrupted_indices);
            fprintf(target, "\tf%u -> c%u [color=red]\n", i, corrupted_indices);
            corrupted_indices++;
        } else {
            fprintf(target, "\tf%u -> f%u [color=red]\n", i, list -> prev_indices[i]);
        }
    }
    
    fprintf(target, "}\n");
}

void mlist_dump_insert_command(s_mlist_dumper* thou, s_mlist* list, mlist_index index, mlist_element_type element) {
    mlist_dumper_add_text(thou, "insert %lf ", element);
    if(index == list -> head) {
        if(index == list -> tail) {
            mlist_dumper_add_text(list -> dumper, "as first\n", index);
        } else {
            mlist_dumper_add_text(list -> dumper, "to head\n", index);
        }
    } else if(index == list -> tail) {
        mlist_dumper_add_text(list -> dumper, "to tail\n", index);
    } else {
        mlist_dumper_add_text(list -> dumper, "to index %d\n", index);
    }
}

void mlist_dump_delete_command(s_mlist_dumper* thou, s_mlist* list, mlist_index index) {
    mlist_dumper_add_text(thou, "delete ");
    
    if(index == list -> prev_indices[list -> tail]) {
        mlist_dumper_add_text(list -> dumper, "last element\n", index);
    } else if(index == list -> head) {
        mlist_dumper_add_text(list -> dumper, "first element\n", index);
    } else {
        mlist_dumper_add_text(list -> dumper, "element at index %d\n", index);
    }
}

void mlist_dump_sort_command(s_mlist_dumper* thou, s_mlist* list) {
    mlist_dumper_add_text(thou, "engage madness\n");
}

void mlist_dump_resize_command(s_mlist_dumper* thou, s_mlist* list, mlist_index capacity) {
    mlist_dumper_add_text(thou, "resize to %lu\n", capacity);
}

void mlist_dumper_add_image(s_mlist_dumper* thou, s_mlist* list) {
    char image_name[64] = {0};
    int image_index = thou -> dump_images++;
    
    int length = sprintf(image_name, "/%d.dot", image_index);
    
    const char* resource_path = mlist_dumper_resource_path(thou, image_name, length);
    
    FILE* file = fopen(resource_path, "w");
    
    mlist_dumper_write_dot(thou, list, file);
    
    fclose(file);
    free((void*) resource_path);
    
    int command_length = 0;
    char* command = NULL;
    
    while(true) {
        command_length = snprintf(command, command_length, "/usr/local/bin/dot -Tpng %s -o %s/%d.png -Gsize=0,0\\!", resource_path, thou -> target_folder_name, image_index) + 1;
        if(command != NULL) break;
        command = malloc(command_length);
    }
    
    system(command);
    
    free(command);
    
    mlist_dumper_add_text(thou, "\n<img src='./%d.png'/>\n", image_index);
}

void mlist_dumper_add_text(s_mlist_dumper* thou, const char* format, ...) {
    va_list ap;
    va_start(ap, format);
    vfprintf(thou -> html_file, format, ap);
    va_end(ap);
}

void mlist_dumper_deconstruct(s_mlist_dumper* thou) {
    mlist_dumper_flush(thou);
}

void mlist_dumper_set_target_folder(s_mlist_dumper* thou, const char* path) {
    thou -> target_folder_name = path;
    thou -> target_folder_name_length = strlen(thou -> target_folder_name);
    
    DIR* dir = opendir(path);
    
    if (!dir) {
        if(errno == ENOENT) {
            if(mkdir(path, 0700)) {
                fprintf(stderr, "%s failed to create folder: %s.\n", __func__, strerror(errno));
            }
        } else {
            fprintf(stderr, "%s failed to access folder: %s.\n", __func__, strerror(errno));
        }
        return;
    }
    
    closedir(dir);
    
    if(thou -> html_file) {
        fclose(thou -> html_file);
    }
    
    const char* resource_path = mlist_dumper_resource_path(thou, "/index.html", 11);
    
    thou -> html_file = fopen(resource_path, "wb");
    
    mlist_dumper_add_text(thou, "<pre><tt>");
    free((void*)resource_path);
}

void mlist_dumper_flush(s_mlist_dumper* thou) {
    if(thou -> html_file) {
        fclose(thou -> html_file);
    }
}

void mlist_dumper_release(s_mlist_dumper* thou) {
    if(thou == NULL) return;
    
    mlist_dumper_deconstruct(thou);
    free(thou);
}
