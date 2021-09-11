
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

char* read_file(const char* filepath, long long* length);
void write_file(const char* filepath, char* buffer, long long file_length);
unsigned long hash(char *str, long long file_length);
int main(int argc, const char** argv);

char* read_file(const char* filepath, long long* length) {
    struct stat filedata = {};
    if(stat(filepath, &filedata) == -1) {
        perror("Unable to read file");
        return NULL;
    }
    *length = filedata.st_size;
    
    char* buffer = (char*) malloc(*length);
    
    if(!buffer) {
        printf("Insufficient memory\n");
        return NULL;
    }
    
    FILE* input = fopen(filepath, "rb");
    fread(buffer, 1, *length, input);
    fclose(input);
    
    return buffer;
}

void write_file(const char* filepath, char* buffer, long long length) {
    FILE* output = fopen(filepath, "w");
    
    if(output == NULL) {
        perror("Unable to write to file");
        return;
    }
    
    fwrite(buffer, length, 1, output);
    fclose(output);
}

unsigned long hash(char *str, long long file_length) {
    unsigned long hash = 5381;
    int c = 0;

    while (file_length--) {
        c = *str++;
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

int main(int argc, const char** argv) {
    if(argc <= 1) {
        printf("Specify input file\n");
        return 0;
    }
    
    const char* target = argv[1];
    long long file_length = 0;
    char* file = read_file(target, &file_length);
    if(!file) return 0;
    
    long long file_hash = hash(file, file_length);
    
    if(file_hash == 15023320726234864276ul && file_length == 205) {
        printf("Program is already patched\n");
        free(file);
        return 0;
    }

    if(file_hash != 17818423926618907797ul && file_length == 205) {
        printf("Invalid binary\n");
        free(file);
        return 0;
    }
    
    file[0x51] = 0x90;
    file[0x52] = 0x90;
    file[0x4b] = 0xc0;
    file[0x4c] = 0x01;
    
    printf("Patched successfully\n");
    
    write_file(target, file, file_length);
    free(file);
}
